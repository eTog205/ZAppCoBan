#include "cuaso.h"
#include "giaodien.h"

#include <functional>

// Hàm tiện ích để tạo nút mẫu
bool create_button(const char* label, const ImVec2& size, const ImVec2& position, std::function<void()> on_click, float rounding = 5.0f)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding); // Thiết lập bo tròn
    ImGui::SetCursorPos(position);
    bool clicked = ImGui::Button(label, size);
    ImGui::PopStyleVar(); // Khôi phục giá trị bo tròn mặc định

    if (clicked)
    {
        on_click();
        return true;
    }
    return false;
}

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

    vitri_tinhnang = ImGui::GetWindowPos();
    kichthuoc_tinhnang = ImGui::GetWindowSize();

    if (!is_collapsed)
    {
        create_button("caidat", ImVec2(kichthuoc_tinhnang.x - 20, 30), ImVec2(10, 10), [&]()
        {
            hienthi_caidat = true;
            ImGui::SetWindowFocus("caidat");
        });
    }

    // Nút thu gọn kéo dài từ cạnh trái đến cạnh phải
    create_button(is_collapsed ? ">" : "<", ImVec2(kichthuoc_tinhnang.x - 20, 30), ImVec2(0, kichthuoc_tinhnang.y - 30), [&]()
    {
        collapse_requested = true;
        collapse_start_time = std::chrono::steady_clock::now();
    });

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

    // Nút thu gọn ở góc phải trên cùng
    create_button(is_collapsed ? "V" : "v", ImVec2(windowWidth - 20, 30), ImVec2(0, 10), [&]()
    {
        collapse_requested = true;
        collapse_start_time = std::chrono::steady_clock::now();
    });

    if (!is_collapsed || collapse_requested)
    {
        ImGui::BeginChild("Vung tha tep", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar);

        ImVec2 content_size = ImGui::GetContentRegionAvail();
        ImVec2 text_size = ImGui::CalcTextSize("Drop Here");
        ImGui::SetCursorPos(ImVec2((content_size.x - text_size.x) / 2, (content_size.y - text_size.y) / 2));
        ImGui::Text("Drop Here");

        ImGui::EndChild(); // Kết thúc vùng con
    }

    ImGui::End();      // Kết thúc cửa sổ kéo thả
    ImGui::PopStyleVar(); // Khôi phục padding mặc định
}


