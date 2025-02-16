// suawindow.cpp
#include "log_nhalam.h"
#include "suawindow.h"

#include <boost/process.hpp>

namespace bp = boost::process;

std::string thucthi_lenh(const std::string& lenh, const bool is_admin)
{
	try
	{
		std::string powershell_path = R"(C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe)";
		std::vector<std::string> args = { "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", lenh };

		if (is_admin)
		{
			// Chạy lệnh với quyền Administrator
			args = { "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command",
					 "Start-Process -FilePath 'powershell' -ArgumentList '-NoProfile', '-ExecutionPolicy', 'Bypass', '-Command', '" + lenh + "' -Verb RunAs" };
		}

		bp::child process(powershell_path, bp::args = args);
		process.wait();

		if (process.exit_code() != 0)
		{
			throw std::runtime_error("Lệnh lỗi với mã thoát: " + std::to_string(process.exit_code()));
		}

		return "Đã khởi chạy thành công.";
	} catch (const std::exception& ex)
	{
		return std::string("Lỗi: ") + ex.what();
	}
}

void chaylenh_suawindow()
{
	try
	{
		std::string powershell_path = R"(C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe)";

		// Lệnh PowerShell sẽ mở với quyền Admin và chạy tất cả lệnh theo thứ tự
		std::string command =
			"Start-Process -FilePath 'powershell.exe' -ArgumentList '-NoExit', '-ExecutionPolicy Bypass', "
			"'sfc /scannow; Dism /Online /Cleanup-Image /RestoreHealth; sfc /scannow' -Verb RunAs";

		bp::child process(powershell_path, bp::args = { "-NoProfile", "-ExecutionPolicy", "Bypass", "-Command", command });
		process.wait(); // Chờ tiến trình hoàn thành
	} catch (const std::exception& ex)
	{
		td_log(loai_log::loi, "Lỗi: " + std::string(ex.what()));
	}
}
