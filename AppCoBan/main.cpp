
#include "xuly_thongso_cuaso.h"

int main()
{
    GLFWwindow* cuaso = khoitao_cuaso();
    if (!cuaso)
        return 1;

    vonglap_chinh(cuaso);

    dondep(cuaso);



}