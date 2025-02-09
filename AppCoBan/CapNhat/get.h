//get.h
#pragma once

#include <boost/asio/ssl/context.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>
#include <string>

namespace beast = boost::beast;           // Boost.Beast
namespace http = beast::http;             // HTTP from Beast
namespace net = boost::asio;              // Boost.Asio
namespace ssl = net::ssl;                 // SSL/TLS
namespace json = boost::json;             // Boost.JSON
using tcp = net::ip::tcp;

struct duan
{
	std::string sohuu = "eTog205";
	std::string kho = "SuaKeyTepApp";
};

std::string send_http_request(const std::string& url, bool is_asset = false, int redirect_count = 0);

// 🔹 Phân tích phản hồi JSON
json::value parse_json_response(const std::string& response_body);

// Hàm getReleaseTag(): lấy tag name của bản phát hành từ GitHub API
std::string get_release_tag();

//bool download_file(const std::string& url, const std::string& save_path);
bool download_latest_release();

bool download_file(const std::string& url, const std::string& save_path);