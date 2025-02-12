//CL_PhanMem.cpp
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#include "xuly_thongso_cuaso.h"

#include "log_nhalam.h"

int main()
{
	int mode = 0;
	g_logger = chuyendoi(mode);

	GLFWwindow* cuaso = khoitao_cuaso();
	if (!cuaso)
		return 1;

	vonglap_chinh(cuaso);

	dondep(cuaso);

	if (mode == 1)
	{
		flush_memory_logs_to_file();
	}

	spdlog::shutdown();
}
