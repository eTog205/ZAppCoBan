//get.cpp
#include "get.h"
#include "log_nhalam.h"

#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <fstream>


std::string send_http_request(const std::string& url, bool is_asset, int redirect_count)
{
	if (redirect_count > 5)
	{
		td_log(loai_log::loi, "Quá số lần redirect cho phép.");
		return "";
	}

	try
	{
		const std::string https_prefix = "https://";
		if (!url.starts_with(https_prefix))
		{
			td_log(loai_log::loi, "URL không hợp lệ: " + url);
			return "";
		}

		std::string url_without_https = url.substr(https_prefix.size());
		auto pos = url_without_https.find('/');
		if (pos == std::string::npos)
		{
			td_log(loai_log::loi, "URL không có target hợp lệ: " + url);
			return "";
		}

		std::string host = url_without_https.substr(0, pos);
		std::string target = url_without_https.substr(pos);

		net::io_context ioc;
		ssl::context ctx{ ssl::context::tlsv12_client };
		ctx.set_default_verify_paths();

		beast::ssl_stream<beast::tcp_stream> stream{ ioc, ctx };

		tcp::resolver resolver{ ioc };
		auto const results = resolver.resolve(host, "443");
		get_lowest_layer(stream).connect(results);

		if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str()))
		{
			beast::error_code ec{ static_cast<int>(::ERR_get_error()), net::error::get_ssl_category() };
			td_log(loai_log::loi, "SNI error: " + ec.message());
			throw beast::system_error{ ec };
		}

		stream.handshake(ssl::stream_base::client);

		http::request<http::empty_body> req{ http::verb::get, target, 11 };
		req.set(http::field::host, host);
		req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

		if (is_asset)
			req.set(http::field::accept, "application/octet-stream");

		http::write(stream, req);

		beast::flat_buffer buffer;
		//http::response<http::string_body> res;
		http::response<http::dynamic_body> res;

		http::read(stream, buffer, res);

		if (res.result() == http::status::found || res.result() == http::status::temporary_redirect || res.result() == http::status::permanent_redirect)
		{
			auto loc = res.find(http::field::location);
			if (loc != res.end())
			{
				auto new_url = std::string(loc->value().data(), loc->value().size());
				return send_http_request(new_url, is_asset, redirect_count + 1);
			}
			td_log(loai_log::loi, "Không tìm thấy trường 'Location' trong phản hồi redirect.");
			return "";
		}

		if (res.result() != http::status::ok)
		{
			td_log(loai_log::loi, "[send_http_request] HTTP lỗi: " + std::to_string(static_cast<int>(res.result())));
			return "";
		}

		beast::error_code ec;
		// Đóng socket của tầng dưới mà không chờ thông báo close_notify của TLS
		stream.next_layer().socket().shutdown(tcp::socket::shutdown_both, ec);
		if (ec)
		{
			td_log(loai_log::loi, "[send_http_request] Lỗi khi shutdown socket: " + ec.message());
		}

		stream.next_layer().socket().close(ec);
		if (ec)
		{
			td_log(loai_log::loi, "[send_http_request] Lỗi khi đóng socket: " + ec.message());
		}

		//return res.body();
		return buffers_to_string(res.body().data());
	} catch (const std::exception& e)
	{
		td_log(loai_log::loi, "[send_http_request] Ngoại lệ: " + std::string(e.what()));
		return "";
	}
}

// 🔹 Phân tích phản hồi JSON
json::value parse_json_response(const std::string& response_body)
{
	try
	{
		return json::parse(response_body);
	} catch (const std::exception& e)
	{
		td_log(loai_log::loi, "Lỗi parse JSON: " + std::string(e.what()));
		return {};
	}
}

std::string get_release_tag()
{
	const std::string url = "https://api.github.com/repos/eTog205/SuaKeyTepApp/releases/latest";
	const std::string response_body = send_http_request(url, false);
	if (response_body.empty())
	{
		td_log(loai_log::loi, "[get_release_tag] Không nhận được phản hồi từ GitHub API!");
		return "";
	}

	//không cần
	//td_log(loai_log::thong_bao, "[get_release_tag] Phản hồi JSON từ GitHub: " + response_body.substr(0, 500));

	json::value jv = parse_json_response(response_body);
	//cái này có thể không cần kiểm tra
	if (!jv.is_object() || !jv.as_object().contains("tag_name"))
	{
		td_log(loai_log::canh_bao, "Không tìm thấy 'tag_name' trong phản hồi JSON.");
		return "";
	}

	auto tag_name = json::value_to<std::string>(jv.as_object()["tag_name"]);
	td_log(loai_log::thong_bao, "Lấy phiên bản mới nhất thành công: " + tag_name);
	return tag_name;
}

// Hàm download_file: tải nội dung từ URL và ghi ra file (ở chế độ binary)
bool download_file(const std::string& url, const std::string& save_path)
{
	const std::string file_content = send_http_request(url, true);

	//không cần
	if (file_content.empty())
	{
		td_log(loai_log::loi, "[download_file] Nội dung tải về rỗng.");
		return false;
	}

	// Ghi nội dung nhận được ra file ở chế độ binary
	std::ofstream ofs(save_path, std::ios::binary);
	if (!ofs)
	{
		//không cần
		td_log(loai_log::loi, "[download_file] Không mở được file để ghi: " + save_path);
		return false;
	}
	ofs.write(file_content.data(), file_content.size());
	ofs.close();

	return true;
}

bool download_latest_release()
{
	const std::string host = "api.github.com";
	const std::string target = "/repos/eTog205/SuaKeyTepApp/releases/latest";

	const std::string response_body = send_http_request("https://" + host + target);
	if (response_body.empty())
	{
		td_log(loai_log::loi, "[download_latest_release] Không nhận được phản hồi từ GitHub API!");
		return false;
	}

	json::value jv = parse_json_response(response_body);
	if (!jv.is_object() || !jv.as_object().contains("assets"))
	{
		td_log(loai_log::canh_bao, "Không tìm thấy 'assets' trong phản hồi JSON.");
		return false;
	}

	auto& assets = jv.as_object()["assets"].as_array();
	if (assets.empty())
	{
		td_log(loai_log::canh_bao, "Bản phát hành mới nhất không chứa asset nào.");
		return false;
	}

	auto& asset = assets[0].as_object();
	if (!asset.contains("browser_download_url"))
	{
		td_log(loai_log::canh_bao, "Asset không chứa 'browser_download_url'.");
		return false;
	}

	const auto download_url = json::value_to<std::string>(asset["browser_download_url"]);

	// Lấy tên tệp gốc từ asset (trường "name")
	std::string file_name;
	if (asset.contains("name"))
	{
		file_name = json::value_to<std::string>(asset["name"]);
	} else
	{
		file_name = "ZTOG.zip";
	}

	return download_file(download_url, file_name);
}
