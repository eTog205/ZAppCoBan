//giainen.cpp
#include "get.h"
#include "giainen.h"

#include "../log_nhalam.h"
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>


void xoa_tapnen(const std::string& file_path)
{
	if (std::remove(file_path.c_str()) != 0)
	{
		// Xóa thất bại
		td_log(loai_log::loi, "Xóa tệp nén thất bại: " + file_path);
	}
}

std::string get_win_rar_path()
{
	HKEY h_key;
	char buffer[512]{};
	DWORD dw_size = sizeof(buffer);

	// Thử mở key "SOFTWARE\WinRAR"
	LONG l_res = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WinRAR", 0, KEY_READ, &h_key);
	if (l_res != ERROR_SUCCESS)
	{
		// Nếu không thành công, thử mở key dưới WOW6432Node (cho hệ thống 64-bit)
		l_res = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\WinRAR", 0, KEY_READ, &h_key);
		if (l_res != ERROR_SUCCESS)
		{
			return "";  // Không tìm thấy thông tin WinRAR
		}
	}

	// Đọc giá trị "ExePath"
	l_res = RegQueryValueExA(h_key, "exe64", nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer), &dw_size);
	RegCloseKey(h_key);
	if (l_res != ERROR_SUCCESS)
	{
		return "";
	}

	return { buffer };
}

bool run_win_rar(const std::string& winrar_path)
{
	duan da;
	// Xây dựng command line: "WinRAR.exe x -ibck "tệp.rar" "thư mục đích""
	std::string cmd_line = "\"" + winrar_path + "\" x -ibck -y \"" + da.tentep + "\" \"" + da.duongdan_giainen + "\"";

	STARTUPINFOA si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFOA);
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	// CreateProcess yêu cầu command line là một mảng char có thể sửa đổi, nên ta copy vào vector
	std::vector cmd_buffer(cmd_line.begin(), cmd_line.end());
	cmd_buffer.push_back('\0');  // Kết thúc chuỗi bằng ký tự null

	const BOOL result = CreateProcessA(nullptr,
									   cmd_buffer.data(),
									   nullptr,
									   nullptr,
									   FALSE,
									   0,
									   nullptr,
									   nullptr,
									   &si,
									   &pi
	);

	if (!result)
	{
		std::cerr << "CreateProcess failed with error: " << GetLastError() << std::endl;
		return false;
	}

	// Đợi cho tiến trình giải nén hoàn thành
	WaitForSingleObject(pi.hProcess, INFINITE);

	// Lấy mã kết thúc của tiến trình
	DWORD exit_code = 0;
	GetExitCodeProcess(pi.hProcess, &exit_code);

	// Đóng handle sau khi hoàn thành
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (exit_code == 0)
	{
		xoa_tapnen(da.tentep);
	}

	return (exit_code == 0);
}

