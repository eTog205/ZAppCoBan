#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#include "log_nhalam.h"
#include "tienich.h"
#include "xuly_thongso_cuaso.h"


int main()
{
	khoidong_log();

	launch_updater();

	GLFWwindow* cuaso = khoitao_cuaso();
	if (!cuaso)
		return 1;

	vonglap_chinh(cuaso);

	dondep(cuaso);
}