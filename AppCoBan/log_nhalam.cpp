//log_nhalam.cpp
#include "log_nhalam.h"

#include <fstream>

using namespace std::chrono;

std::string lay_thoigian()
{
	// Lấy thời gian hiện tại từ hệ thống
	const auto now = system_clock::now();

	// Chuyển đổi thời gian sang thời gian cục bộ dựa theo múi giờ hiện tại (thread-safe)
	const zoned_time local_time{ current_zone(), now };

	// Làm tròn thời gian cục bộ xuống giây (loại bỏ phần thập phân)
	auto local_time_sec = zoned_time{
		local_time.get_time_zone(),
		floor<seconds>(local_time.get_local_time())
	};

	//	Định dạng thời gian theo thứ tự: giây-phút-giờ ngày-tháng-năm
	//	%S: giây, %M: phút, %H: giờ, %d: ngày, %m: tháng, %Y: năm
	//	%F : tương đương với %Y-%m-%d (năm-tháng-ngày)
	//	%T : tương đương với %H:%M:%S (giờ:phút:giây)
	return format("{:%Hgiờ-%Mphút-%Sgiây %d-%m-%Y}", local_time_sec);
}

// Hàm đảm bảo rằng thư mục log tồn tại. Nếu chưa có thì tạo mới.
// Tham số newlyCreated sẽ được đặt là true nếu thư mục được tạo mới.
bool dambao_thumuc_log_tontai(const fs::path& thumuc_log, bool& taomoi)
{
	taomoi = false;
	if (!exists(thumuc_log))
	{
		if (create_directory(thumuc_log))
			taomoi = true;
		else
			return false;
	}
	return true;
}

// Hàm xử lý tệp log hiện tại ("hientai.txt") trong thư mục log.
// Nếu tệp tồn tại, đọc dòng đầu tiên (thời gian tạo log cũ) và đổi tên tệp thành
// "<thời gian tạo>.txt".
void xuly_teplog_hientai(const fs::path& thumuc_log, const fs::path& teplog_hientai)
{
	if (exists(teplog_hientai))
	{
		if (std::ifstream in_file(teplog_hientai); in_file.is_open())
		{
			if (std::string dongdau; getline(in_file, dongdau) && !dongdau.empty())
			{
				in_file.close();
				const fs::path new_file_path = thumuc_log / (dongdau + ".txt");
				fs::rename(teplog_hientai, new_file_path);
			} else
				in_file.close();
		}
	}
}

// Hàm tạo tệp log mới ("hientai.txt") và ghi dòng đầu tiên chứa thời gian tạo log.
bool taotep_logmoi(const fs::path& thumuc_log, const fs::path& teplog_hientai, std::string& thoigian_dau_log)
{
	thoigian_dau_log = lay_thoigian();
	std::ofstream out_file(teplog_hientai, std::ios::out);
	if (!out_file)
	{
		return false;
	}

	out_file << thoigian_dau_log << "\n";
	out_file.close();

	return true;
}

// Hàm này cần được gọi khi khởi động chương trình để đảm bảo môi trường log sẵn sàng.
bool khoidong_log()
{
	log_nhalam lg;
	bool created = false;
	if (!dambao_thumuc_log_tontai(lg.thumuc_log, created))
		return false;
	if (!created)
	{  // Nếu thư mục đã tồn tại, xử lý tệp log hiện tại.
		xuly_teplog_hientai(lg.thumuc_log, lg.teplog_hientai);
	}

	return taotep_logmoi(lg.thumuc_log, lg.teplog_hientai, lg.thoigian_dau_log);
}

// Hàm ghi một thông điệp log vào tệp log hiện tại. 
// Các loại log: "THÔNG BÁO", "CẢNH BÁO", "LỖI".
void thongdiep_log(const loai_log loai, const std::string& ten_tep, const std::string& thongdiep)
{
	const log_nhalam lg;
	std::ofstream out_file(lg.teplog_hientai, std::ios::app);
	if (!out_file)
	{
		//cerr << "Lỗi: Không thể mở tệp log để ghi thông điệp.\n";
		return;
	}

	const std::string time_str = lay_thoigian();
	std::string type_str;
	switch (loai)
	{
		case loai_log::thong_bao:
			type_str = "✅THÔNG BÁO";
			break;
		case loai_log::canh_bao:
			type_str = "CẢNH BÁO";
			break;
		case loai_log::loi:
			type_str = "❌LỖI";
			break;
	}

	out_file << time_str << " [" << type_str << "] (" << ten_tep << ") " << thongdiep << "\n";
	out_file.close();
}


