#ifndef SSL_LIB_H
#define SSL_LIB_H

#include "openssl/ssl.h"
#include "openssl/err.h"
SSL_CTX *
SSL_init_CTX(void);

void
SSL_load_cert(SSL_CTX *ctx, char *cert, char *key);

void
SSL_coms(SSL *ssl);

#endif
