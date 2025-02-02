#include "csdl.h"

#include <boost/beast/core.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <fstream>
#include <iostream>

sqlite3* db = nullptr;

// 🔹 Hàm giải mã AES-128-CBC
std::string decrypt_aes(const std::string& ciphertext)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::string plaintext(ciphertext.size(), '\0');
    int len = 0, plaintext_len = 0;

    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, key, iv);
    EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plaintext.data()), &len,
                      reinterpret_cast<const unsigned char*>(ciphertext.data()), static_cast<int>(ciphertext.size()));
    plaintext_len += len;
    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plaintext.data()) + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len);
    return plaintext;
}

// 🔹 Đọc token từ `config.dat` (giải mã AES)
std::string load_token()
{
    std::ifstream file("config.dat", std::ios::binary);
    if (!file)
    {
        std::cerr << "❌ Không tìm thấy `config.dat`!\n";
        return "";
    }
    const std::string encrypted((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
    file.close();
    return decrypt_aes(encrypted);
}

// 🔹 Hàm giải mã Base64 nếu cần
std::string decode_base64(const std::string& encoded)
{
    std::string decoded;
    decoded.resize(beast::detail::base64::decoded_size(encoded.size()));
    auto len = beast::detail::base64::decode(decoded.data(), encoded.data(), encoded.size());
    decoded.resize(len.first);
    return decoded;
}

// 🔹 Hàm gửi request HTTP GET
std::string send_http_request(const std::string& host, const std::string& target, const std::string& token)
{
    try
    {
        net::io_context ioc;
        boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12_client);
        ctx.set_default_verify_paths();  // Cấu hình SSL

        tcp::resolver resolver(ioc);
        beast::ssl_stream<tcp::socket> stream(ioc, ctx);

        auto const results = resolver.resolve(host, "443");
        net::connect(stream.next_layer(), results.begin(), results.end());
        stream.handshake(boost::asio::ssl::stream_base::client);

        // Tạo request HTTP
        http::request<http::string_body> req{ http::verb::get, target, 11 };
        req.set(http::field::host, host);
        req.set(http::field::user_agent, "Boost.Beast");
        req.set(http::field::accept, "application/vnd.github.v3.raw");

        if (!token.empty())
        {
            req.set(http::field::authorization, "token " + token);
        }

        http::write(stream, req);

        // Nhận phản hồi
        beast::flat_buffer buffer;
        http::response<http::dynamic_body> res;
        read(stream, buffer, res);

        // Đóng kết nối
        beast::error_code ec;
        stream.shutdown(ec);
        if (ec == net::error::eof) { ec = {}; }
        if (ec) { throw beast::system_error(ec); }

        // Kiểm tra mã trạng thái HTTP
        if (res.result() != http::status::ok)
        {
            throw std::runtime_error("HTTP Error: " + std::to_string(static_cast<int>(res.result())));
        }

        return buffers_to_string(res.body().data());

    } catch (const std::exception& e)
    {
        std::cerr << "❌ Lỗi HTTP Request: " << e.what() << std::endl;
        return "";
    }
}

// 🔹 Hàm lấy nội dung `sql.db` từ GitHub API
std::string fetch_github_data(const std::string& owner, const std::string& repo, const std::string& file_path, const std::string& token)
{
    const std::string host = "api.github.com";
    const std::string target = "/repos/" + owner + "/" + repo + "/contents/" + file_path;

    std::string response = send_http_request(host, target, token);

    if (response.empty())
    {
        std::cerr << "❌ Không thể tải dữ liệu từ GitHub\n";
        return "";
    }

    // Kiểm tra nếu phản hồi là JSON metadata thay vì nội dung thực
    try
    {
        auto json_response = nlohmann::json::parse(response);

        if (json_response.contains("encoding") && json_response["encoding"] == "base64")
        {
            const std::string base64_content = json_response["content"];
            return decode_base64(base64_content);
        }
    } catch (...)
    {
        // Nếu lỗi parse JSON, có thể nội dung đã là raw data
        throw;
    }

    return response;
}

std::string fetch_github_file_metadata(const std::string& owner, const std::string& repo, const std::string& file_path, const std::string& token)
{
    const std::string host = "api.github.com";
    const std::string target = "/repos/" + owner + "/" + repo + "/commits?path=" + file_path;

    std::string response = send_http_request(host, target, token);

    //std::cout << "🔹 Commits API Response:\n" << response << std::endl;

    if (response.empty())
    {
        std::cerr << "❌ Không thể lấy metadata của file từ GitHub\n";
        return "";
    }

    return response; // Trả về metadata của commit
}

// 🔹 Hàm lưu nội dung ra file
void save_to_file(const std::string& filename, const std::string& data)
{
    std::ofstream outFile(filename);
    if (outFile)
    {
        outFile << data;
        outFile.close();
        std::cout << "✅ Nội dung được lưu vào file: " << filename << std::endl;
    } else
    {
        std::cerr << "❌ Lỗi khi ghi file: " << filename << std::endl;
    }
}

void luu_tepsha(const std::string& sha_file, const std::string& owner, const std::string& repo, const std::string& file_path, const std::string& decrypted_token)
{
    std::ofstream sha_file_out(sha_file);
    if (sha_file_out)
    {
        std::string metadata_response = fetch_github_file_metadata(owner, repo, file_path, decrypted_token);
        std::cout << "🔹 Phản hồi metadata từ GitHub:\n" << metadata_response << std::endl;

        if (!metadata_response.empty())
        {
            nlohmann::json metadata_json = nlohmann::json::parse(metadata_response);
            if (metadata_json.contains("sha"))
            {
                std::string new_sha = metadata_json["sha"];
                std::cout << "🔹 SHA mới nhận được từ GitHub: " << new_sha << std::endl;
                sha_file_out << new_sha;
            } else
            {
                std::cerr << "❌ Metadata không chứa `sha`!\n";
            }
        } else
        {
            std::cerr << "❌ Không nhận được metadata từ GitHub.\n";
        }
        sha_file_out.close();
    }
}

void capnhat_data()
{
    std::cout << "🔎 Đọc token từ file...\n";
    std::string decrypted_token = load_token();

    if (decrypted_token.empty())
    {
        std::cerr << "❌ Lỗi: cập nhật dữ liệu. Vui lòng tải phiên bản mới về!\n";
        return;
    }

    // Cấu hình repository và file cần lấy
    const std::string owner = "eTog205";
    const std::string repo = "SuaKeyTepApp";
    const std::string file_path = "sql.db";
    const std::string sha_file = "sql.sha";

    // 🔹 Kiểm tra xem tệp SHA có tồn tại không
    std::ifstream sha_file_in(sha_file);
    std::string old_sha;
    if (sha_file_in)
    {
        std::getline(sha_file_in, old_sha);
        sha_file_in.close();
    }

    // 🔹 Nếu SHA cũ tồn tại, lấy SHA mới để so sánh
    if (!old_sha.empty())
    {
        std::string metadata_response = fetch_github_file_metadata(owner, repo, file_path, decrypted_token);
        if (metadata_response.empty())
        {
            std::cerr << "❌ Không thể lấy metadata từ GitHub.\n";
            return;
        }

        // 🔹 Parse JSON để lấy `sha`
        nlohmann::json metadata_json;
        try
        {
            metadata_json = nlohmann::json::parse(metadata_response);
        } catch (const std::exception& e)
        {
            std::cerr << "❌ Lỗi khi parse metadata: " << e.what() << std::endl;
            return;
        }

        if (!metadata_json.is_array() || metadata_json.empty() || !metadata_json[0].contains("sha"))
        {
            std::cerr << "❌ Metadata không chứa thông tin `sha`.\n";
            return;
        }

        std::string new_sha = metadata_json[0]["sha"];

        // 🔹 Nếu SHA không thay đổi, không cần tải lại
        if (old_sha == new_sha)
        {
            std::cout << "✅ Dữ liệu không thay đổi (SHA trùng khớp), không cần tải lại.\n";
            return;
        }
    }

    // 🔹 Nếu tệp SHA không tồn tại hoặc SHA đã thay đổi, tải file mới
    std::cout << "🔄 Dữ liệu mới có phiên bản cập nhật, tiến hành tải...\n";

    std::string new_data = fetch_github_data(owner, repo, file_path, decrypted_token);
    if (!new_data.empty())
    {
        save_to_file("sql.db", new_data);

        // 🔹 Lưu SHA mới vào file để sử dụng lần sau
        std::ofstream sha_file_out(sha_file);
        if (sha_file_out)
        {
            std::string metadata_response = fetch_github_file_metadata(owner, repo, file_path, decrypted_token);

            //std::cout << "🔹 Metadata API Response:\n" << metadata_response << std::endl;

            if (!metadata_response.empty())
            {
                try
                {
                    nlohmann::json metadata_json = nlohmann::json::parse(metadata_response);

                    if (metadata_json.is_array() && !metadata_json.empty() && metadata_json[0].contains("sha"))
                    {
                        std::string new_sha = metadata_json[0]["sha"];
                        std::cout << "🔹 SHA mới nhận được từ GitHub: " << new_sha << std::endl;
                        sha_file_out << new_sha;
                    } else
                    {
                        std::cerr << "❌ JSON không chứa thông tin `sha`, kiểm tra phản hồi!\n";
                    }
                } catch (const std::exception& e)
                {
                    std::cerr << "❌ Lỗi khi parse JSON metadata: " << e.what() << std::endl;
                }
            } else
            {
                std::cerr << "❌ Không nhận được metadata từ GitHub.\n";
            }
            sha_file_out.close();
        }
        std::cout << "✅ Đã cập nhật dữ liệu và lưu SHA mới.\n";
    } else
    {
        std::cerr << "❌ Lỗi khi tải dữ liệu `sql.db`\n";
    }
}

int open_database_read_only(const char* dbName)
{
    return sqlite3_open_v2(dbName, &db, SQLITE_OPEN_READONLY, nullptr);
}

void close_database()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

int get_row_count(const char* table_name, int* row_count)
{
    *row_count = 0;
    const std::string sql = "SELECT COUNT(*) FROM " + std::string(table_name) + ";";
    sqlite3_stmt* stmt;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        //std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        *row_count = sqlite3_column_int(stmt, 0);
    } else
    {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return (rc == SQLITE_ROW) ? SQLITE_OK : rc;
}

// Hàm thực thi SQL với xử lý lỗi
int execute_sql(const char* sql)
{
    char* errMsg = nullptr;
    const int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Lỗi SQL: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    return rc;
}

// Tạo bảng nếu chưa có
int create_table()
{
    const auto sql = "CREATE TABLE IF NOT EXISTS Items ("
        "ID TEXT PRIMARY KEY, "
        "Name TEXT NOT NULL, "
        "Category TEXT);";

    return execute_sql(sql);
}

bool database_exists(const char* db_name)
{
    return std::filesystem::exists(db_name);
}

void khoidong_sql()
{
    std::cout << "Kiểm tra sql ....";
    if (!database_exists("sql.db"))
    {
        std::cout << "1. csdl không tồn tại ban đầu sẽ được tạo\n";
    }

    if (open_database_read_only("sql.db") != SQLITE_OK)
    {
        std::cout << "2. không mở được sql!\n";
    }
    std::cout << "các kiểm tra đã thực hiện xong\n";
}



