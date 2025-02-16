//xuly_thongso_cuaso.cpp
#include "csdl.h"
#include "cuaso.h"
#include "dv_csdl.h"
#include "giaodien.h"
#include "log_nhalam.h"
#include "xuly_thongso_cuaso.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


void caidat_font()
{
	const ImGuiIO& io = ImGui::GetIO();

	// Tùy chọn phạm vi glyph mặc định
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
	// Khởi tạo ImGui
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

	//td_log(loai_log::thong_bao, "Vòng lặp chính khởi động");

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
		giaodien_thanhcongcu(gd, chieurong_manhinh, chieucao_manhinh);
		giaodien_menuben(gd, chieucao_manhinh);
		giaodien_tienich(gd, chieurong_manhinh, chieucao_manhinh);
		giaodien_caidat(gd, chieurong_manhinh, chieucao_manhinh);
		giaodien_bangdl(gd, chieurong_manhinh, chieucao_manhinh);

		// Render
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