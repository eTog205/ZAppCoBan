#pragma once

#include "imgui.h"

#include <chrono>
#include <GLFW/glfw3.h>

using namespace std;


struct GiaoDien
{
    float chieurong_menuben = 180.0f;  // Chiều rộng menu bên
    float chieurong_menuben_thugon = 50.0f; // Chiều rộng khi menu thu gọn
    float chieurong_menuben_morong = 180.0f;
    float letrai_menuben = 14.0f;      // Lề trái menu bên
    float letren_menuben = 14.0f;      // Lề trên menu bên
    float chieucao_menuben = 0.0f;     // Chiều cao menu bên (tự động từ chiều cao màn hình)

    bool menuben_thugon = false;       // Trạng thái thu gọn menu bên
    bool yeucau_thugon = false;       // Biến yêu cầu thu gọn
    std::chrono::steady_clock::time_point batdau_thugon = std::chrono::steady_clock::now(); // Thời gian bắt đầu thu gọn
    float thoigian_thugon = 0.3f;      // Thời gian chuyển đổi trạng thái
};


void handle_collapse(bool& is_collapsed, bool& collapse_requested, const std::chrono::steady_clock::time_point& collapse_start_time, float& current_size, float expanded_size, float collapsed_size, float delay_seconds);

ImVec4 AdjustColorBrightness(const ImVec4& color, float factor);

void giaodien_menuben1(GiaoDien& giaodien, bool& hienthi_caidat, ImVec2& vitri_tinhnang, ImVec2& kichthuoc_tinhnang, int chieucao_manhinh);

void giaodien_caidat(GLFWwindow* cuaSo, bool& hienthi_caidat);

void giaodien_caidat_cot();

void giaodien_timvatai(const GiaoDien& gd, int chieurong_manhinh, int chieucao_manhinh);


