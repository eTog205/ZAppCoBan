#pragma once


#include <string>
#include <vector>

using namespace std;

string trim(const string& str);
vector<string> split_string(const string& str, char delimiter);
void load_config_file(const string& config_path);
void process_json_file(const string& file_path);
void process_file(const string& file_path);

