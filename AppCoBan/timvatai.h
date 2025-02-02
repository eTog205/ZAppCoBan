#pragma once
//#include <boost/process.hpp>

//#include <nlohmann/json.hpp>
//#include <string>
//using json = nlohmann::json;

// Cấu trúc lưu thông tin phần mềm
//struct PhanMem
//{
//    std::string id;
//    std::string trangthai;
//    std::string name;
//    std::string version;
//    std::string category;
//    PhanMem* danhSachPhanMem = nullptr;
//    int soLuongPhanMem = 0;
//};

//namespace bp = boost::process;

// Hàm thực thi lệnh hệ thống và lấy kết quả
//std::string thucthi_lenh(const std::string& command);
//std::string loc_toi_n(const std::string& input);
//std::string laydauvao(const std::string& tukhoa);

//bool napDuLieuTuJSON(const std::string& filePath);
//void giaiPhongBoNho();


//dv_csdl.h
#include "giaodien.h"

// Lớp trung gian xử lý logic
class LogicXuLy
{
public:
    static void nap_du_lieu(giaodien& gd);
};