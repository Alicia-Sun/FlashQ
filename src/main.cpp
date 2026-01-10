#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>

#include "networking/connection.h"
#include "queue/queue.h"

using namespace std;

FlashQ server;

int queue_size;
int payload_size_code;
PayloadSize max_payload_size;
std::string config_filename;

// Requires 4 arguments
// -n                   : queue size (number of messages it can hold)
// -s                   : payload size which is based on enums 0-3 representing 
// 1, 4, 16, and 64 KB
// <config_filename>    : path to config JSON file
// ex. "./main -n 5 -s 3 configs/config1.json"
int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "n:s:")) != -1) {
        switch (opt) {  
                
            case 'n':
                // queue size (num msgs)
                queue_size = atoi(optarg);
                break;        
            
            case 's':
                // payload size (enum)
                payload_size_code = atoi(optarg);
                max_payload_size = parse_payload_size(payload_size_code);
                break;     

            case '?':
                if (isprint(optopt)) {
                    fprintf(stdout, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stdout, "Unknown option character `\\x%x'.\n", 
                        optopt);
                }
                exit(EXIT_FAILURE);

            default:
                abort();
        }
    }

    if (optind == argc) {
		fprintf(stdout, "Error: Missing required arguments\n");
		exit(1);
	}
	config_filename = argv[optind];
    int init_status = server.init(queue_size, 
        static_cast<uint64_t>(max_payload_size), config_filename);
    if (init_status < 0) {
        perror("Error: Failed to initialize FlashQ");
        return -1;
    }

    int server_fd = Connection::listen_on(server.server_config.port);
    while (true) {
        int client_fd = Connection::accept_client(server_fd);
        std::thread(&FlashQ::handle_client, &server, client_fd).detach();
    }
    return 0;
}