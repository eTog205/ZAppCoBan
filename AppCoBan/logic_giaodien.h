//logic_giaodien.h
#pragma once
#include <xstring>

struct logic_giaodien
{
	int soluong_cot = 0; // Số lượng cột hiện tại
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

	// Destructor để giải phóng bộ nhớ
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

