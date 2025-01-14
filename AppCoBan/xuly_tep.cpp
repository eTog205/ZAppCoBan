#include "nlohmann/json.hpp"
#include "xuly_tep.h"

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <unordered_set>

// Biến lưu danh sách key hoặc node mục tiêu
static unordered_set<string> targetKeys;

// Hàm cắt khoảng trắng thừa
string trim(const string& str)
{
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

// Hàm phân tách chuỗi theo dấu phẩy
vector<string> split_string(const string& str, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream token_stream(str);
    while (getline(token_stream, token, delimiter))
    {
        tokens.push_back(trim(token));
    }
    return tokens;
}

// Nạp tệp cấu hình
void load_config_file(const string& config_path)
{
    ifstream config_file(config_path);
    if (!config_file.is_open())
    {
        cerr << "Failed to open configuration file: " << config_path << endl;
        return;
    }

    string line;
    while (getline(config_file, line))
    {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue; // Bỏ qua dòng trống hoặc comment

        // Xử lý target_keys
        regex target_keys_regex(R"(target_keys:\s*(.*))");
        smatch match;
        if (regex_search(line, match, target_keys_regex))
        {
            string keys = match[1];
            if (!keys.empty())
            {
                vector<string> key_list = split_string(keys, ',');
                targetKeys.insert(key_list.begin(), key_list.end());
                cout << "Target keys loaded: ";
                for (const auto& key : key_list)
                {
                    cout << key << " ";
                }
                cout << endl;
            } else
            {
                cout << "No target keys specified. Processing all keys by default." << endl;
            }
        }
    }

    config_file.close();
}

// Xử lý tệp JSON
void process_json_file(const string& file_path)
{
    ifstream file(file_path);
    if (!file.is_open())
    {
        cerr << "Failed to open file: " << file_path << endl;
        return;
    }

    nlohmann::json json_data;
    file >> json_data;

    // Lọc dữ liệu theo target_keys
    cout << "Processing JSON file with target keys:" << endl;
    if (!targetKeys.empty())
    {
        for (const auto& key : targetKeys)
        {
            if (json_data.contains(key))
            {
                cout << key << ": " << json_data[key] << endl;
            } else
            {
                cout << key << ": Not found" << endl;
            }
        }
    } else
    {
        for (auto it = json_data.begin(); it != json_data.end(); ++it)
        {
            cout << it.key() << ": " << it.value() << endl;
        }
    }
}

// Xử lý tệp được kéo thả
void process_file(const string& file_path)
{
    string extension = file_path.substr(file_path.find_last_of('.') + 1);
    if (extension == "json")
    {
        process_json_file(file_path); // Gọi hàm xử lý JSON
        //} else if (extension == "xml") {
        //    processXmlFile(filePath); // Gọi hàm xử lý XML
    } else
    {
        std::cerr << "Unsupported file format: " << extension << std::endl;
    }
}