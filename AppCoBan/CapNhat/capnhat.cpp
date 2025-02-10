//capnhat.cpp
#include "../log_nhalam.h"
#include "capnhat.h"

#include <fstream>
#include <iomanip>
#include <sstream>

#include "get.h"

using namespace chrono;

string doctep_phienban1(const string& tentep)
{
	ifstream tep(tentep);
	string dong;

	if (!tep.is_open())
		return dong;

	getline(tep, dong);


	tep.close();
	return dong;
}

void ghitep_phienban(const string& tentep, const string& dl_canghi)
{
	ofstream tep(tentep);

	if (!tep.is_open())
	{
		td_log(loai_log::canh_bao, "mở tệp thất bại.");
		return;
	}

	tep << dl_canghi;
	tep.close();
}

int loc_dl(const string& dauvao)
{
	string kq;
	for (const char ch : dauvao)
	{
		if (isdigit(ch))
			kq.push_back(ch);
	}

	if (kq.empty())
		return 0;

	try
	{
		return stoi(kq);
	} catch (const invalid_argument& e)
	{
		td_log(loai_log::loi, "không phải là số hợp lệ: " + string(e.what()));
	} catch (const out_of_range& e)
	{
		td_log(loai_log::loi, "vượt quá giới hạn kiểu int" + string(e.what()));
	}

	return 0;
}

string dinhdang_dl(const int so_phienban)
{
	ostringstream oss;
	oss << setw(3) << setfill('0') << so_phienban;

	const string chuoi_sau_kiemtra = oss.str();
	string chuoi_ketqua = string(1, chuoi_sau_kiemtra[0]) + "." + string(1, chuoi_sau_kiemtra[1]) + "." + string(1, chuoi_sau_kiemtra[2]);

	return chuoi_ketqua;
}

// Hàm so sánh hai số và in ra kết quả so sánh
bool sosanh_phienban(const int so_hientai, const int so_layve)
{
	return so_hientai < so_layve;
}

void kiemtra_va_capnhat()
{
	const std::string latest_tag = get_release_tag();
	if (latest_tag.empty())
	{
		td_log(loai_log::loi, "Không thể lấy phiên bản mới nhất!");
		return;
	}

	// Đọc phiên bản hiện tại từ "phienban.txt"
	const std::string dl_doc = doctep_phienban1("phienban.txt");
	const int so_hientai_daloc = loc_dl(dl_doc);
	const int so_layve_daloc = loc_dl(latest_tag);

	if (sosanh_phienban(so_hientai_daloc, so_layve_daloc))
	{
		td_log(loai_log::thong_bao, "Có bản cập nhật mới: " + latest_tag);

		if (download_latest_release())
		{
			const std::string dl_doi_dinhdang = dinhdang_dl(so_layve_daloc);
			ghitep_phienban("phienban.txt", dl_doi_dinhdang);
			td_log(loai_log::thong_bao, "Cập nhật hoàn tất phiên bản " + dl_doi_dinhdang);
		} else
			td_log(loai_log::loi, "Không thể tải xuống bản cập nhật!");
	} else
		td_log(loai_log::thong_bao, "Phiên bản hiện tại (" + dinhdang_dl(so_hientai_daloc) + ") đã mới nhất.");
}

