#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#include "../log_nhalam.h"
#include "capnhat.h"

int main()
{
	khoidong_log();

	kiemtra_va_capnhat();
}
