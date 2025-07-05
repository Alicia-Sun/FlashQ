#pragma once
#include <string>
#include <unordered_map>
#include <fstream>

struct ServerConfig {
    int id;
    std::string ip_address;
    int port;
    bool is_primary;
};

bool parse_config(int id, const std::string& config_file, std::unordered_map<int, ServerConfig>& server_configs);