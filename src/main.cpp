#include "networking/connection.h"
#include "nodes/queue.h"
#include <unistd.h>
#include <string>
#include <thread>
#include <iostream>

using namespace std;

QueueNode server;
int server_id;
std::string config_filename;

void handle_client(int id) {
    while (1) {
        // TODO:
    }
}

// ex. "./main -i 1 configs/config1.json"
int main(int argc, char *argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "i:")) != -1) {
        switch (opt) {  
            case 'i':
                // pulling node id
                server_id = atoi(optarg);
                break;        

            case '?':
                if (isprint(optopt)) {
                    fprintf(stdout, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stdout, "Unknown option character `\\x%x'.\n", optopt);
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
    int init_status = server.init(server_id, 0, config_filename);
    if (init_status < 0) {
        perror("Error: Failed to initialize QueueNode");
        return -1;
    }

    int server_fd = Connection::listen_on(server.server_configs[server_id].port);
    while (true) {
        int client_fd = Connection::accept_client(server_fd);
        // TODO: must case over type of node and implement different handlers
        std::thread(handle_client, client_fd).detach();
    }
    return 0;
}