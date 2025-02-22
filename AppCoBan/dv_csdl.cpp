//dv_csdl.cpp
#include "csdl.h"
#include "dv_csdl.h"
#include "giaodien.h"
#include "log_nhalam.h"


void nap_du_lieu()
{
	int row_count = 0;
	if (get_row_count("Items", &row_count) == SQLITE_OK && row_count > 0)
	{
		gd.row_count = row_count;
	}

	gd.data.clear();

	const std::string sql = "SELECT ID, Name, Category FROM Items;";
	sqlite3_stmt* stmt;

	if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			std::vector<std::string> row;
			row.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))); // ID
			row.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))); // Tên
			row.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))); // Phân loại
			gd.data.push_back(row);
		}
	} else
	{
		td_log(loai_log::loi, "lấy dữ liệu từ Items!");
	}

	sqlite3_finalize(stmt);
}

void ve_giaodien(const int chieurong_manhinh, const int chieucao_manhinh)
{
	giaodien_thanhcongcu(chieurong_manhinh, chieucao_manhinh);
	giaodien_menuben(chieucao_manhinh);
	giaodien_tienich(chieurong_manhinh, chieucao_manhinh);
	giaodien_caidat(chieurong_manhinh, chieucao_manhinh);
	giaodien_bangdl(chieurong_manhinh, chieucao_manhinh);

	giaodien_demo();
}


