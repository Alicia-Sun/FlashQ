#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

struct ServerConfig {
    std::string ip_address;
    int port;
};

// Parses config_file (.json) for server params. Returns through parameter 
// server_config.
bool parse_config(const std::string& config_file, ServerConfig& server_configs);