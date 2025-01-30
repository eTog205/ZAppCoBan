
#include "cuaso.h"
#include "giaodien.h"
#include "timvatai.h"


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
ImVec4 AdjustColorBrightness(const ImVec4& color, const float factor)
{
    return {
        std::min(color.x * factor, 1.0f),
        std::min(color.y * factor, 1.0f),
        std::min(color.z * factor, 1.0f),
        color.w // Giữ nguyên độ trong suốt (alpha)
    };
}

void giaodien_menuben1(ThongSo& ts, bool& hienthi_caidat, ImVec2& vitri_tinhnang, ImVec2& kichthuoc_tinhnang, const int chieucao_manhinh)
{
    //float chieurong_hientai = lay_giatri<float>(ts, "chieurong_menuben");
    float chieurong_hientai = ts.chieurong_menuben;

    // Cập nhật chiều cao menu
    ts.chieucao_menuben = static_cast<float>(chieucao_manhinh) - 28;

    // Xử lý thu gọn menu
    handle_collapse(
        ts.menuben_thugon,
        ts.yeucau_thugon,
        ts.batdau_thugon,
        chieurong_hientai,
        ts.chieurong_menuben_morong,
        ts.chieurong_menuben_thugon,
        ts.thoigian_thugon
    );


    // Ghi lại chiều rộng thực tế
    ts.chieurong_menuben = chieurong_hientai;

    // Tạo giao diện menu bên
    ImGui::SetNextWindowPos(ImVec2(ts.letrai_menuben, ts.letren_menuben));
    ImGui::SetNextWindowSize(ImVec2(chieurong_hientai, ts.chieucao_menuben));

    ImGui::Begin("Menu bên", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    vitri_tinhnang = ImGui::GetCursorScreenPos();
    kichthuoc_tinhnang = ImGui::GetWindowSize();

    // Màu sắc nút cài đặt
    ImVec4 button_caidat_color = ImVec4(0, 0, 0, 0);
    ImVec4 button_caidat_hover_color = (button_caidat_color.w == 0.0f) ? ImVec4(0.3f, 0.3f, 0.3f, 0.2f) : AdjustColorBrightness(button_caidat_color, 0.8f);
    ImVec4 button_caidat_active_color = (button_caidat_color.w == 0.0f) ? ImVec4(0.2f, 0.2f, 0.2f, 0.4f) : AdjustColorBrightness(button_caidat_color, 1.2f);

    // Nút mở cài đặt
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, button_caidat_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_caidat_hover_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_caidat_active_color);

    const auto button_caidat_size = ImVec2(chieurong_hientai, 30.0f);

    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
    const std::string text = "S cài đặt";
    const int max_chars = std::max(1, static_cast<int>((chieurong_hientai - ts.chieurong_menuben_thugon) / (ts.chieurong_menuben - ts.chieurong_menuben_thugon) * static_cast<float>(text.size())));

    const std::string visible_text = text.substr(0, max_chars);

    ImVec2 text_pos = ImGui::GetCursorScreenPos();
    text_pos.x += 10.0f;
    text_pos.y += 10.0f;

    if (ImGui::Button(" ", button_caidat_size))
    {
        hienthi_caidat = true;
        ImGui::SetWindowFocus("cai dat");
    }

    ImGui::GetWindowDrawList()->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), visible_text.c_str());
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();

    // Nút thu gọn/mở rộng menu
    float button_y_pos = kichthuoc_tinhnang.y - 28.0f;
    ImGui::SetCursorPos(ImVec2(0.0f, button_y_pos));

    ImVec4 button_color = ImVec4(0.6f, 0.6f, 0.6f, 0.2f);
    ImVec4 button_hover_color = AdjustColorBrightness(button_color, 0.8f);
    ImVec4 button_active_color = AdjustColorBrightness(button_color, 1.2f);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, button_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hover_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);

    if (ImGui::Button(ts.menuben_thugon ? ">" : "<", ImVec2(chieurong_hientai, 30.0f)))
    {
        ts.yeucau_thugon = true;
        ts.batdau_thugon = std::chrono::steady_clock::now();
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();

    ImGui::End();
}

void giaodien_caidat(GLFWwindow* cuaSo, bool& hienthi_caidat)
{
    static const char* danhSachCheDo[] = { "Cua So", "Toan Man Hinh", "Khong Vien" };

    if (hienthi_caidat)
    {
        ImGui::Begin("cai dat", &hienthi_caidat);

        // Combo box để chọn chế độ hiển thị
        if (ImGui::BeginCombo("Che Do Hien Thi", danhSachCheDo[cauhinh_cuaso.chedo]))
        {
            for (int i = 0; i < IM_ARRAYSIZE(danhSachCheDo); i++)
            {
                bool daChon = (cauhinh_cuaso.chedo == i);
                if (ImGui::Selectable(danhSachCheDo[i], daChon))
                {
                    cauhinh_cuaso.chedo = static_cast<CauHinhCuaSo::chedo_hienthi>(i);
                    thaydoi_chedo_hienthi(cuaSo, cauhinh_cuaso);
                }
                if (daChon)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::End();
    }
}

void comboBox_(const char* label, const char* options[], int options_count, int& currentSelection, float gt_botron)
{
    // Nếu chưa chọn gì (currentSelection == 0), hiển thị chuỗi rỗng ""
    const char* preview = (currentSelection == 0) ? "" : options[currentSelection];

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
            if (i != currentSelection) // Bỏ qua cái đã chọn
            {
                if (ImGui::Selectable(options[i]))
                {
                    currentSelection = i;
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
        currentSelection = 0; // Reset về trạng thái không hiển thị gì
    }
}

// Hàm giao diện để tìm và cài đặt phần mềm từ winget
void giaodien_timvatai(ThongSo& ts, const int chieurong_manhinh, const int chieucao_manhinh)
{
    ts.chieurong_bang = static_cast<float>(chieurong_manhinh) - ts.letrai_menuben * 3 - ts.chieurong_menuben; // Chiều rộng hiện tại của menu bên
    ts.chieucao_bang = static_cast<float>(chieucao_manhinh) - 28;
    ts.letrai_bang = ts.chieurong_menuben + ts.letrai_menuben * 2;

    ImGui::SetNextWindowPos(ImVec2(ts.letrai_bang, ts.letren_bang));
    ImGui::SetNextWindowSize(ImVec2(ts.chieurong_bang, ts.chieucao_bang));
    ImGui::Begin("Bảng Không Có Tiêu Đề", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    const auto p_min = ImVec2(ts.letrai_bang, ts.letren_bang);
    const auto p_max = ImVec2(ts.letrai_bang + ts.chieurong_bang, ts.letren_bang + ts.chieucao_bang);

    draw_list->AddRectFilled(p_min, p_max, IM_COL32(39, 39, 42, 255), ts.botron_nen, ImDrawFlags_RoundCornersAll); // Vẽ nền màu RGB(39, 39, 42)

    // Nút "+" để mở menu chọn cột
    ImGui::SameLine(ImGui::GetWindowWidth() - 30); // Đặt sát bên phải
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    if (ImGui::Button("+")) ts.hienthi_caidat_cot = !ts.hienthi_caidat_cot;
    ImGui::PopStyleColor(3);

    // Hiển thị danh sách bật/tắt cột
    if (ts.hienthi_caidat_cot)
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 150, ImGui::GetWindowPos().y + 25));
        ImGui::SetNextWindowSize(ImVec2(140, 120));
        ImGui::Begin("Chọn cột", &ts.hienthi_caidat_cot, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

        for (int i = 0; i < ts.soluong_cot; ++i)
        {
            if (ts.ten_cot[i] != "Tên")
                ImGui::Checkbox(ts.ten_cot[i].c_str(), &ts.hienthi_cot[i]);
        }

        ImGui::End();
    }

    // Hiển thị bảng nếu có cột được bật
    if (ImGui::BeginTable("MyTable", ts.soluong_cot, ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_Hideable))
    {
        for (int i = 0; i < ts.soluong_cot; ++i)
        {
            ImGui::TableSetupColumn(ts.ten_cot[i].c_str(), ts.hienthi_cot[i] ? ImGuiTableColumnFlags_None : ImGuiTableColumnFlags_NoHeaderWidth);
        }

        for (int i = 0; i < ts.soluong_cot; ++i)
        {
            ImGui::TableSetColumnEnabled(i, ts.hienthi_cot[i]);
        }
        ImGui::TableHeadersRow();

        // Thêm ô dưới tiêu đề (với các tính năng tương ứng)
        ImGui::TableNextRow();
        for (int i = 0; i < ts.soluong_cot; ++i)
        {
            if (ts.hienthi_cot[i])
            {
                ImGui::TableSetColumnIndex(i);

                if (ts.ten_cot[i] == "Tên")
                {
                    // Thêm ô văn bản có thể nhập
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ts.botron_nho);
                    ImGui::InputText("##Tìmkiếm", ts.searchBuffer, IM_ARRAYSIZE(ts.searchBuffer));
                    ImGui::PopStyleVar();

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
                } else if (ts.ten_cot[i] == "Trạng thái")
                {
                    // Thêm combo box cho "Trạng thái"
                    static const char* trangthai[] = { "##","Đã cài", "Chưa cài" };
                    comboBox_("##Trạngthái", trangthai, IM_ARRAYSIZE(trangthai), ts.trangthai_hientai, ts.botron_nho);
                } else if (ts.ten_cot[i] == "Phiên bản")
                {
                    // Thêm combo box cho "Phiên bản"
                    static const char* phienban[] = { "##","Có phiên bản mới" };
                    comboBox_("##Phiênbản", phienban, IM_ARRAYSIZE(phienban), ts.phienban_hientai, ts.botron_nho);
                } else if (ts.ten_cot[i] == "Phân loại")
                {
                    // Thêm combo box cho "Phân loại"
                    static const char* phanloai[] = {
                      "##","Trình duyệt", "Mạng xã hội", "Tài liệu", "Công cụ đa dụng", "Tiện ích", "Khác"
                    };
                    comboBox_("##PhânLoại", phanloai, IM_ARRAYSIZE(phanloai), ts.phanloai_hientai, ts.botron_nho);
                }
            }
        }

        if (ImGui::Button("Bỏ các tùy chọn lọc"))
        {
            // Reset tất cả về trạng thái ban đầu
            ts.searchBuffer[0] = '\0';  // Xóa nội dung ô tìm kiếm
            ts.trangthai_hientai = 0;    // Reset combo trạng thái
            ts.phienban_hientai = 0;     // Reset combo phiên bản
            ts.phanloai_hientai = 0;     // Reset combo phân loại
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleColor(3);
    ImGui::End();
}
