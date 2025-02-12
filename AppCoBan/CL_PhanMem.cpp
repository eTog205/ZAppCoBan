//CL_PhanMem.cpp
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#include "log_nhalam.h"
#include "xuly_thongso_cuaso.h"

int main()
{
	khoidong_log();

	GLFWwindow* cuaso = khoitao_cuaso();
	if (!cuaso)
		return 1;

	vonglap_chinh(cuaso);

	dondep(cuaso);
}