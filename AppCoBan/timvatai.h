#pragma once
#include <array>
//#include <boost/process.hpp>
#include <nlohmann/json.hpp>
#include <string>


constexpr auto MAX_SOFTWARE = 100;
constexpr auto MAX_CATEGORIES = 10;

struct CauHinh
{
    std::string ten_cot;
    bool hienthi_cot;

    std::string ten_nut;
    bool hienthi_nut;
};

// Mảng tĩnh lưu tối đa 10 cột
constexpr int max_columns = 10;
inline CauHinh columns[max_columns];
inline int columnCount = 0; // Số lượng cột thực tế

// Cấu trúc lưu thông tin phần mềm
struct PhanMem
{
    std::string id;
    std::string ten;
    std::string phienban_caidat;
    std::string phienban_moinhat;
    std::string phanloai;
};

using json = nlohmann::json;
//namespace bp = boost::process;

// Hàm thực thi lệnh hệ thống và lấy kết quả
//std::string thucthi_lenh(const std::string& command);
//std::string loc_toi_n(const std::string& input);
//std::string laydauvao(const std::string& tukhoa);


bool tai_tep_json(const std::string& tentep, json& dl_json);
int tai_ds_pm(const std::string& tentep, PhanMem ds_PhanMem[], int kt_toida);
int tai_ch_pm(const std::string& tentep);