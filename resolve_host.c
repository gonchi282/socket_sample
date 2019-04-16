#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>

#define IPV4_LENGTH   16

int main(int argc, char **argv)
{
    char *hostname;                  // ホストネーム
    char ipv4[IPV4_LENGTH];          // IPアドレス

    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s [hostname]", argv[0]);
        return -1;
    }

    hostname = argv[1];

    resolve_host_ipv4(hostname, ipv4);

    printf("%s: %s\n",hostname, ipv4);

    return 0;
}

int resolve_host_ipv4(const char *hostname, char *ipv4)
{
    struct hostent *host = NULL;    // ホスト情報

    if ((hostname == NULL) || (ipv4 == NULL))
    {
        return -1;  // 異常終了
    }

    host = gethostbyname(hostname);
    if(host == NULL)
    {
        fprintf(stderr, "gethostbyname() failed: %s\n", strerror(errno));
        return -1;  // 異常終了
    }

    if(host->h_length != 4)
    {
        fprintf(stderr, "IPv6 address.\n");
        return -1;  // 異常終了
    }

    sprintf(ipv4, "%d.%d.%d.%d", (unsigned char)*((host->h_addr_list[0])), (unsigned char)*((host->h_addr_list[0] + 1)), (unsigned char)*((host->h_addr_list[0] + 2)), (unsigned char)*((host->h_addr_list[0] + 3)));
    return 0;   // 正常終了
}
