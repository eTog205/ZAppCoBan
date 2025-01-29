
#include "timvatai.h"

#include <fstream>
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

// Hàm đọc tệp JSON vào đối tượng JSON
bool tai_tep_json(const std::string& tentep, json& dl_json)
{
    std::ifstream file(tentep);
    if (!file.is_open())
    {
        std::cerr << "Không thể mở tệp: " << tentep << std::endl;
        return false;
    }
    file >> dl_json;
    file.close();
    return !dl_json.is_discarded();
}

// Hàm tải danh sách phần mềm từ tệp JSON
int tai_ds_pm(const std::string& tentep, PhanMem ds_PhanMem[], int kt_toida)
{
    json jsonData;
    if (!tai_tep_json(tentep, jsonData))
    {
        return 0;
    }

    int count = 0;
    for (const auto& item : jsonData)
    {
        if (count >= kt_toida)
        {
            break; // Đảm bảo không vượt quá kích thước mảng
        }
        ds_PhanMem[count].id = item.value("id", "");
        ds_PhanMem[count].ten = item.value("name", "");
        ds_PhanMem[count].phienban_caidat = item.value("version_installed", "");
        ds_PhanMem[count].phienban_moinhat = item.value("version_latest", "");
        ds_PhanMem[count].phanloai = item.value("category", "");
        count++;
    }
    return count;
}


