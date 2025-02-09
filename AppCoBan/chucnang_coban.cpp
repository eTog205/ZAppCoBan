#include "chucnang_coban.h"

#include <iostream>

std::string thucthi_lenh(const std::string& command)
{
	std::ostringstream output;

	try
	{
		bp::ipstream pipe_stream; // Luồng để nhận đầu ra từ lệnh
		bp::child process(command, bp::std_out > pipe_stream);
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
			// cái này không hợp cần đổi
			return "Package not found";
		}

		if (process.exit_code() != 0)
		{
			// cái này không hợp cần đổi
			throw std::runtime_error("Lenh loi voi ma thoat: " + std::to_string(process.exit_code()));
		}
	} catch (const std::exception& ex)
	{
		//std::cerr << "Error executing command: " << ex.what() << std::endl;
		throw;
	}

	return output.str();
}

void chaylenh(const std::string& id)
{
	thucthi_lenh("winget install " + id);
}