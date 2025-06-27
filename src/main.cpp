#include "networking/connection.h"
#include "nodes/queue.h"
#include <unistd.h>
#include <string>
#include <thread>

std::string config_filename;
int port;

void handle_client(int id) {
    while (1) {
        // TODO:
    }
}

int main(int argc, char *argv[]) {
    int opt;

    while ((opt = getopt(argc, argv, "cpqP:")) != -1) {
        switch (opt) {
            case 'c':
                fprintf(stderr, "Starting consumer node\n");
                break;
            case 'p':
                fprintf(stdout, "Starting producer node\n");
                break;
            case 'q':
                fprintf(stdout, "Starting queue node\n");
                break;
            case 'P':
                // temporary method of pulling port
                port = atoi(optarg);
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


    int server_fd = Connection::listen_on(port);
    while (true) {
        int client_fd = Connection::accept_client(server_fd);
        // TODO: must case over type of node and implement different handlers
        std::thread(handle_client, client_fd).detach();
    }
    return 0;
}