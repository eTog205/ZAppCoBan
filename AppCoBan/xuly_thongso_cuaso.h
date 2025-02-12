#pragma once
#include <GLFW/glfw3.h>

void caidat_font();

// Khởi tạo cửa sổ và ImGui
GLFWwindow* khoitao_cuaso();

// Vòng lặp chính
void vonglap_chinh(GLFWwindow* cuaso);

void dondep(GLFWwindow* cuaso);

