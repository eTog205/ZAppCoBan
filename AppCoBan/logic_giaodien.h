//logic_giaodien.h
#pragma once
#include <unordered_set>

struct logic_giaodien
{
	int soluong_cot = 0;
	int succhua = 3; // Sức chứa thực tế của mảng
	std::string* ten_cot;
	std::vector<int> thutu_cot;

	// Constructor
	logic_giaodien()
	{
		ten_cot = new std::string[succhua];
		thutu_cot.push_back(0);
	}

	// Không cho phép copy
	logic_giaodien(const logic_giaodien&) = delete;
	logic_giaodien& operator=(const logic_giaodien&) = delete;

	// Vẫn cho phép move
	logic_giaodien(logic_giaodien&& other) noexcept
	{
		soluong_cot = other.soluong_cot;
		succhua = other.succhua;
		ten_cot = other.ten_cot;

		other.ten_cot = nullptr;
		other.soluong_cot = 0;
		other.succhua = 0;
	}

	logic_giaodien& operator=(logic_giaodien&& other) noexcept
	{
		if (this != &other)
		{
			delete[] ten_cot;

			soluong_cot = other.soluong_cot;
			succhua = other.succhua;
			ten_cot = other.ten_cot;
			thutu_cot = std::move(other.thutu_cot);

			other.ten_cot = nullptr;
			other.soluong_cot = 0;
			other.succhua = 0;
		}
		return *this;
	}

	~logic_giaodien()
	{
		delete[] ten_cot;
	}

	static void chaylenh_winget(const std::string& lenh_id);
	static void chaylenh_tienich();
};

void them_cot(logic_giaodien& lg_gd, const std::string& tenmoi);

// Hàm template cho phép truyền nhiều cột cùng lúc
template <typename... Args>
void them_cot(logic_giaodien& lg_gd, const std::string& tenmoi, Args... args)
{
	them_cot(lg_gd, tenmoi);
	them_cot(lg_gd, args...); // Đệ quy gọi lại để thêm nhiều cột
}

template<typename... Args>
void thaydoi_thutu_cot(logic_giaodien& lg_gd, Args... args)
{
	// Tạo vector từ danh sách các tham số truyền vào
	std::vector<int> new_order_provided = { args... };
	// Tổng số cột: 1 (checkbox) + số cột được thêm
	int total_columns = lg_gd.soluong_cot + 1;
	std::vector<int> new_order;
	std::unordered_set<int> used;

	// Thêm các chỉ số do người dùng nhập, nếu hợp lệ và không trùng
	for (int idx : new_order_provided)
	{
		if (idx >= 0 && idx < total_columns && !used.contains(idx))
		{
			new_order.push_back(idx);
			used.insert(idx);
		}
	}
	// Nối thêm các chỉ số còn thiếu theo thứ tự mặc định (0, 1, 2, …)
	for (int idx = 0; idx < total_columns; ++idx)
	{
		if (!used.contains(idx))
		{
			new_order.push_back(idx);
		}
	}
	// Cập nhật lại thứ tự cột
	lg_gd.thutu_cot = new_order;
}

void khoitao_logic_giaodien(logic_giaodien& lg_gd);

std::string wstring_to_string(const std::wstring& wch);
std::wstring string_to_wstring(const std::string& ch);

float tt_thugonkichthuoc(bool& da_thugon, bool& yeucau_thugon, const std::chrono::steady_clock::time_point& thoigian_batdau_thugon, float kichthuoc_morong, float kichthuoc_thugon, float thoigian_tre);

std::string tt_vanbancothenhinthay(const std::wstring& toanbo_vanban, float chieurong_hientai, float chieurong_toithieu, float chieurong_toida);

void macdinh(logic_giaodien& lg_gd);
