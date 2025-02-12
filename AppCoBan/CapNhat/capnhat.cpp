//capnhat.cpp
#include "../log_nhalam.h"
#include "capnhat.h"
#include "get.h"
#include "giainen.h"

#include <boost/dll.hpp>
#include <boost/process.hpp>
#include <fstream>

using namespace std::chrono;
namespace bfs = boost::filesystem;

std::string doctep_phienban(const std::string& tentep)
{
	std::ifstream tep(tentep);
	std::string dong;

	if (!tep.is_open())
	{
		td_log(loai_log::canh_bao, "Không thể mở tệp để đọc " + std::string(tentep));
		return dong;
	}

	getline(tep, dong);

	tep.close();
	return dong;
}

void ghitep_phienban(const std::string& tentep, const std::string& dl_canghi)
{
	std::ofstream tep(tentep);

	if (!tep.is_open())
	{
		td_log(loai_log::canh_bao, "mở tệp để ghi thất bại." + std::string(tentep));
		return;
	}

	tep << dl_canghi;
	tep.close();
}

int loc_dl(const std::string& dauvao)
{
	std::string kq;
	for (const char ch : dauvao)
	{
		if (isdigit(ch))
			kq.push_back(ch);
	}

	if (kq.empty())
		return 0;

	try
	{
		return std::stoi(kq);
	} catch (const std::invalid_argument& e)
	{
		td_log(loai_log::loi, "không phải là số hợp lệ: " + std::string(e.what()));
	} catch (const std::out_of_range& e)
	{
		td_log(loai_log::loi, "vượt quá giới hạn kiểu int" + std::string(e.what()));
	}

	return 0;
}

std::string dinhdang_dl(const int so_phienban)
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << so_phienban;

	const std::string chuoi_sau_kiemtra = oss.str();
	std::string chuoi_ketqua = std::string(1, chuoi_sau_kiemtra[0]) + "." + std::string(1, chuoi_sau_kiemtra[1]) + "." + std::string(1, chuoi_sau_kiemtra[2]);

	return chuoi_ketqua;
}

bool sosanh_phienban(const int so_hientai, const int so_layve)
{
	return so_hientai < so_layve;
}

void kiemtra_capnhat()
{
	const std::string latest_tag = get_release_tag();
	if (latest_tag.empty())
	{
		td_log(loai_log::loi, "Không thể lấy phiên bản mới nhất!");
		return;
	}

	// Đọc phiên bản hiện tại từ "phienban.txt"
	const std::string dl_doc = doctep_phienban("phienban.txt");
	const int so_hientai_daloc = loc_dl(dl_doc);
	const int so_layve_daloc = loc_dl(latest_tag);

	if (sosanh_phienban(so_hientai_daloc, so_layve_daloc))
	{
		td_log(loai_log::thong_bao, "Có bản cập nhật mới: " + latest_tag);

		if (download_latest_release())
		{
			const std::string dl_doi_dinhdang = dinhdang_dl(so_layve_daloc);
			ghitep_phienban("phienban.txt", dl_doi_dinhdang);

			capnhat();
			td_log(loai_log::thong_bao, "Hoàn tất cập nhật phiên bản mới " + dl_doi_dinhdang);

			td_log(loai_log::thong_bao, "Chạy AppCoBan.exe " + dl_doi_dinhdang);
			chay_app_co_ban();
		} else
			td_log(loai_log::loi, "Không thể tải xuống bản cập nhật!");
	} else
	{
		td_log(loai_log::thong_bao, "Phiên bản hiện tại (" + dinhdang_dl(so_hientai_daloc) + ") đã mới nhất.");
		chay_app_co_ban();
	}
}

void capnhat()
{
	const std::string duongdan_winrar = get_win_rar_path();
	run_win_rar(duongdan_winrar);
}

void chay_app_co_ban()
{
	try
	{
		// 1. Lấy đường dẫn của CapNhat.exe đang chạy
		const bfs::path duongdan_capnhat = boost::dll::program_location();

		// 2. Lấy thư mục chứa CapNhat.exe
		const bfs::path thumuc_capnhat = duongdan_capnhat.parent_path();

		// 3. Lấy thư mục chính (parent của thư mục CapNhat)
		const bfs::path thumuc_cha = thumuc_capnhat.parent_path();

		// 4. Xác định đường dẫn đến AppCoBan.exe nằm trong thư mục chính
		const bfs::path duongdan_appcb = thumuc_cha / "AppCoBan.exe";

		// 5. Chạy AppCoBan.exe với thư mục làm việc là thư mục chính (nếu cần)
		boost::process::child process(
			duongdan_appcb.string(),
			boost::process::start_dir = thumuc_cha.string() // đặt working directory là thư mục chính
		);

		process.wait(); // Chờ tiến trình hoàn thành (tuỳ chọn)
	} catch (const std::exception& e)
	{
		td_log(loai_log::loi, "chạy AppCoBan.exe:" + std::string(e.what()));
	}
}