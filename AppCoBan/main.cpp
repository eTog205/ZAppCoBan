
#include "xuly_thongso_cuaso.h"

#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

int main()
{
    GLFWwindow* cuaso = khoitao_cuaso();
    if (!cuaso)
        return 1;

    vonglap_chinh(cuaso);

    dondep(cuaso);

}