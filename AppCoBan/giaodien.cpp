
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

void giaodien_menuben1(giaodien& gd, bool& hienthi_caidat, ImVec2& vitri_tinhnang, ImVec2& kichthuoc_tinhnang, const int chieucao_manhinh)
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
    const int max_chars = std::max(1, static_cast<int>((chieurong_hientai - gd.chieurong_menuben_thugon) / (gd.chieurong_menuben - gd.chieurong_menuben_thugon) * static_cast<float>(text.size())));

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
    const float button_y_pos = kichthuoc_tinhnang.y - 28.0f;
    ImGui::SetCursorPos(ImVec2(0.0f, button_y_pos));

    constexpr auto button_color = ImVec4(0.6f, 0.6f, 0.6f, 0.2f);
    const ImVec4 button_hover_color = AdjustColorBrightness(button_color, 0.8f);
    const ImVec4 button_active_color = AdjustColorBrightness(button_color, 1.2f);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, button_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hover_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);

    if (ImGui::Button(gd.menuben_thugon ? ">" : "<", ImVec2(chieurong_hientai, 30.0f)))
    {
        gd.yeucau_thugon = true;
        gd.batdau_thugon = std::chrono::steady_clock::now();
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

// Cập nhật bảng phần mềm
void capNhatBangPhanMem(giaodien& gd, logic_giaodien& lg_gd)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    const auto p_min = ImVec2(gd.letrai_bang, gd.letren_bang);
    const auto p_max = ImVec2(gd.letrai_bang + gd.chieurong_bang, gd.letren_bang + gd.chieucao_bang);

    draw_list->AddRectFilled(p_min, p_max, IM_COL32(39, 39, 42, 255), gd.botron_nen, ImDrawFlags_RoundCornersAll); // Vẽ nền màu RGB(39, 39, 42)

    // Nút "+" để mở menu chọn cột
    ImGui::SameLine(ImGui::GetWindowWidth() - 30); // Đặt sát bên phải
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    if (ImGui::Button("+")) gd.hienthi_caidat_cot = !gd.hienthi_caidat_cot;
    ImGui::PopStyleColor(3);

    // Hiển thị danh sách bật/tắt cột
    if (gd.hienthi_caidat_cot)
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x - 150, ImGui::GetWindowPos().y + 25));
        ImGui::SetNextWindowSize(ImVec2(140, 120));
        ImGui::Begin("Chọn cột", &gd.hienthi_caidat_cot, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

        for (int i = 0; i < lg_gd.soluong_cot; ++i)
        {
            if (lg_gd.ten_cot[i] != "Tên")
                ImGui::Checkbox(lg_gd.ten_cot[i].c_str(), &gd.hienthi_cot[i]);
        }

        ImGui::End();
    }

    // Thiết lập bảng
    if (ImGui::BeginTable("BangPhanMem", lg_gd.soluong_cot, ImGuiTableFlags_Hideable | ImGuiTableFlags_NoBordersInBody))
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

        // Thiết lập tiêu đề cột
        for (int i = 0; i < lg_gd.soluong_cot; ++i)
        {
            ImGui::TableSetupColumn(lg_gd.ten_cot[i].c_str(), gd.hienthi_cot[i] ? ImGuiTableColumnFlags_None : ImGuiTableColumnFlags_NoHeaderWidth);
        }

        for (int i = 0; i < lg_gd.soluong_cot; ++i)
        {
            ImGui::TableSetColumnEnabled(i, gd.hienthi_cot[i]);
        }

        // Tiêu đề cột, thay thế "chọn" bằng checkbox tổng
        ImGui::TableNextRow();
        for (int i = 0; i < lg_gd.soluong_cot; ++i)
        {
            if (!gd.hienthi_cot[i]) continue; // Nếu cột bị ẩn, bỏ qua

            ImGui::TableSetColumnIndex(i);
            ImGui::TableSetupColumn("Chọn", ImGuiTableColumnFlags_WidthFixed, 40.0f);
            for (int i1 = 0; i1 < lg_gd.soluong_cot; i1++)
            {
                ImGui::TableSetupColumn(lg_gd.ten_cot[i1].c_str());
            }
            ImGui::TableHeadersRow();

            // Thêm các hàng và các ô vào bảng
            for (size_t row = 0; row < gd.data.size(); ++row)
            {
                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                std::string id = gd.data[row][0];
                ImGui::Checkbox(("##check" + std::to_string(row)).c_str(), &gd.selected_map[id]);


                for (size_t column = 0; column < gd.data[row].size(); ++column)
                {
                    ImGui::TableSetColumnIndex(static_cast<int>(column + 1));
                    ImGui::Text("%s", gd.data[row][column].c_str());
                }
            }
            //if (gd.ten_cot[i] == "chọn")
            //{
            //    bool chonTatCaTruoc = gd.chon_tatca;
            //    // Checkbox tổng để chọn tất cả phần mềm
            //    if (ImGui::Checkbox("##ChonTatCa", &gd.chon_tatca))
            //    {
            //        if (chonTatCaTruoc != gd.chon_tatca)
            //        {
            //            for (int j = 0; j < gd.soLuongPhanMemTruocDo; j++)
            //                gd.chon_phanmem[j] = gd.chon_tatca;
            //        }
            //    }

            //    // Kiểm tra lại nếu tất cả checkbox đều được chọn hoặc bỏ chọn
            //    bool tatCaDaChon = true, tatCaBoChon = true;
            //    for (int j = 0; j < gd.soLuongPhanMemTruocDo; j++)
            //    {
            //        if (gd.chon_phanmem[j])
            //            tatCaBoChon = false;
            //        else
            //            tatCaDaChon = false;
            //    }

            //    // Cập nhật trạng thái checkbox tổng
            //    if (tatCaDaChon)
            //        gd.chon_tatca = true;
            //    else if (tatCaBoChon)
            //        gd.chon_tatca = false;

            //} else
            //{
            //    ImGui::Text("%s", gd.ten_cot[i].c_str()); // Hiển thị tên các cột khác
            //}
        }

        // Dòng nhập bộ lọc
        ImGui::TableNextRow();
        for (int i = 0; i < lg_gd.soluong_cot; ++i)
        {
            if (!gd.hienthi_cot[i]) continue;

            ImGui::TableSetColumnIndex(i);
            if (lg_gd.ten_cot[i] == "Tên")
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, gd.botron_nho);
                ImGui::InputText("##Tìmkiếm", gd.searchBuffer, IM_ARRAYSIZE(gd.searchBuffer));
                ImGui::PopStyleVar();
            }
        }

        // Hiển thị dữ liệu trong bảng
        //for (int i = 0; i < gd.soLuongPhanMemTruocDo; i++)
        //{
        //    if (gd.searchBuffer[0] != '\0' && std::string(pm.danhSachPhanMem[i].name).find(gd.searchBuffer) == std::string::npos)
        //    {
        //        continue; // Bỏ qua nếu không khớp tìm kiếm
        //    }

        //    ImGui::TableNextRow();
        //    int columnIndex = 0;

        //    for (int j = 0; j < lg_gd.soluong_cot; ++j)
        //    {
        //        if (!gd.hienthi_cot[j]) continue; // Bỏ qua nếu cột bị ẩn

        //        ImGui::TableSetColumnIndex(columnIndex);

        //        if (lg_gd.ten_cot[j] == "chọn")
        //        {
        //            // Hiển thị checkbox cho từng dòng
        //            ImGui::Checkbox(("##Chon" + std::to_string(i)).c_str(), &gd.chon_phanmem[i]);
        //        } else if (lg_gd.ten_cot[j] == "Trạng thái")
        //        {
        //            ImGui::Text("%s", pm.danhSachPhanMem[i].trangthai.c_str());
        //        } else if (lg_gd.ten_cot[j] == "Tên")
        //        {
        //            ImGui::Text("%s", pm.danhSachPhanMem[i].name.c_str());
        //        } else if (lg_gd.ten_cot[j] == "Phiên bản")
        //        {
        //            ImGui::Text("%s", pm.danhSachPhanMem[i].version.c_str());
        //        } else if (lg_gd.ten_cot[j] == "Phân loại")
        //        {
        //            ImGui::Text("%s", pm.danhSachPhanMem[i].category.c_str());
        //        }

        //        columnIndex++;
        //    }
        //}

        ImGui::EndTable();
    }

    ImGui::PopStyleColor(3);

    // Nút bỏ lọc
    if (ImGui::Button("Bỏ các tùy chọn lọc"))
    {
        gd.searchBuffer[0] = '\0';
        gd.trangthai_hientai = 0;
        gd.phienban_hientai = 0;
        gd.phanloai_hientai = 0;
    }
}

// Hàm giao diện để tìm và cài đặt phần mềm từ winget
void giaodien_timvatai(giaodien& gd, const int chieurong_manhinh, const int chieucao_manhinh)
{

    gd.chieurong_bang = static_cast<float>(chieurong_manhinh) - gd.letrai_menuben * 3 - gd.chieurong_menuben; // Chiều rộng hiện tại của menu bên
    gd.chieucao_bang = static_cast<float>(chieucao_manhinh) - gd.letren_bang;
    gd.letrai_bang = gd.chieurong_menuben + gd.letrai_menuben * 2;

    ImGui::SetNextWindowPos(ImVec2(gd.letrai_bang, gd.letren_bang));
    ImGui::SetNextWindowSize(ImVec2(gd.chieurong_bang, gd.chieucao_bang));
    ImGui::Begin("Bảng Không Có Tiêu Đề", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    logic_giaodien lg_gd;
    capNhatBangPhanMem(gd, lg_gd);
    ImGui::End();
}

void giaodien_thanhcongcu(const giaodien& gd, const int chieurong_manhinh, const int chieucao_manhinh)
{
    float chieurong_hientai = static_cast<float>(chieurong_manhinh) - gd.letrai_menuben * 2;
    float chieucao_hientai = static_cast<float>(chieucao_manhinh) - gd.chieucao_menuben - gd.letrai_menuben * 2;

    // Tạo giao diện menu bên
    ImGui::SetNextWindowPos(ImVec2(gd.letrai_menuben, gd.letrai_menuben));
    ImGui::SetNextWindowSize(ImVec2(chieurong_hientai, chieucao_hientai));

    ImGui::Begin("thanh công cụ", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    ImGui::Button("nút tải về");

    ImGui::End();
}