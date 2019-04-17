#include "myheader.h"
#define ADDRESS_SIZE 4

int resolve_host_ipv4(const char *hostname, char *ipv4)
{
    int i;
    int ipaddr[ADDRESS_SIZE];
    struct hostent *host = NULL;    // ホスト情報

    if (hostname == NULL)
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

    for (i = 0; i < ADDRESS_SIZE; i++)
    {
        ipaddr[i] = (int)((unsigned char)*(host->h_addr_list[0] + i));
    }

    sprintf(ipv4, "%d.%d.%d.%d", ipaddr[0], ipaddr[1], ipaddr[2], ipaddr[3]);

//    sprintf(ipv4, "%d.%d.%d.%d", (int)((unsigned char)*((host->h_addr_list[0]))), (int)((unsigned char)*((host->h_addr_list[0] + 1))), (int)((unsigned char)*((host->h_addr_list[0] + 2))), (int)((unsigned char)*((host->h_addr_list[0] + 3))));
    return 0;   // 正常終了
}
