//logic_giaodien.cpp
#include "chucnang_coban.h"
#include "log_nhalam.h"
#include "logic_giaodien.h"

void them_cot(logic_giaodien& lg_gd, const std::string& tenmoi)
{
	if (lg_gd.soluong_cot < lg_gd.succhua)
	{
		// Còn chỗ, chỉ cần thêm vào
		lg_gd.ten_cot[lg_gd.soluong_cot++] = tenmoi;
	} else
	{
		// Hết chỗ, cấp phát lại với kích thước lớn hơn
		const int succhua_moi = lg_gd.succhua * 2;
		const auto temp = new std::string[succhua_moi];

		// Sao chép dữ liệu cũ sang mảng mới
		for (int i = 0; i < lg_gd.soluong_cot; i++)
		{
			temp[i] = lg_gd.ten_cot[i];
		}

		// Giải phóng mảng cũ
		delete[] lg_gd.ten_cot;

		// Cập nhật con trỏ và sức chứa mới
		lg_gd.ten_cot = temp;
		lg_gd.succhua = succhua_moi;

		// Thêm cột mới
		lg_gd.ten_cot[lg_gd.soluong_cot++] = tenmoi;
	}
}

void khoitao_logic_giaodien(logic_giaodien& lg_gd)
{
	them_cot(lg_gd, "id", "Tên", "Phân loại");
}

std::string wstring_to_string(const std::wstring& wstr)
{
	if (wstr.empty()) return "";

	const int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), str.data(), size_needed, nullptr, nullptr);

	return str;
}

std::wstring string_to_wstring(const std::string& str)
{
	if (str.empty()) return L"";

	const int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstr[0], size_needed);

	return wstr;
}