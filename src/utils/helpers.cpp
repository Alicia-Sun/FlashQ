#include "helpers.h"

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

// TODO: for now it assumes config file is correct format, need to validate.
bool parse_config(const std::string& config_file, ServerConfig& server_config) {
    ifstream file(config_file);
    json j;
    file >> j;

    server_config = {j["queue_params"]["ip"], j["queue_params"]["port"]};
    return true;
}