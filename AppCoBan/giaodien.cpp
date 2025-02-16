//giaodien.cpp
#include "chucnang_coban.h"
#include "csdl_thongbao.h"
#include "giaodien.h"
#include "suawindow.h"


void handle_collapse(bool& is_collapsed, bool& collapse_requested, const std::chrono::steady_clock::time_point& collapse_start_time, float& current_size, const float expanded_size, const float collapsed_size, const float delay_seconds)
{
	if (collapse_requested)
	{
		const auto now = std::chrono::steady_clock::now();
		const std::chrono::duration<float> elapsed_seconds = now - collapse_start_time;
		const float target_size = is_collapsed ? expanded_size : collapsed_size;
		const float start_size = is_collapsed ? collapsed_size : expanded_size;
		// Tỷ lệ thời gian trôi qua
		float t = elapsed_seconds.count() / delay_seconds;

		// Giới hạn t trong khoảng [0, 1]
		t = std::min(std::max(t, 0.0f), 1.0f);

		// Cập nhật kích thước
		current_size = start_size + t * (target_size - start_size);

		// Hoàn tất chuyển đổi
		if (t >= 1.0f)
		{
			is_collapsed = !is_collapsed; // Đảo trạng thái
			collapse_requested = false;  // Reset yêu cầu
		}
	}
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

	// ComboBox chính
	if (ImGui::BeginCombo(label, preview, ImGuiComboFlags_NoArrowButton))
	{
		for (int i = 1; i < options_count; ++i) // Bỏ qua option đầu tiên (rỗng)
		{
			if (i != current_selection) // Bỏ qua cái đã chọn
			{
				if (ImGui::Selectable(options[i]))
				{
					current_selection = i;
				}
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopStyleColor(7);
	ImGui::PopStyleVar();

	ImGui::SameLine();
	const std::string btn_label = std::string("X##") + label; // Tạo ID duy nhất
	if (ImGui::Button(btn_label.c_str()))
	{
		current_selection = 0; // Reset về trạng thái không hiển thị gì
	}
}

void cap_nhat_bang_phan_mem(giaodien& gd, const logic_giaodien& lg_gd)
{
	int column_count = lg_gd.soluong_cot + 1;
	if (ImGui::BeginTable("BangPhanMem", column_count, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable))
	{
		// Thiết lập cột đầu tiên cho checkbox
		ImGui::TableSetupColumn("Chọn", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoResize, 40.0f);

		// Thiết lập các cột dữ liệu theo tiêu đề có trong lg_gd.ten_cot
		for (int i = 0; i < lg_gd.soluong_cot; ++i)
		{
			ImGui::TableSetupColumn(lg_gd.ten_cot[i].c_str());
		}
		ImGui::TableHeadersRow();

		for (size_t row = 0; row < gd.data.size(); ++row)
		{
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			std::string id = gd.data[row][0];

			ImGui::Checkbox(("##check" + std::to_string(row)).c_str(), &gd.selected_map[id]);

			// Các cột dữ liệu: hiển thị nội dung của từng ô
			for (int col = 0; col < lg_gd.soluong_cot; ++col)
			{
				ImGui::TableSetColumnIndex(col + 1);
				if (col < static_cast<int>(gd.data[row].size()))
				{
					ImGui::Text("%s", gd.data[row][col].c_str());
				}
			}
		}
		ImGui::EndTable();
	}
}

void giaodien_thanhcongcu(giaodien& gd, const int chieurong_manhinh, const int chieucao_manhinh)
{
	const float chieurong_hientai = static_cast<float>(chieurong_manhinh) - gd.letrai_menuben * 2;
	const float chieucao_hientai = static_cast<float>(chieucao_manhinh) - gd.chieucao_menuben - gd.letrai_menuben * 2;

	// Tạo giao diện menu bên
	ImGui::SetNextWindowPos(ImVec2(gd.letrai_menuben, gd.letrai_menuben));
	ImGui::SetNextWindowSize(ImVec2(chieurong_hientai, chieucao_hientai));

	ImGui::Begin("thanh công cụ", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	if (ImGui::Button("nút cài đặt"))
	{
		for (const auto& item : gd.selected_map)
		{
			if (item.second)
			{
				chaylenh(item.first);
			}
		}
	}

	ImGui::End();
}

void giaodien_menuben(giaodien& gd, const int chieucao_manhinh)
{
	float chieurong_hientai = gd.chieurong_menuben;

	// Cập nhật chiều cao menu
	gd.chieucao_menuben = static_cast<float>(chieucao_manhinh) - gd.letren_menuben;

	// Xử lý thu gọn menu
	handle_collapse(
		gd.menuben_thugon,
		gd.yeucau_thugon,
		gd.batdau_thugon,
		chieurong_hientai,
		gd.chieurong_menuben_morong,
		gd.chieurong_menuben_thugon,
		gd.thoigian_thugon
	);

	// Ghi lại chiều rộng thực tế
	gd.chieurong_menuben = chieurong_hientai;

	// Tạo giao diện menu bên
	ImGui::SetNextWindowPos(ImVec2(gd.letrai_menuben, gd.letren_menuben));
	ImGui::SetNextWindowSize(ImVec2(chieurong_hientai, gd.chieucao_menuben));
	ImGui::Begin("Menu bên", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	auto button_color = ImVec4(0, 0, 0, 0);
	ImVec4 button_hover_color = (button_color.w == 0.0f) ? ImVec4(0.3f, 0.3f, 0.3f, 0.2f) : adjust_color_brightness(button_color, 0.8f);
	ImVec4 button_active_color = (button_color.w == 0.0f) ? ImVec4(0.2f, 0.2f, 0.2f, 0.4f) : adjust_color_brightness(button_color, 1.2f);

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, button_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hover_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);

	constexpr float button_height = 30.0f;
	const ImVec2 button_size(chieurong_hientai, button_height);

	const std::vector<MenuItem> menu_items = {
		{L"Bảng dữ liệu", "bangdl"},
		{L"Tiện ích", "tienich"},
		{L"Cài đặt", "caidat"},
	};

	// Tính toán tỷ lệ để thu gọn chữ
	float ratio = (chieurong_hientai - gd.chieurong_menuben_thugon) / (gd.chieurong_menuben_morong - gd.chieurong_menuben_thugon);
	ratio = std::clamp(ratio, 0.0f, 1.0f);

	for (const auto& item : menu_items)
	{
		ImVec2 button_pos = ImGui::GetCursorScreenPos();
		button_pos.x = gd.letrai_menuben;
		ImGui::SetCursorScreenPos(button_pos);

		int max_chars = static_cast<int>(ratio * static_cast<float>(item.full_text.size()));
		max_chars = std::clamp(max_chars, 1, static_cast<int>(item.full_text.size()));

		std::wstring visible_text_w = item.full_text.substr(0, max_chars);
		std::string visible_text = wstring_to_string(visible_text_w);

		ImVec2 text_pos = ImGui::GetCursorScreenPos();
		text_pos.x += 10.0f;
		text_pos.y += 10.0f;

		if (ImGui::Button(("###" + item.id).c_str(), button_size))
		{
			if (item.id == "bangdl") ImGui::SetWindowFocus("bangdl");
			if (item.id == "tienich") ImGui::SetWindowFocus("tienich");
			if (item.id == "caidat") ImGui::SetWindowFocus("caidat");
		}

		ImGui::GetWindowDrawList()->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), visible_text.c_str());
	}

	// Xóa style cho các nút khác trước khi vẽ nút thu gọn
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	// Nút thu gọn luôn nằm cuối và có style riêng
	ImVec2 button_pos;
	button_pos.x = gd.letrai_menuben;
	button_pos.y = gd.chieucao_menuben + button_height * 3;
	ImGui::SetCursorScreenPos(button_pos);

	constexpr auto button_thugon_color = ImVec4(0.6f, 0.6f, 0.6f, 0.2f);
	const ImVec4 button_thugon_hover_color = adjust_color_brightness(button_thugon_color, 0.8f);
	const ImVec4 button_thugon_active_color = adjust_color_brightness(button_thugon_color, 1.2f);

	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
	ImGui::PushStyleColor(ImGuiCol_Button, button_thugon_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_thugon_hover_color);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_thugon_active_color);

	if (ImGui::Button(gd.menuben_thugon ? ">" : "<", button_size))
	{
		gd.yeucau_thugon = true;
		gd.batdau_thugon = std::chrono::steady_clock::now();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();

	ImGui::End();
}

// Hàm giao diện để tìm và cài đặt phần mềm từ winget
void giaodien_bangdl(giaodien& gd, const int chieurong_manhinh, const int chieucao_manhinh)
{
	gd.chieurong_bang = static_cast<float>(chieurong_manhinh) - gd.letrai_menuben * 3 - gd.chieurong_menuben;
	gd.chieucao_bang = static_cast<float>(chieucao_manhinh) - gd.letren_bang;
	gd.letrai_bang = gd.chieurong_menuben + gd.letrai_menuben * 2;

	ImGui::SetNextWindowPos(ImVec2(gd.letrai_bang, gd.letren_bang));
	ImGui::SetNextWindowSize(ImVec2(gd.chieurong_bang, gd.chieucao_bang));
	ImGui::Begin("bangdl", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	logic_giaodien lg_gd;
	khoitao_logic_giaodien(lg_gd);

	cap_nhat_bang_phan_mem(gd, lg_gd);
	ImGui::End();
}

void giaodien_tienich(giaodien& gd, const int chieurong_manhinh, const int chieucao_manhinh)
{
	float chieurong_tienich = static_cast<float>(chieurong_manhinh) - gd.letrai_menuben * 3 - gd.chieurong_menuben;
	float chieucao_tienich = static_cast<float>(chieucao_manhinh) - gd.letren_bang;
	float letrai_tienich = gd.chieurong_menuben + gd.letrai_menuben * 2;

	ImGui::SetNextWindowPos(ImVec2(letrai_tienich, gd.letren_bang));
	ImGui::SetNextWindowSize(ImVec2(chieurong_tienich, chieucao_tienich));

	ImGui::Begin("tienich", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	if (ImGui::Button("Sửa Window"))
	{
		chaylenh_suawindow();
	}
	ImGui::End();
}

void giaodien_caidat(giaodien& gd, const int chieurong_manhinh, const int chieucao_manhinh)
{
	float chieurong_caidat = static_cast<float>(chieurong_manhinh) - gd.letrai_menuben * 3 - gd.chieurong_menuben;
	float chieucao_caidat = static_cast<float>(chieucao_manhinh) - gd.letren_bang;
	float letrai_caidat = gd.chieurong_menuben + gd.letrai_menuben * 2;

	ImGui::SetNextWindowPos(ImVec2(letrai_caidat, gd.letren_bang));
	ImGui::SetNextWindowSize(ImVec2(chieurong_caidat, chieucao_caidat));

	ImGui::Begin("caidat", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Text("Tính năng đang được phát triển. Vui lòng đợi");

	ImGui::End();
}


