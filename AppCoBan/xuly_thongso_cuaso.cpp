
#include "cuaso.h"
#include "giaodien.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "xuly_thongso_cuaso.h"

#include <iostream>


static void caidat_font()
{
    ImGuiIO& io = ImGui::GetIO();

    // Nạp font hỗ trợ tiếng Việt
    io.Fonts->AddFontFromFileTTF("C:/Users/ZitKhongLo/Downloads/Noto_Sans/NotoSans-VariableFont_wdth,wght.ttf", 16.0f, nullptr);

}

GLFWwindow* khoitao_cuaso()
{
    // Khởi tạo ImGui
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(cauhinh_cuaso.chieurong, cauhinh_cuaso.chieucao, cauhinh_cuaso.tieude, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Đặt giới hạn kích thước tối thiểu cho cửa sổ
    glfwSetWindowSizeLimits(window, cauhinh_cuaso.chieurong, cauhinh_cuaso.chieucao, GLFW_DONT_CARE, GLFW_DONT_CARE);

    // Lấy thông tin màn hình chính
    GLFWmonitor* manhinh_chinh = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(manhinh_chinh);

    // Tính toán vị trí để căn giữa cửa sổ
    int chieu_rong_cuaso = cauhinh_cuaso.chieurong;
    int chieu_cao_cuaso = cauhinh_cuaso.chieucao;
    int vitri_x = (mode->width - chieu_rong_cuaso) / 2;
    int vitri_y = (mode->height - chieu_cao_cuaso) / 2;

    // Đặt vị trí cửa sổ
    glfwSetWindowPos(window, vitri_x, vitri_y);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Thiết lập góc bo tròn cho cửa sổ
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 10.0f;

    caidat_font();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    return window;
}

void vonglap_chinh(GLFWwindow* cuaso)
{
    // Biến điều khiển
    bool hienthi_caidat = false;

    // Cấu hình màu sắc
    //ImU32 title_color = ImGui::GetColorU32(ImVec4(0.2f, 0.5f, 0.8f, 1.0f)); // Màu tiêu đề (RGBA)
    //ImU32 data_color = ImGui::GetColorU32(ImVec4(0.9f, 0.9f, 0.9f, 1.0f));  // Màu dữ liệu (RGBA)
    //ImU32 column_color = ImGui::GetColorU32(ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Màu cột (RGBA)
    //ImU32 transparent_color = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Màu trong suốt (RGBA)

    // Màu đen trong suốt
    //ImU32 black_transparent = ImGui::GetColorU32(ImVec4(0, 0, 0, 0.5f));

    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.5f, 0.8f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.5f, 0.8f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2f, 0.5f, 0.8f, 1.0f);

    ImVec2 vitri_tinhnang;
    ImVec2 kichthuoc_tinhnang;

    // Vòng lặp chính
    while (!glfwWindowShouldClose(cuaso))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Lấy kích thước cửa sổ chính
        int chieurong_manhinh, chieucao_manhinh;
        glfwGetFramebufferSize(cuaso, &chieurong_manhinh, &chieucao_manhinh);

        // Hiển thị các giao diện
        giaodien_menuben(hienthi_caidat, vitri_tinhnang, kichthuoc_tinhnang, chieucao_manhinh);
        giaodien_caidat(cuaso, hienthi_caidat);
        giaodien_keotha_tep(cuaso, chieurong_manhinh, chieucao_manhinh, vitri_tinhnang, kichthuoc_tinhnang);
        giaodien_test();

        // Render
        ImGui::Render();
        glViewport(0, 0, chieurong_manhinh, chieucao_manhinh);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(cuaso);
    }
}

void dondep(GLFWwindow* cuaso)
{
    // Dọn dẹp
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(cuaso);
    glfwTerminate();
}