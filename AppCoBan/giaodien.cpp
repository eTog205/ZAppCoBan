//giaodien.cpp
#include "chucnang_cotloi.h"
#include "giaodien.h"

#include "logic_giaodien.h"

giaodien gd;

// Hàm tính toán vị trí, kích thước, tên cửa sổ và cờ cho một cửa sổ con
thongtin_cuaso_imgui tinh_thongtin_cuaso(const int chieurong_manhinh, const int chieucao_manhinh)
{
	thongtin_cuaso_imgui tt;

	// Tính toạ độ
	tt.vitri.x = gd.chieurong_menuben + gd.letrai_menuben * 2;
	tt.vitri.y = gd.letren_bang;

	// Tính kích thước
	tt.kichthuoc.x = static_cast<float>(chieurong_manhinh) - gd.letrai_menuben * 3 - gd.chieurong_menuben;
	tt.kichthuoc.y = static_cast<float>(chieucao_manhinh) - gd.letren_bang;

	return tt;
}

// Hàm hỗ trợ để tự động sửa đổi màu
ImVec4 adjust_color_brightness(const ImVec4& color, const float factor)
{
	return {
		std::min(color.x * factor, 1.0f),
		std::min(color.y * factor, 1.0f),
		std::min(color.z * factor, 1.0f),
		color.w // Giữ nguyên độ trong suốt (alpha)
	};
}

void combo_box(const char* label, const char* options[], const int options_count, int& current_selection, const float gt_botron)
{
	// Nếu chưa chọn gì (currentSelection == 0), hiển thị chuỗi rỗng ""
	const char* preview = (current_selection == 0) ? "" : options[current_selection];

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, gt_botron);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Giữ nguyên màu nền
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Không đổi màu khi di chuột vào
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Đen nhưng hoàn toàn trong suốt
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Đặt màu nền danh sách hạ xuống

	if (ImGui::BeginCombo(label, preview, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 1; i < options_count; ++i) // Bỏ qua option đầu tiên (rỗng)
		{
			if (i != current_selection && ImGui::Selectable(options[i])) // Bỏ qua cái đã chọn
				current_selection = i;
		}
		ImGui::EndCombo();
	}

	ImGui::PopStyleColor(7);
	ImGui::PopStyleVar();

	ImGui::SameLine();
	const std::string btn_label = std::string("X##") + label;

	if (ImGui::Button(btn_label.c_str()))
		current_selection = 0; // Reset về trạng thái không hiển thị gì
}

void capnhat_bang_phanmem()
{
	std::vector<ColumnConfig> visible_columns;
	for (const auto& col : lg_gd.ch_b.columns)
	{
		if (col.hienthi)
			visible_columns.push_back(col);
	}

	const int tong_cot = static_cast<int>(visible_columns.size());
	if (tong_cot == 0)
		return;

	if (ImGui::BeginTable("BangPhanMem", tong_cot, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Sortable))
	{
		for (const auto& cot : visible_columns)
		{
			int co = 0;
			if (!cot.thaydoi_kt)
			{
				co |= ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize;
			}
			ImGui::TableSetupColumn(cot.tieude.c_str(), co, cot.chieurong_hientai);
		}
		ImGui::TableHeadersRow();

		for (size_t hang = 0; hang < lg_gd.data.size(); ++hang)
		{
			ImGui::TableNextRow();
			for (int vitri_cot = 0; vitri_cot < tong_cot; vitri_cot++)
			{
				ImGui::TableSetColumnIndex(vitri_cot);
				const auto& cot = visible_columns[vitri_cot];
				if (cot.id == "chon")
				{
					std::string id = lg_gd.data[hang][0];
					ImGui::Checkbox(("##check" + std::to_string(hang)).c_str(), &lg_gd.selected_map[id]);
				} else
				{
					int vitri_dulieu = 0;
					if (cot.id == "id")
						vitri_dulieu = 0;
					else if (cot.id == "ten")
						vitri_dulieu = 1;
					else if (cot.id == "phanloai")
						vitri_dulieu = 2;
					if (vitri_dulieu < static_cast<int>(lg_gd.data[hang].size()))
					{
						ImGui::Text("%s", lg_gd.data[hang][vitri_dulieu].c_str());
					}
				}
			}
		}
		ImGui::EndTable();
	}
}

void giaodien_thanhcongcu(const int chieurong_manhinh, const int chieucao_manhinh)
{
	const float chieurong_hientai = static_cast<float>(chieurong_manhinh) - gd.letrai_menuben * 2;
	const float chieucao_hientai = static_cast<float>(chieucao_manhinh) - gd.chieucao_menuben - gd.letrai_menuben * 2;

	ImGui::SetNextWindowPos(ImVec2(gd.letrai_menuben, gd.letrai_menuben));
	ImGui::SetNextWindowSize(ImVec2(chieurong_hientai, chieucao_hientai));

	ImGui::Begin("thanh công cụ", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
	if (ImGui::Button("nút cài đặt"))
	{
		for (auto& item : lg_gd.selected_map)
		{
			if (item.second)
				logic_giaodien::chaylenh_winget(item.first);
		}
	}

	ImGui::End();
}

void giaodien_menuben(const int chieucao_manhinh)
{
	gd.chieucao_menuben = static_cast<float>(chieucao_manhinh) - gd.letren_menuben;

	// Xử lý thu gọn menu
	const float chieurong_hientai = tt_thugonkichthuoc(gd.menuben_thugon, gd.yeucau_thugon, gd.batdau_thugon,
													   gd.chieurong_menuben_morong, gd.chieurong_menuben_thugon,
													   gd.thoigian_thugon);

	gd.chieurong_menuben = chieurong_hientai;

	ImGui::SetNextWindowPos(ImVec2(gd.letrai_menuben, gd.letren_menuben));
	ImGui::SetNextWindowSize(ImVec2(chieurong_hientai, gd.chieucao_menuben));
	ImGui::Begin("Menu bên", nullptr,
				 ImGuiWindowFlags_NoMove |
				 ImGuiWindowFlags_NoResize |
				 ImGuiWindowFlags_NoTitleBar |
				 ImGuiWindowFlags_NoScrollbar |
				 ImGuiWindowFlags_NoScrollWithMouse);

	auto mau_nut = ImVec4(0, 0, 0, 0);
	ImVec4 mau_nut_hover = (mau_nut.w == 0.0f) ? ImVec4(0.3f, 0.3f, 0.3f, 0.2f) : adjust_color_brightness(mau_nut, 0.8f);
	ImVec4 mau_nut_active = (mau_nut.w == 0.0f) ? ImVec4(0.2f, 0.2f, 0.2f, 0.4f) : adjust_color_brightness(mau_nut, 1.2f);

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, mau_nut);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, mau_nut_hover);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, mau_nut_active);

	constexpr float chieucao_nut = 30.0f;
	const ImVec2 kichthuoc_nut(chieurong_hientai, chieucao_nut);

	const std::vector<MenuItem> menu_items = {
		{L"Bảng dữ liệu", "bangdl"},
		{L"Tiện ích", "tienich"},
		{L"Cài đặt", "caidat"},
	};

	for (const auto& [full_text, id] : menu_items)
	{
		ImVec2 vitri_nut = ImGui::GetCursorScreenPos();
		vitri_nut.x = gd.letrai_menuben;
		ImGui::SetCursorScreenPos(vitri_nut);

		// Tính toán tỷ lệ để thu gọn chữ
		std::string vanban_nhinthay = tt_vanbancothenhinthay(full_text,
															 chieurong_hientai,
															 gd.chieurong_menuben_thugon,
															 gd.chieurong_menuben_morong);

		ImVec2 vitri_vanban = ImGui::GetCursorScreenPos();
		vitri_vanban.x += 10.0f;
		vitri_vanban.y += 10.0f;

		if (ImGui::Button(("###" + id).c_str(), kichthuoc_nut))
		{
			if (id == "bangdl") ImGui::SetWindowFocus("bangdl");
			if (id == "tienich") ImGui::SetWindowFocus("tienich");
			if (id == "caidat") ImGui::SetWindowFocus("caidat");
		}

		ImGui::GetWindowDrawList()->AddText(vitri_vanban, ImGui::GetColorU32(ImGuiCol_Text), vanban_nhinthay.c_str());
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	ImVec2 vitri_nut;
	vitri_nut.x = gd.letrai_menuben;
	vitri_nut.y = gd.chieucao_menuben + chieucao_nut * 3;
	ImGui::SetCursorScreenPos(vitri_nut);

	constexpr auto button_thugon_color = ImVec4(0.6f, 0.6f, 0.6f, 0.2f);
	const ImVec4 button_thugon_hover_color = adjust_color_brightness(button_thugon_color, 0.8f);
	const ImVec4 button_thugon_active_color = adjust_color_brightness(button_thugon_color, 1.2f);

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, button_thugon_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_thugon_hover_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_thugon_active_color);

	if (ImGui::Button(gd.menuben_thugon ? ">" : "<", kichthuoc_nut))
	{
		gd.yeucau_thugon = true;
		gd.batdau_thugon = std::chrono::steady_clock::now();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	ImGui::End();
}

void giaodien_bangdl(const int chieurong_manhinh, const int chieucao_manhinh)
{
	thongtin_cuaso_imgui tt = tinh_thongtin_cuaso(chieurong_manhinh, chieucao_manhinh);

	ImGui::SetNextWindowPos(ImVec2(tt.vitri));
	ImGui::SetNextWindowSize(ImVec2(tt.kichthuoc));
	ImGui::Begin("bangdl", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	lg_gd.khoidong_bang_dl();
	capnhat_bang_phanmem();
	ImGui::End();
}

void giaodien_tienich(const int chieurong_manhinh, const int chieucao_manhinh)
{
	thongtin_cuaso_imgui tt = tinh_thongtin_cuaso(chieurong_manhinh, chieucao_manhinh);

	ImGui::SetNextWindowPos(ImVec2(tt.vitri));
	ImGui::SetNextWindowSize(ImVec2(tt.kichthuoc));
	ImGui::Begin("tienich", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	if (ImGui::Button("Sửa Window"))
		logic_giaodien::chaylenh_tienich();

	ImGui::End();
}

void giaodien_caidat(const int chieurong_manhinh, const int chieucao_manhinh)
{
	const thongtin_cuaso_imgui tt = tinh_thongtin_cuaso(chieurong_manhinh, chieucao_manhinh);

	ImGui::SetNextWindowPos(tt.vitri);
	ImGui::SetNextWindowSize(tt.kichthuoc);
	ImGui::Begin("caidat", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGui::Text("Tính năng đang được phát triển. Vui lòng đợi");

	ImGui::End();
}

void giaodien_demo()
{
	// dưới là cửa sổ demo sẵn của imgui
	ImGui::ShowDemoWindow();

	// nếu muốn tạo cửa sổ thì phải ImGui::Begin() và phải luôn có ImGui::End() ở cuối
	// ImGui::Text là dòng văn bản sẽ xuất hiện
	// ImGui::Button là nút có thể nhấn
	//nếu muốn chỉnh màu có thể xem hàm combo_box
	//bắt buộc đặt tên vì đó được xem là id duy nhất.
	ImGui::Begin("Mẫu");

	//thêm # sẽ ẩn chữ
	ImGui::Button("#nút");
	ImGui::Text("");

	ImGui::End();
}


