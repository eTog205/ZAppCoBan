#pragma once
#include <filesystem>
#include <string>

using namespace std;
namespace fs = filesystem;

#define td_log(loai, thongdiep) thongdiep_log(loai, fs::path(__FILE__).filename().string(), thongdiep)

// Kiểu enum lưu trữ loại log
enum class loai_log :uint8_t
{
	thong_bao,
	canh_bao,
	loi
};

struct log_nhalam
{
	fs::path thumuc_log;     // Thư mục chứa log
	fs::path teplog_hientai; // Đường dẫn tệp log hiện tại ("hientai.txt")
	string thoigian_dau_log; // Thời gian tạo log (dòng đầu của tệp)

	explicit log_nhalam(const fs::path& dir = "log")
		: thumuc_log(dir), teplog_hientai(dir / "hientai.txt")
	{
	}
};

string lay_thoigian();

bool dambao_thumuc_log_tontai(const fs::path& thumuc_log, bool& newly_created);
void xuly_teplog_hientai(const fs::path& thumuc_log, const fs::path& teplog_hientai);
bool taotep_logmoi(const fs::path& thumuc_log, const fs::path& teplog_hientai, string& thoigian_dau_log);
void thongdiep_log(loai_log loai, const string& ten_tep, const string& thongdiep);
bool khoidong_log();
