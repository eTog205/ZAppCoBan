//dv_csdl.cpp
#include "csdl.h"
#include "dv_csdl.h"

#include <iostream>

//std::string thucthi_lenh(const std::string& command)
//{
//    std::ostringstream output;
//
//    try
//    {
//        bp::ipstream pipe_stream; // Luồng để nhận đầu ra từ lệnh
//        bp::child process(command, bp::std_out > pipe_stream);
//        std::string line;
//        bool kotimthaygoi = false;
//
//        while (pipe_stream && getline(pipe_stream, line))
//        {
//            output << line << "\n";
//
//
//            if (line.find("No package found matching input criteria") != std::string::npos)
//            {
//                kotimthaygoi = true;
//            }
//        }
//
//        process.wait(); // Chờ lệnh kết thúc
//
//        if (kotimthaygoi)
//        {
//            return "Package not found";
//        }
//
//        if (process.exit_code() != 0)
//        {
//            throw std::runtime_error("Lenh loi voi ma thoat: " + std::to_string(process.exit_code()));
//        }
//    } catch (const std::exception& ex)
//    {
//        std::cerr << "Error executing command: " << ex.what() << std::endl;
//        throw;
//    }
//
//    return output.str();
//}
//
//std::string loc_toi_n(const std::string& input)
//{
//    // Tìm vị trí của ký tự 'N'
//    const size_t pos = input.find('N');
//
//    // Nếu tìm thấy 'N', trả về chuỗi từ vị trí đó
//    std::string result;
//    if (pos != std::string::npos)
//    {
//        result = input.substr(pos);
//    } else
//    {
//        // Nếu không tìm thấy 'N', trả về chuỗi rỗng
//        return "";
//    }
//
//    // Kiểm tra ký tự cuối cùng có phải là '\n' không
//    if (result.back() == '\n')
//    {
//        result.pop_back(); // Xóa ký tự '\n' ở cuối chuỗi
//    }
//
//    return result;
//}
//
//std::string laydauvao(const std::string& tukhoa)
//{
//    //Tạo và thực thi lệnh winget search
//    const std::string lenh = "winget search " + tukhoa;
//    const std::string dau_vao = thucthi_lenh(lenh);
//    std::string daloc_dauvao = loc_toi_n(dau_vao);
//
//    return daloc_dauvao;
//}

void LogicXuLy::nap_du_lieu(giaodien& gd)
{
    int row_count = 0;
    if (get_row_count("Items", &row_count) == SQLITE_OK && row_count > 0)
    {
        gd.row_count = row_count;
    }

    gd.data.clear();

    const std::string sql = "SELECT ID, Name, Category FROM Items;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::vector<std::string> row;
            row.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))); // ID
            row.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))); // Name
            row.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))); // Category
            gd.data.push_back(row);
        }
    } else
    {
        std::cerr << "Lỗi lấy dữ liệu từ Items!" << std::endl;
    }

    sqlite3_finalize(stmt);
}

