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

	SSL_accept(ssl);
	printf("SSL %sNULL\n", ssl ? "is not " : "is ");
	bytes = SSL_read(ssl, buff,  sizeof(buff));
	buff[bytes] = '\0';
	printf("Recieved: %s\n", buff);
	SSL_write(ssl, buff, strlen(buff));
	fd = SSL_get_fd(ssl);
	SSL_free(ssl);
	close(fd);
}

SSL_CTX *
SSL_init_client_CTX(void)
{
	const SSL_METHOD *method;
	SSL_CTX *ctx;

	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	method = TLSv1_2_client_method();
	ctx = SSL_CTX_new(method);
	if(NULL == ctx)
	{
		ERR_print_errors_fp(stderr);
		_exit(1);
	}
	return ctx;
}
