//dv_csdl.h
#pragma once
#include "giaodien.h"
//#include <boost/process.hpp>

//namespace bp = boost::process;

// Hàm thực thi lệnh hệ thống và lấy kết quả
//std::string thucthi_lenh(const std::string& command);
//std::string loc_toi_n(const std::string& input);
//std::string laydauvao(const std::string& tukhoa);

// Lớp trung gian xử lý logic
class LogicXuLy
{
public:
    static void nap_du_lieu(giaodien& gd);
};


