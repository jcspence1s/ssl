#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "connection.h"

int start_server(int port)
{
    int opt = 1;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
				&opt, sizeof(opt)) != 0)
    {
        perror("Unable to set socket options.\n");
        exit(1);
    }
    struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd, (struct sockaddr *) &address, sizeof(address)) != 0)
    {
        perror("Unable to bind socket.\n");
        exit(1);
    }
    listen(fd, 2);
	return fd;
}
