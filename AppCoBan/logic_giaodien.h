//logic_giaodien.h
#pragma once
#include <string>

struct logic_giaodien
{
	int soluong_cot = 0;
	int succhua = 3; // Sức chứa thực tế của mảng
	std::string* ten_cot;

	// Constructor
	logic_giaodien()
	{
		ten_cot = new std::string[succhua];
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
};

void them_cot(logic_giaodien& lg_gd, const std::string& tenmoi);

// Hàm template cho phép truyền nhiều cột cùng lúc
template <typename... Args>
void them_cot(logic_giaodien& lg_gd, const std::string& tenmoi, Args... args)
{
	them_cot(lg_gd, tenmoi);
	them_cot(lg_gd, args...); // Đệ quy gọi lại để thêm nhiều cột
}

void khoitao_logic_giaodien(logic_giaodien& lg_gd);

std::string wstring_to_string(const std::wstring& wstr);
std::wstring string_to_wstring(const std::string& str);

float tt_thugonkichthuoc(bool& da_thugon,
						 bool& yeucau_thugon,
						 const std::chrono::steady_clock::time_point& thoigian_batdau_thugon,
						 float kichthuoc_morong,
						 float kichthuoc_thugon,
						 float thoigian_tre);

std::string tt_vanbancothenhinthay(const std::wstring& toanbo_vanban,
								   float chieurong_hientai,
								   float chieurong_toithieu,
								   float chieurong_toida);

