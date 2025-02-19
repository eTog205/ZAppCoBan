//xuly_thongso_cuaso.h
#pragma once

void set_taskbar_icon(GLFWwindow* window);

void dat_icon_cho_cuaso(GLFWwindow* window, const char* icon_path);

void caidat_font();

void glfw_error_callback(int error, const char* description);

GLFWwindow* khoitao_cuaso();
void vonglap_chinh(GLFWwindow* cuaso);
void dondep(GLFWwindow* cuaso);


