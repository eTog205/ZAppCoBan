//logic_giaodien.cpp
#include "chay_luongphu.h"
#include "chucnang_cotloi.h"
#include "log_nhalam.h"
#include "logic_giaodien.h"

logic_giaodien lg_gd;

void logic_giaodien::khoidong_bang_dl()
{
	ch_b.columns.clear();
	ch_b.add_column(ColumnConfig("chon", "Chọn", 40.0f, 40.0f, 40.0f, false, true));
	ch_b.add_column(ColumnConfig("id", "ID", 80.0f, 50.0f, 200.0f, true, true));
	ch_b.add_column(ColumnConfig("ten", "Tên", 150.0f, 80.0f, 300.0f, true, true));
	ch_b.add_column(ColumnConfig("phanloai", "Phân loại", 120.0f, 60.0f, 250.0f, true, true));
}

std::string wstring_to_string(const std::wstring& wch)
{
	if (wch.empty()) return "";

	const int size_needed = WideCharToMultiByte(CP_UTF8, 0, wch.c_str(), static_cast<int>(wch.size()), nullptr, 0, nullptr, nullptr);
	std::string str(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, wch.c_str(), static_cast<int>(wch.size()), str.data(), size_needed, nullptr, nullptr);

	return str;
}

std::wstring string_to_wstring(const std::string& ch)
{
	if (ch.empty()) return L"";

	const int size_needed = MultiByteToWideChar(CP_UTF8, 0, ch.c_str(), static_cast<int>(ch.size()), nullptr, 0);
	std::wstring wstr(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, ch.c_str(), static_cast<int>(ch.size()), &wstr[0], size_needed);

	return wstr;
}

float tt_thugonkichthuoc(bool& da_thugon, bool& yeucau_thugon, const std::chrono::steady_clock::time_point& thoigian_batdau_thugon, const float kichthuoc_morong, const float kichthuoc_thugon, const float thoigian_tre)
{
	float new_size = da_thugon ? kichthuoc_thugon : kichthuoc_morong;
	if (yeucau_thugon)
	{
		const auto now = std::chrono::steady_clock::now();
		const float elapsed = std::chrono::duration<float>(now - thoigian_batdau_thugon).count();

		const float t = std::clamp(elapsed / thoigian_tre, 0.0f, 1.0f);

		const float start_size = da_thugon ? kichthuoc_thugon : kichthuoc_morong;
		const float target_size = da_thugon ? kichthuoc_morong : kichthuoc_thugon;
		new_size = start_size + t * (target_size - start_size);

		if (t >= 1.0f)
		{
			da_thugon = !da_thugon;
			yeucau_thugon = false;
		}
	}

	return new_size;
}

std::string tt_vanbancothenhinthay(const std::wstring& toanbo_vanban, const float chieurong_hientai, const float chieurong_toithieu, const float chieurong_toida)
{
	const float ratio = std::clamp((chieurong_hientai - chieurong_toithieu) / (chieurong_toida - chieurong_toithieu), 0.0f, 1.0f);
	const int max_chars = std::clamp(static_cast<int>(ratio * static_cast<float>(toanbo_vanban.size())), 1, static_cast<int>(toanbo_vanban.size()));
	return wstring_to_string(toanbo_vanban.substr(0, max_chars));
}

void logic_giaodien::chaylenh_winget(const std::string& lenh_id)
{
	lp_chay_lenhwinget(lenh_id);
}

void logic_giaodien::chaylenh_tienich()
{
	lp_chay_lenhwindow();
}


