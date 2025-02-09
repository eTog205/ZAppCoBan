
#include "chucnang_coban.h"
#include "log_nhalam.h"
#include "xuly_thongso_cuaso.h"
#include <shellapi.h>


#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")


bool LaunchUpdater()
{
	// Đường dẫn tương đối đến updater.exe (nằm trong thư mục con "CapNhat" của working directory hiện tại)
	const auto updater_relative_path = L"CapNhat\\CapNhat.exe";

	// Gọi ShellExecute để mở CapNhat.exe.
	// Các tham số:
	// - hwnd: NULL (không có cửa sổ cha)
	// - lpOperation: "open" (mở file)
	// - lpFile: đường dẫn đến file thực thi CapNhat.exe
	// - lpParameters: NULL (không truyền tham số dòng lệnh)
	// - lpDirectory: NULL (sử dụng thư mục làm việc hiện tại)
	// - nShowCmd: SW_SHOWNORMAL (hiển thị cửa sổ bình thường)
	HINSTANCE result = ShellExecute(nullptr, L"open", updater_relative_path, nullptr, nullptr, SW_HIDE);

	// Kiểm tra kết quả:
	// ShellExecute trả về giá trị > 32 nếu thành công, giá trị <= 32 báo hiệu lỗi.
	if (reinterpret_cast<intptr_t>(result) <= 32)
	{
		td_log(loai_log::loi, "Không thể khởi chạy updater. Mã lỗi: " + to_string(reinterpret_cast<intptr_t>(result)));
		return false;
	}
	return true;
}

int main()
{
	GLFWwindow* cuaso = khoitao_cuaso();
	if (!cuaso)
		return 1;

	vonglap_chinh(cuaso);

	dondep(cuaso);

}