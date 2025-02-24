//logic_giaodien.h
#pragma once

struct ColumnConfig
{
	std::string id;        // Id duy nhất của cột (dùng để thay đổi thứ tự, truy xuất,…)
	std::string tieude;     // Tiêu đề hiển thị trên header
	float chieurong_hientai;           // Chiều rộng hiện tại (nếu không cho phép resize thì chiều rộng cố định)
	float chieurong_toithieu;       // Chiều rộng tối thiểu
	float chieurong_toida;       // Chiều rộng tối đa
	bool thaydoi_kt;        // Có cho phép thay đổi kích thước hay không
	bool hienthi;          // Có hiển thị cột hay không

	ColumnConfig(std::string id, std::string title, const float chieurong = 100.0f, const float min_width = 40.0f, const float max_width = 1000.0f, const bool resizable = true, const bool visible = true)
		: id(std::move(id)), tieude(std::move(title)), chieurong_hientai(chieurong), chieurong_toithieu(min_width), chieurong_toida(max_width), thaydoi_kt(resizable), hienthi(visible)
	{
	}
};

struct TableConfig
{
	std::vector<ColumnConfig> columns;

	void add_column(const ColumnConfig& col)
	{
		columns.push_back(col);
	}

	void reorder_columns(const std::vector<std::string>& new_order)
	{
		std::vector<ColumnConfig> new_cols;
		for (const auto& id : new_order)
		{
			for (const auto& col : columns)
			{
				if (col.id == id)
				{
					new_cols.push_back(col);
					break;
				}
			}
		}

		for (const auto& col : columns)
		{
			bool found = false;
			for (const auto& id : new_order)
			{
				if (col.id == id)
				{
					found = true; break;
				}
			}
			if (!found)
			{
				new_cols.push_back(col);
			}
		}
		columns = std::move(new_cols);
	}
};

struct logic_giaodien
{
	TableConfig ch_b;
	std::vector<std::vector<std::string>> data;
	std::unordered_map<std::string, bool> selected_map;

	int dem_hang;

	void khoidong_bang_dl();

	static void chaylenh_winget(const std::string& lenh_id);
	static void chaylenh_tienich();
};

extern logic_giaodien lg_gd;

std::string wstring_to_string(const std::wstring& wch);
std::wstring string_to_wstring(const std::string& ch);

float tt_thugonkichthuoc(bool& da_thugon, bool& yeucau_thugon, const std::chrono::steady_clock::time_point& thoigian_batdau_thugon, float kichthuoc_morong, float kichthuoc_thugon, float thoigian_tre);

std::string tt_vanbancothenhinthay(const std::wstring& toanbo_vanban, float chieurong_hientai, float chieurong_toithieu, float chieurong_toida);


