#pragma once

#include "imgui.h"

#include <chrono>
#include <GLFW/glfw3.h>

using namespace std;

void handle_collapse(bool& is_collapsed, bool& collapse_requested, std::chrono::steady_clock::time_point& collapse_start_time, float& current_size, float expanded_size, float collapsed_size, float delay_seconds);

void giaodien_menuben(bool& hienthi_caidat, ImVec2& vitri_tinhnang, ImVec2& kichthuoc_tinhnang, int chieucao_manhinh);

void giaodien_caidat(GLFWwindow* cuaSo, bool& hienthi_caidat);

void giaodien_keotha_tep(GLFWwindow* cuaSo, int chieurong_manhinh, int chieucao_manhinh, const ImVec2& vitri_tinhnang, const ImVec2& kichthuoc_tinhnang);