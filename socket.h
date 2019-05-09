#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

#define BUFSIZE 1
#define IPSIZE 16
#define OK 0
#define NG -1

#define DNT_WRITE 0
#define SEND_ERR -1


int send_request(int, std::string);
int send_request_by_ssl(SSL *, std::string);
std::string recv_response(int, int);
std::string recv_response_by_ssl(SSL *, int);
std::string resolve_hostname(std::string);


