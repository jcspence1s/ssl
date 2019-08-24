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
	if(3 == argc)
	{
		SSL_CTX *ctx;
		int socket = 0;
		SSL *ssl;
		char *input = NULL;
		char *server_ip = argv[1];
		int port = atoi(argv[2]);
		SSL_library_init();
		ctx = SSL_init_client_CTX();
		socket = connect_server(server_ip, port);
		ssl = SSL_new(ctx);
		SSL_set_fd(ssl, socket);
		if(-1 == SSL_connect(ssl))
		{
			fprintf(stderr, "Could not connect to server\n");
			_exit(1);
		}
		size_t len = 0;
		getline(&input, &len, stdin);
		int length = SSL_write(ssl, input, strlen(input));
		printf("Receiving:\n");
		free(input);
		input = calloc(length + 1, sizeof(char));
		SSL_read(ssl, input, length);
		printf("%s\n", input);
		free(input);
		SSL_free(ssl);
		close(socket);
		SSL_CTX_free(ctx);
	}
	return 0;
}
