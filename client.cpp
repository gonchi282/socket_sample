#include "myheader2.h"

int recv_res(int, std::string&, size_t, int);
int main(int argc, char *argv[])
{
    SSL *ssl;               // SSL
    SSL_CTX *ctx;           // SSL_CTX
    Request req;            // request
    int sock;               // socketディスクリプタ
    struct sockaddr_in servSockAddr;    // ソケットアドレス構造体
    unsigned short servPort;            // サーバーポート
    std::string recvBuffer;             // 受信一時バッファ
    std::string sendBuffer;             // 送信一時バッファ
    std::string ipv4address;            // IPv4アドレス文字列の格納先
    std::string hostname;               // ホストネーム
    std::string reqmsg;                 // リクエストメッセージ
    std::stringstream ss;               // ストリングストリーム
    int byteRcvd, totalBytesRcvd;       // 受信バッファサイズ

    // 引数を数える
    if(argc != 3)
    {
        std::cerr << "argument count mismatch error." << std::endl;
        exit(EXIT_FAILURE);
    }

    hostname = argv[1];     // ホストネームを代入

    // servSockAddrを0クリアする
    memset(&servSockAddr, 0, sizeof(servSockAddr));
    // IPv4プロトコルを使用する
    servSockAddr.sin_family = AF_INET;

    // ホストネームを解決する
    ipv4address = resolve_host_ipv4(hostname);
    if(ipv4address == "")
    {
        perror("resolve_host_ipv4() failed");
        exit(EXIT_FAILURE);
    }
    // IPアドレスをネットワーク数にする
    if(inet_aton(ipv4address.c_str(), &servSockAddr.sin_addr) == 0)
    {
        perror("Invalid IP Address.");
        exit(EXIT_FAILURE);
    }
    // サーバーポートを整数型にする
    servPort = static_cast<unsigned short>(std::atoi(argv[2]));
    if(servPort == 0)
    {
        perror("Invalid Port.\n");
        exit(EXIT_FAILURE);
    }
    servSockAddr.sin_port = htons(servPort);

    // ソケット生成（IPv4, TCP使用）
    if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        perror("socket() failed.\n");
        exit(EXIT_FAILURE);
    }
    std::cout << inet_ntoa(servSockAddr.sin_addr) << std::endl;

    // 接続
    if(connect(sock, (struct sockaddr *) &servSockAddr, sizeof(servSockAddr)) < 0)
    {
        perror("connect() failed.\n");
        exit(EXIT_FAILURE);
    }

    std::cout << "connect to " << inet_ntoa(servSockAddr.sin_addr) << std::endl;

    // リクエスト設定
    req.host = ss.str();
    req.path = "index.html";

    ss << "GET /" << req.path.c_str() << " HTTP/1.1\r\n";
    send(sock, ss.str().c_str() ,ss.str().size(), 0);
    ss.str("");
    ss.clear(std::stringstream::goodbit);
    ss << "HOST: " << hostname.c_str() << "\r\n\r\n";
    send(sock, ss.str().c_str(), ss.str().size(), 0);

    do
    {
        byteRcvd = recv_res(sock, recvBuffer, 30, 0);
        std::cout << recvBuffer;
    }while(byteRcvd == 30);

    
    close(sock);

    return 0;
}

int recv_res(int sock, std::string& sendBuffer, size_t size, int flag)
{
    char buffer[size];
    int recvsize;

    recvsize = recv(sock, buffer, size, flag);

    sendBuffer = buffer;

    return recvsize;
}
    
    
