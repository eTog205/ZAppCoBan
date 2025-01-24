#include "cuaso.h"


// Hàm thay đổi chế độ hiển thị
void thaydoi_chedo_hienthi(GLFWwindow* cuaso, const CauHinhCuaSo& cauhinh)
{
    switch (cauhinh.chedo)
    {
        case CauHinhCuaSo::cuaso:
            glfwSetWindowMonitor(cuaso, nullptr, 100, 100, cauhinh.chieurong, cauhinh.chieucao, 0);
            glfwSetWindowAttrib(cuaso, GLFW_DECORATED, GLFW_TRUE); // Có viền
            break;

        case CauHinhCuaSo::toan_manhinh:
        {
            GLFWmonitor* manHinh = glfwGetPrimaryMonitor();
            const GLFWvidmode* chedo_manhinh = glfwGetVideoMode(manHinh);
            glfwSetWindowMonitor(cuaso, manHinh, 0, 0, chedo_manhinh->width, chedo_manhinh->height, chedo_manhinh->refreshRate);
            break;
        }

        case CauHinhCuaSo::khongvien:
        {
            // Không cần sử dụng glfwSetWindowMonitor, chỉ điều chỉnh thuộc tính
            GLFWmonitor* manHinh = glfwGetPrimaryMonitor();
            const GLFWvidmode* chedo_manhinh = glfwGetVideoMode(manHinh);

            glfwSetWindowAttrib(cuaso, GLFW_DECORATED, GLFW_FALSE); // Không viền
            glfwSetWindowPos(cuaso, 0, 0); // Đặt vị trí góc trên bên trái
            glfwSetWindowMonitor(cuaso,nullptr,0,0, chedo_manhinh->width, chedo_manhinh->height, chedo_manhinh->refreshRate); // Đặt kích thước phù hợp
            break;
        }
//else // KhongVien
//            {
//                glfwSetWindowAttrib(cuaso, GLFW_DECORATED, GLFW_FALSE); // Không viền
//                glfwSetWindowMonitor(cuaso, nullptr, 0, 0, chedo_manhinh->width, chedo_manhinh->height, 0);
//            }
//            break;
//        }
    }
}