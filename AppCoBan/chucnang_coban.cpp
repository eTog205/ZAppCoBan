//chucnang_coban.cpp
#include "chucnang_coban.h"
#include "log_nhalam.h"

std::string thucthi_lenh(const std::string& lenh)
{
	std::ostringstream output;
	try
	{
		bool sha256_mismatch = false;
		bp::ipstream pipe_stream; // Luồng để nhận đầu ra từ lệnh
		bp::child process(lenh, bp::std_out > pipe_stream);
		std::string line;
		bool kotimthaygoi = false;

		while (pipe_stream && getline(pipe_stream, line))
		{
			output << line << "\n";

			if (line.find("No package found matching input criteria") != std::string::npos)
				kotimthaygoi = true;

			if (line.find("Installer hash does not match") != std::string::npos)
				sha256_mismatch = true;
		}

		process.wait(); // Chờ lệnh kết thúc

		if (kotimthaygoi)
		{
			return "Package not found";
		}

		if (sha256_mismatch)
		{
			td_log(loai_log::canh_bao, "phần mềm được chọn gặp lỗi (sha256_mismatch) cần đợi một thời gian mới có thể cài đặt)");
		}

		if (process.exit_code() != 0)
		{
			throw std::runtime_error("Lenh loi voi ma thoat: " + std::to_string(process.exit_code()));
		}
	} catch (const std::exception& ex)
	{
		td_log(loai_log::loi, "Lỗi xảy ra: " + std::string(ex.what()));
	}

	return output.str();
}

void chaylenh(const std::string& id, const std::string& tuychon_them)
{
	std::thread([id, tuychon_them]()
	{
		const std::string tuychon_macdinh = " --silent --accept-package-agreements --accept-source-agreements --disable-interactivity";
		const std::string lenh = "winget install " + id + tuychon_macdinh + tuychon_them;

		thucthi_lenh(lenh);
	}).detach();
}


