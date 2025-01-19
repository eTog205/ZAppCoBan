#include "cuaso.h"
#include "giaodien.h"
#include "imgui.h"

#include <functional>


void handle_collapse(bool& is_collapsed, bool& collapse_requested, std::chrono::steady_clock::time_point& collapse_start_time, float& current_size, float expanded_size, float collapsed_size, float delay_seconds)
{
    if (collapse_requested)
    {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<float> elapsed_seconds = now - collapse_start_time;
        float target_size = is_collapsed ? expanded_size : collapsed_size;
        float start_size = is_collapsed ? collapsed_size : expanded_size;
        float t = elapsed_seconds.count() / delay_seconds;
        if (t >= 1.0f)
        {
            t = 1.0f;
            is_collapsed = !is_collapsed;
            collapse_requested = false;
        }
        current_size = start_size + t * (target_size - start_size);
    }
}

// Hàm hỗ trợ để tự động sửa đổi màu
ImVec4 AdjustColorBrightness(const ImVec4& color, float factor)
{
    return {
        std::min(color.x * factor, 1.0f),
        std::min(color.y * factor, 1.0f),
        std::min(color.z * factor, 1.0f),
        color.w // Giữ nguyên độ trong suốt (alpha)
    };
}

void giaodien_menuben(bool& hienthi_caidat, ImVec2& vitri_tinhnang, ImVec2& kichthuoc_tinhnang, int chieucao_manhinh)
{
    static bool is_collapsed = false; // Biến trạng thái thu gọn
    static bool collapse_requested = false; // Biến yêu cầu thu gọn
    static auto collapse_start_time = std::chrono::steady_clock::now(); // Thời gian bắt đầu thu gọn
    static float current_width = 180.0f; // Chiều rộng hiện tại của menu bên
    constexpr float expanded_width = 180.0f; // Chiều rộng khi mở rộng
    constexpr float collapsed_width = 50.0f; // Chiều rộng khi thu gọn
    constexpr float delay_seconds = 0.3f; // Thời gian delay (giây)

    handle_collapse(is_collapsed, collapse_requested, collapse_start_time, current_width, expanded_width, collapsed_width, delay_seconds);

    // Giao diện menu bên
    ImGui::SetNextWindowPos(ImVec2(14, 14));
    ImGui::SetNextWindowSize(ImVec2(current_width, static_cast<float>(chieucao_manhinh) - 28));
    ImGui::Begin("tinh nang", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

    vitri_tinhnang = ImGui::GetCursorScreenPos();
    kichthuoc_tinhnang = ImGui::GetWindowSize();

    ImVec4 button_caidat_color = ImVec4(0, 0, 0, 0);
    ImVec4 button_caidat_hover_color = (button_caidat_color.w == 0.0f) ? ImVec4(0.3f, 0.3f, 0.3f, 0.2f) : AdjustColorBrightness(button_caidat_color, 0.8f);
    ImVec4 button_caidat_active_color = (button_caidat_color.w == 0.0f) ? ImVec4(0.2f, 0.2f, 0.2f, 0.4f) : AdjustColorBrightness(button_caidat_color, 1.2f);


    // Nút mở cài đặt có chiều rộng bằng chiều rộng của menu bên, không nền
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, button_caidat_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_caidat_hover_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_caidat_active_color);

    auto button_caidat_size = ImVec2(current_width, 30.0f); // Kích thước nút

    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));

    // Hiển thị chữ "S" hoặc "S caidat" dựa trên trạng thái
    std::string text = "S caidat";
    int max_chars = std::max(1, static_cast<int>((current_width - collapsed_width) / (expanded_width - collapsed_width) * static_cast<float>(text.size())));
    std::string visible_text = text.substr(0, max_chars);

    // Vẽ nút
    if (ImGui::Button(" ", button_caidat_size))
    {
        hienthi_caidat = true;
        ImGui::SetWindowFocus("caidat");
    }

    // Tính vị trí chữ luôn đặt cố định
    ImVec2 text_pos = ImGui::GetCursorScreenPos();
    text_pos.x += 10.0f; // Cách nút 10px theo trục X
    text_pos.y += 10.0f; // Cách nút 10px theo trục Y

    // Vẽ chữ
    ImGui::GetWindowDrawList()->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), visible_text.c_str());

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();


    // Nút thu gọn có chiều rộng bằng chiều rộng của menu bên, bo tròn cạnh và nằm sát cạnh dưới, vị trí bắt đầu từ sát cạnh bên trái đến sát cạnh bên phải
    float button_y_pos = kichthuoc_tinhnang.y - 28.0f;
    ImGui::SetCursorPos(ImVec2(0.0f, button_y_pos));

    auto button_color = ImVec4(0.6f, 0.6f, 0.6f, 0.2f);
    ImVec4 button_hover_color = AdjustColorBrightness(button_color, 0.8f);
    ImVec4 button_active_color = AdjustColorBrightness(button_color, 1.2f);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, button_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hover_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);

    auto button_size = ImVec2(current_width, 30.0f); // Kích thước nút

    if (ImGui::Button(is_collapsed ? ">" : "<", button_size))
    {
        collapse_requested = true;
        collapse_start_time = std::chrono::steady_clock::now();
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

void giaodien_keotha_tep(GLFWwindow* cuaSo, int chieurong_manhinh, int chieucao_manhinh, const ImVec2& vitri_tinhnang, const ImVec2& kichthuoc_tinhnang)
{
    static bool is_collapsed = false; // Biến trạng thái thu gọn
    static bool collapse_requested = false; // Biến yêu cầu thu gọn
    static auto collapse_start_time = std::chrono::steady_clock::now(); // Thời gian bắt đầu thu gọn
    static float current_height = 150.0f; // Chiều cao hiện tại của cửa sổ
    constexpr float delay_seconds = 0.3f; // Thời gian delay (giây)
    constexpr float drop_zone_height = 150.0f;  // Chiều cao của cửa sổ kéo thả
    constexpr float collapsed_height = 50.0f;  // Chiều cao khi thu gọn
    constexpr float margin = 14.0f;            // Khoảng cách viền ngoài

    handle_collapse(is_collapsed, collapse_requested, collapse_start_time, current_height, drop_zone_height, collapsed_height, delay_seconds);

    float windowPosX = vitri_tinhnang.x + kichthuoc_tinhnang.x + margin; // Cách phải menu bên 14 pixel
    float windowPosY = static_cast<float>(chieucao_manhinh) - current_height - margin; // Cách viền dưới 14 pixel
    float windowWidth = static_cast<float>(chieurong_manhinh) - windowPosX - margin; // Chiều rộng còn lại

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20)); // Padding bên trong
    ImGui::SetNextWindowPos(ImVec2(windowPosX, windowPosY));
    ImGui::SetNextWindowSize(ImVec2(windowWidth, current_height));

    ImGui::Begin("Cua so tha tep", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

    auto button_color = ImVec4(1.0f, 0.71f, 0.76f, 1.0f);
    ImVec4 button_hover_color = AdjustColorBrightness(button_color, 0.8f);
    ImVec4 button_active_color = AdjustColorBrightness(button_color, 1.2f);


    // Nút thu gọn ở góc phải trên cùng
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, button_color); // default_color
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hover_color); // hover_color
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color); // active_color

    auto button_size = ImVec2(25, 25); // Kích thước nút

    ImGui::SetCursorPos(ImVec2(windowWidth - 25.0f, 5.0f));

    if (ImGui::Button(is_collapsed ? "^" : "v", button_size))
    {
        collapse_requested = true;
        collapse_start_time = std::chrono::steady_clock::now();
    }

    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();


    // Ô nhập đường dẫn tệp
    ImGui::SetCursorPos(ImVec2(30.0f, 10.0f));
    ImGui::SetNextItemWidth(windowWidth - button_size.x * 4);

    static char file_path[256] = ""; // Bộ đệm chứa đường dẫn tệp
    ImGui::InputText("###hidden", file_path, IM_ARRAYSIZE(file_path));

    if (!is_collapsed || collapse_requested)
    {
        ImVec2 content_size1 = ImGui::GetContentRegionAvail();

        float pos_y = button_size.y * 1.5f + 10.0f;
        float drop_zone_width = content_size1.x * 0.8f;
        float chieucao_vung_thatep = drop_zone_height - pos_y - 10.0f;
        float centered_x = (content_size1.x - drop_zone_width) / 2.0f;


        // Đặt vị trí và bắt đầu vùng thả tệp
        ImGui::SetCursorPos(ImVec2(centered_x, pos_y));
        ImGui::BeginChild("Vung tha tep", ImVec2(drop_zone_width, chieucao_vung_thatep), true, ImGuiWindowFlags_NoScrollbar);

        ImVec2 content_size = ImGui::GetContentRegionAvail();
        ImVec2 text_size = ImGui::CalcTextSize("Drop Here");
        ImGui::SetCursorPos(ImVec2((content_size.x - text_size.x) / 2, (content_size.y - text_size.y) / 2));
        ImGui::Text("Drop Here");

        ImGui::EndChild(); // Kết thúc vùng con
    }

    ImGui::End();      // Kết thúc cửa sổ kéo thả
    ImGui::PopStyleVar(); // Khôi phục padding mặc định
}


