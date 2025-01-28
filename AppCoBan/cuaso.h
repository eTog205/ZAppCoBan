#pragma once
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>


// Định nghĩa cấu trúc dữ liệu cho cấu hình
struct CauHinhCuaSo
{
    int chieurong; // Chiều rộng của cửa sổ
    int chieucao;  // Chiều cao của cửa sổ
    const char* tieude; // Tiêu đề của cửa sổ

    enum chedo_hienthi : uint8_t { cuaso, toan_manhinh, khongvien } chedo; // Chế độ hiển thị

    // Hàm tạo cấu hình mặc định
    static CauHinhCuaSo macdinh()
    {
        return { 1024, 700, "Tông", cuaso };
    }

};

inline CauHinhCuaSo cauhinh_cuaso = CauHinhCuaSo::macdinh();
inline std::vector<std::string> dropped_files;
//inline bool is_hovered = false;

// Hàm thay đổi chế độ hiển thị
void thaydoi_chedo_hienthi(GLFWwindow* cuaso, const CauHinhCuaSo& cauhinh);