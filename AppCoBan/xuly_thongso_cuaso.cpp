//xuly_thongso_cuaso.cpp
#include "csdl.h"
#include "cuaso.h"
#include "dv_csdl.h"
#include "giaodien.h"
#include "log_nhalam.h"
#include "resource.h"
#include "xuly_thongso_cuaso.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void set_taskbar_icon(GLFWwindow* window)
{
	// Lấy handle của cửa sổ
	const HWND hwnd = glfwGetWin32Window(window);

	// Tải icon từ resource IDI_ICON1 phải khớp với resource.h
	auto h_icon = static_cast<HICON>(LoadImage(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
	if (h_icon)
	{
		SetClassLongPtr(hwnd, GCLP_HICON, reinterpret_cast<LONG_PTR>(h_icon));
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(h_icon));
		//SendMessage(hwnd, WM_SETICON, ICON_SMALL2, reinterpret_cast<LPARAM>(h_icon));
		//SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(h_icon));
	}
}
#endif

void dat_icon_cho_cuaso(GLFWwindow* window, const char* icon_path)
{
	// Load ảnh RGBA
	int width = 0, height = 0, channels = 0;
	unsigned char* data = stbi_load(icon_path, &width, &height, &channels, 4);
	if (!data)
	{
		td_log(loai_log::canh_bao, "Không thể tải icon: " + std::string(icon_path));
		return;
	}

	GLFWimage image;
	image.width = width;
	image.height = height;
	image.pixels = data;

	glfwSetWindowIcon(window, 1, &image);

	stbi_image_free(data);
}

void caidat_font()
{
	const ImGuiIO& io = ImGui::GetIO();

	//const ImWchar* full_range = io.Fonts->GetGlyphRangesDefault(); // Toàn bộ ký tự cơ bản

	// Nạp font hỗ trợ tiếng Việt
	io.Fonts->AddFontFromFileTTF("arial.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesVietnamese()); // Toàn bộ ký tự cơ bản
	//arial
	//NotoSans-VariableFont_wdth,wght
	// Xây dựng lại texture cho font
	//io.Fonts->Build();
}

GLFWwindow* khoitao_cuaso()
{
	if (!glfwInit())
	{
		td_log(loai_log::loi, "khởi tạo glfw thất bại");
		return nullptr;
	}

	GLFWwindow* window = glfwCreateWindow(cauhinh_cuaso.chieurong, cauhinh_cuaso.chieucao, cauhinh_cuaso.tieude, nullptr, nullptr);

	if (!window)
	{
		td_log(loai_log::loi, "khởi tạo cửa sổ glfw thất bại");
		glfwTerminate();
		return nullptr;
	}

	//dat_icon_cho_cuaso(window, "duck.png");

	// Đặt giới hạn kích thước tối thiểu cho cửa sổ
	glfwSetWindowSizeLimits(window, cauhinh_cuaso.chieurong, cauhinh_cuaso.chieucao, GLFW_DONT_CARE, GLFW_DONT_CARE);

	// Lấy thông tin màn hình chính
	GLFWmonitor* manhinh_chinh = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(manhinh_chinh);

	// Tính toán vị trí để căn giữa cửa sổ
	const int chieu_rong_cuaso = cauhinh_cuaso.chieurong;
	const int chieu_cao_cuaso = cauhinh_cuaso.chieucao;
	const int vitri_x = (mode->width - chieu_rong_cuaso) / 2;
	const int vitri_y = (mode->height - chieu_cao_cuaso) / 2;

	// Đặt vị trí cửa sổ
	glfwSetWindowPos(window, vitri_x, vitri_y);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Thiết lập góc bo tròn cho cửa sổ
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 10.0f;
	style.FrameRounding = 4.0f;

	caidat_font();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	return window;
}

void vonglap_chinh(GLFWwindow* cuaso)
{
	capnhat_data();
	khoidong_sql();

	giaodien gd;
	LogicXuLy::nap_du_lieu(gd);

	while (!glfwWindowShouldClose(cuaso))
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		int chieurong_manhinh, chieucao_manhinh;
		glfwGetFramebufferSize(cuaso, &chieurong_manhinh, &chieucao_manhinh);

		giaodien_thanhcongcu(gd, chieurong_manhinh, chieucao_manhinh);
		giaodien_menuben(gd, chieucao_manhinh);
		giaodien_tienich(gd, chieurong_manhinh, chieucao_manhinh);
		giaodien_caidat(gd, chieurong_manhinh, chieucao_manhinh);
		giaodien_bangdl(gd, chieurong_manhinh, chieucao_manhinh);

		ImGui::Render();
		glViewport(0, 0, chieurong_manhinh, chieucao_manhinh);
		glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		glFinish();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(cuaso);
	}
}

void dondep(GLFWwindow* cuaso)
{
	close_database();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(cuaso);
	glfwTerminate();
}


