//logic_giaodien.cpp
#include "logic_giaodien.h"

#include "chucnang_coban.h"
#include "log_nhalam.h"

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

