
#include "cuaso.h"
#include "giaodien.h"
#include "imgui.h"
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

void giaodien_menuben1(GiaoDien& giaodien, bool& hienthi_caidat, ImVec2& vitri_tinhnang, ImVec2& kichthuoc_tinhnang, const int chieucao_manhinh)
{
    float chieurong_hientai = giaodien.chieurong_menuben;

    // Cập nhật chiều cao menu
    giaodien.chieucao_menuben = static_cast<float>(chieucao_manhinh) - 28;

    // Xử lý thu gọn menu
    handle_collapse(
        giaodien.menuben_thugon,
        giaodien.yeucau_thugon,
        giaodien.batdau_thugon,
        chieurong_hientai,
        giaodien.chieurong_menuben_morong,
        giaodien.chieurong_menuben_thugon,
        giaodien.thoigian_thugon
    );

    // Ghi lại chiều rộng thực tế
    giaodien.chieurong_menuben = chieurong_hientai;

    // Tạo giao diện menu bên
    ImGui::SetNextWindowPos(ImVec2(giaodien.letrai_menuben, giaodien.letren_menuben));
    ImGui::SetNextWindowSize(ImVec2(chieurong_hientai, giaodien.chieucao_menuben));
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
    const int max_chars = std::max(1, static_cast<int>((chieurong_hientai - giaodien.chieurong_menuben_thugon) / (giaodien.chieurong_menuben - giaodien.chieurong_menuben_thugon) * static_cast<float>(text.size())));
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

    if (ImGui::Button(giaodien.menuben_thugon ? ">" : "<", ImVec2(chieurong_hientai, 30.0f)))
    {
        giaodien.yeucau_thugon = true;
        giaodien.batdau_thugon = std::chrono::steady_clock::now();
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

void giaodien_caidat_cot()
{
    ImGui::Begin("nut", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
    // Nút mở cài đặt tiêu đề
    if (ImGui::Button("Cài đặt tiêu đề"))
    {
        ImGui::OpenPopup("Settings");
    }

    // Hiển thị popup cài đặt
    if (ImGui::BeginPopup("Settings"))
    {
        for (int i = 0; i < columnCount; ++i)
        {
            ImGui::Checkbox(columns[i].ten_cot.c_str(), &columns[i].hienthi_cot); // Bật/tắt cột
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

// Hàm giao diện để tìm và cài đặt phần mềm từ winget
void giaodien_timvatai(const GiaoDien& gd, int chieurong_manhinh, int chieucao_manhinh)
{
    const float chieurong_hientai = static_cast<float>(chieurong_manhinh) - gd.letrai_menuben * 3 - gd.chieurong_menuben; // Chiều rộng hiện tại của menu bên

    ImGui::SetNextWindowPos(ImVec2(gd.chieurong_menuben + gd.letrai_menuben * 2, gd.letren_menuben));
    ImGui::SetNextWindowSize(ImVec2(chieurong_hientai, static_cast<float>(chieucao_manhinh) - 28));
    ImGui::Begin("Bảng Không Có Tiêu Đề", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    // Tính tổng số cột hiển thị
    int visibleColumns = 0;
    for (int i = 0; i < columnCount; ++i)
    {
        if (columns[i].hienthi_cot)
        {
            visibleColumns++;
        }
    }

    // Hiển thị bảng nếu có cột được bật
    if (visibleColumns > 0 && ImGui::BeginTable("MyTable", visibleColumns, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        // Hiển thị tiêu đề cột
        for (int i = 0; i < columnCount; ++i)
        {
            if (columns[i].hienthi_cot)
            {
                ImGui::TableSetupColumn(columns[i].ten_cot.c_str());
            }
        }
        ImGui::TableHeadersRow();

        // Thêm ô dưới tiêu đề (với các tính năng tương ứng)
        ImGui::TableNextRow();
        for (int i = 0; i < columnCount; ++i)
        {
            if (columns[i].hienthi_cot)
            {
                ImGui::TableSetColumnIndex(i);

                if (columns[i].ten_cot == "Tên phần mềm")
                {
                    // Thêm ô văn bản có thể nhập
                    static char searchBuffer[128] = "";
                    ImGui::InputText("##Search", searchBuffer, IM_ARRAYSIZE(searchBuffer));
                } else if (columns[i].ten_cot == "Trạng thái")
                {
                    // Thêm combo box cho "Trạng thái"
                    static const char* options[] = { "Đã cài", "Chưa cài" };
                    static int currentOption = 0;
                    if (ImGui::BeginCombo("##Trạng thái", options[currentOption]))
                    {
                        for (int j = 0; j < IM_ARRAYSIZE(options); ++j)
                        {
                            if (ImGui::Selectable(options[j], currentOption == j))
                            {
                                currentOption = j;
                            }
                        }
                        ImGui::EndCombo();
                    }
                } else if (columns[i].ten_cot == "Phiên bản")
                {
                    // Thêm combo box cho "Phiên bản"
                    static const char* versions[] = { "Có phiên bản mới" };
                    static int currentVersion = 0;
                    if (ImGui::BeginCombo("##Phiên bản", versions[currentVersion]))
                    {
                        for (int j = 0; j < IM_ARRAYSIZE(versions); ++j)
                        {
                            if (ImGui::Selectable(versions[j], currentVersion == j))
                            {
                                currentVersion = j;
                            }
                        }
                        ImGui::EndCombo();
                    }
                } else if (columns[i].ten_cot == "Phân loại")
                {
                    // Thêm combo box cho "Phân loại"
                    static const char* categories[] = {
                        "Trình duyệt", "Mạng xã hội", "Tài liệu", "Công cụ đa dụng", "Tiện ích", "Khác"
                    };
                    static int currentCategory = 0;
                    if (ImGui::BeginCombo("##Phân loại", categories[currentCategory]))
                    {
                        for (int j = 0; j < IM_ARRAYSIZE(categories); ++j)
                        {
                            if (ImGui::Selectable(categories[j], currentCategory == j))
                            {
                                currentCategory = j;
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
            }
        }

        ImGui::Button("bỏ các tùy chọn lọc");

        // Thêm dữ liệu vào bảng (giả sử có 5 hàng)
        for (int row = 0; row < 5; row++)
        {
            for (int i = 0; i < columnCount; ++i)
            {
                if (columns[i].hienthi_cot)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text("Cell %d,%s", row, columns[i].ten_cot.c_str());
                }
            }
        }

        ImGui::EndTable();
    }
    ImGui::End();
}
