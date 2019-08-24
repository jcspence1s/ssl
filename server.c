#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "ssl_lib.h"
#include "connection.h"


int main(int argc, char **argv)
{
	if(argc == 2)
	{
		SSL_library_init();
		SSL_CTX *ctx = SSL_init_CTX();
		SSL_load_cert(ctx, "../test_cert.pem", "../test_cert.pem");
		int socket = start_server(atoi(argv[1]));
		while(1)
		{
			struct sockaddr_in address;
			socklen_t len = sizeof(address);
			SSL *ssl;

			printf("Waiting for connection on port %d\n", atoi(argv[1]));
			int client_fd = accept(socket, (struct sockaddr*)&address, &len);
			ssl = SSL_new(ctx);
			SSL_set_fd(ssl, client_fd);
			SSL_coms(ssl);
		}
		close(socket);
		SSL_CTX_free(ctx);
	}
	else
	{
		fprintf(stderr, "./server <port number>\n");
	}
}
