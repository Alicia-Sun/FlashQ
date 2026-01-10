#include "helpers.h"

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// TODO: for now it assumes config file is correct format, need to validate.
bool parse_config(int id, const std::string& config_file, unordered_map<int, 
    ServerConfig>& server_configs
) {
    ifstream file(config_file);
    json j;
    file >> j;

    for (auto& node : j["queue_nodes"]) {
        if (server_configs.find(node["id"]) != server_configs.end()) {
            // repeated node id, return error
            return false;
        }
        ServerConfig new_node_config {node["id"], node["ip"], node["port"], 
            node["is_primary"]};
        server_configs[node["id"]] = new_node_config;
    }
    return true;
}