#include "chucnang_coban.h"
#include "log_nhalam.h"
#include "tienich.h"

#include <shellapi.h>


bool launch_updater()
{
#ifdef _DEBUG
	return false;

#else
	// Đường dẫn tương đối đến updater.exe (nằm trong thư mục con "CapNhat" của working directory hiện tại)
	const auto updater_relative_path = L"CapNhat\\CapNhat.exe";
	HINSTANCE result = ShellExecute(nullptr, L"open", updater_relative_path, nullptr, nullptr, SW_HIDE);

	// Kiểm tra kết quả:
	// ShellExecute trả về giá trị > 32 nếu thành công, giá trị <= 32 báo hiệu lỗi.
	if (reinterpret_cast<intptr_t>(result) <= 32)
	{
		DWORD errorCode = GetLastError();
		td_log(loai_log::loi, "Không thể khởi chạy updater. Mã lỗi: " + std::to_string(reinterpret_cast<intptr_t>(result)) +
			   ", GetLastError: " + std::to_string(errorCode));
		return false;
	}

	td_log(loai_log::thong_bao, "Khởi chạy updater thành công.");
	return true;

#endif
}
