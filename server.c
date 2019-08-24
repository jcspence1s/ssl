#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "openssl/ssl.h"
#include "openssl/err.h"

int 
start_server(int port);

SSL_CTX *
SSL_init_CTX(void);

void
SSL_load_cert(SSL_CTX *ctx, char *cert, char *key);

void
SSL_coms(SSL *ssl);

int main(int argc, char **argv)
{
	if(argc == 2)
	{
		SSL_library_init();
		SSL_CTX *ctx = SSL_init_CTX();
		SSL_load_cert(ctx, "test_cert.pem", "test_cert.pem");
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

SSL_CTX *
SSL_init_CTX(void)
{
	const SSL_METHOD *method;
	SSL_CTX *ctx;

	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	method = TLSv1_2_server_method();
	ctx = SSL_CTX_new(method);
	if(NULL == ctx)
	{
		ERR_print_errors_fp(stderr);
		_exit(1);
	}
	return ctx;
}

void
SSL_load_cert(SSL_CTX *ctx, char *cert, char *key)
{
	if(0 >= SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM))
	{
		ERR_print_errors_fp(stderr);
		_exit(2);
	}
	if(0 >= SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM))
	{
		ERR_print_errors_fp(stderr);
		_exit(3);
	}
	if(!SSL_CTX_check_private_key(ctx))
	{
		fprintf(stderr, "Private and public key do not match.\n");
		_exit(4);
	}
}
void
SSL_coms(SSL *ssl)
{
	char buff[1024] = {0};
	int fd = 0;
	int bytes = 0;

	bytes = SSL_read(ssl, buff,  sizeof(buff));
	buff[bytes] = '\0';
	SSL_write(ssl, buff, strlen(buff));
	fd = SSL_get_fd(ssl);
	SSL_free(ssl);
	close(fd);
}
