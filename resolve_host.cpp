#include "myheader2.h"

std::string resolve_host_ipv4(std::string& hostname)
{
    int loop;                       // ループ用変数
    int ipaddr[ADDRESS_SIZE];       // IPv4格納配列
    struct hostent *host = NULL;    // ホスト情報
    std::stringstream ss;           // string stream

    host = gethostbyname(hostname.c_str());
    if(host == NULL)
    {
        std::cerr << "gethostbyname() failed: " << strerror(errno) << std::endl;
        return "";
    }

    if(host->h_length != 4)
    {
        std::cerr << "IPv6 address." << std::endl;
        return "";
    }

    for(loop = 0; loop < ADDRESS_SIZE; loop++)
    {
        ipaddr[loop] = (int)((unsigned char)*(host->h_addr_list[0] + loop));
    }

    // 
    for(loop = 0; loop < ADDRESS_SIZE - 1; loop++)
    {
        ss << ipaddr[loop] << ".";
    }

    ss << ipaddr[ADDRESS_SIZE-1];

    return ss.str();
}
