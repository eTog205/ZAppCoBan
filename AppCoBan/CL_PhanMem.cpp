//CL_PhanMem.cpp
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include "log_nhalam.h"
#include "xuly_thongso_cuaso.h"

int main()
{
	static boost::interprocess::named_mutex mutex(boost::interprocess::open_or_create, "ZitApp1");

	// Nếu mutex đã bị khóa, tức là phần mềm đang chạy, thoát ngay
	if (!mutex.try_lock())
	{
		MessageBoxA(nullptr, "Phần mềm đã chạy, không thể mở thêm!", "Cảnh báo", MB_OK | MB_ICONWARNING);
		return 1;
	}

	constexpr int mode = 1;
	g_logger = chuyendoi(mode);

	GLFWwindow* cuaso = khoitao_cuaso();
	if (!cuaso)
		return 1;

	vonglap_chinh(cuaso);

	dondep(cuaso);

	if (mode == 1)
		flush_memory_logs_to_file();

	spdlog::shutdown();

	// Giải phóng mutex khi tắt chương trình
	boost::interprocess::named_mutex::remove("ZitApp1");
}
