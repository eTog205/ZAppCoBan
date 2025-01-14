#pragma once
#include <GLFW/glfw3.h>
#include <imgui.h>

using namespace std;


// Khởi tạo cửa sổ và ImGui
GLFWwindow* khoitao_cuaso();

// Vòng lặp chính
void vonglap_chinh(GLFWwindow* cuaso);

void dondep(GLFWwindow* cuaso);

