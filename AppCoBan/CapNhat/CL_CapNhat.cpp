//CL_CapNhat.cpp
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#include "../log_nhalam.h"
#include "capnhat.h"

int main()
{
	constexpr int mode = 1;
	g_logger = chuyendoi(mode);

	kiemtra_capnhat();

	if (mode == 1)
	{
		flush_memory_logs_to_file();
	}

	spdlog::shutdown();
}
