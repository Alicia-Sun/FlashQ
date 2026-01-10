#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

struct ServerConfig {
    int id;
    std::string ip_address;
    int port;
    bool is_primary;
};

// Parses config_file (.json) for system configuration for server params.
// Returns through parameters a map of all node ids -> server_configs.
bool parse_config(int id, const std::string& config_file, 
    std::unordered_map<int, ServerConfig>& server_configs
);