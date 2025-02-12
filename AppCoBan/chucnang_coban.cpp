//chucnang_coban.cpp
#include "chucnang_coban.h"
#include "log_nhalam.h"

std::string thucthi_lenh(const std::string& lenh)
{
	std::ostringstream output;

	try
	{
		bp::ipstream pipe_stream; // Luồng để nhận đầu ra từ lệnh
		bp::child process(lenh, bp::std_out > pipe_stream);
		std::string line;
		bool kotimthaygoi = false;

		while (pipe_stream && getline(pipe_stream, line))
		{
			output << line << "\n";

			if (line.find("No package found matching input criteria") != std::string::npos)
			{
				kotimthaygoi = true;
			}
		}

		process.wait(); // Chờ lệnh kết thúc

		if (kotimthaygoi)
		{
			return "Package not found";
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
	// Các tùy chọn mặc định cho winget
	const std::string tuychon_macdinh = " --silent --accept-package-agreements --accept-source-agreements --disable-interactivity ";

	const std::string lenh = "winget install " + id + tuychon_macdinh + tuychon_them;

	thucthi_lenh(lenh);
}