#pragma once
#include <boost/process.hpp>
#include <string>

namespace bp = boost::process;

// Hàm thực thi lệnh hệ thống và lấy kết quả
std::string thucthi_lenh(const std::string& command);

void chaylenh(const std::string& id);
