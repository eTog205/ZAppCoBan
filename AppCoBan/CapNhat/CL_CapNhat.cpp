#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#include "capnhat.h"
#include "log_nhalam.h"

int main()
{
	khoidong_log();

	kiemtra_va_capnhat();
}
