//xuly_thongso_cuaso.h
#pragma once
#include <GLFW/glfw3.h>

void set_taskbar_icon(GLFWwindow* window);
void caidat_font();

void glfw_error_callback(int error, const char* description);

GLFWwindow* khoitao_cuaso();
void vonglap_chinh(GLFWwindow* cuaso);
void dondep(GLFWwindow* cuaso);


