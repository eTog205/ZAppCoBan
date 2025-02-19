//dv_csdl.cpp
#include "csdl.h"
#include "dv_csdl.h"
#include "log_nhalam.h"

void LogicXuLy::nap_du_lieu(giaodien& gd)
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

