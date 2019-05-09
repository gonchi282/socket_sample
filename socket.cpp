#include "socket.h"

int main(int argc, char **argv)
{
    int send_size;                  // 書き込みサイズ
    int recv_size;                  // 受信サイズ
    int sock;                       // ソケットディスクリプタ
    int portNo = 443;                // ポート番号 
    std::string hostname;           // ホストネーム
    std::string ipaddr_str;         // IPアドレス
    std::string recvBuffer;         // 受信データ
    std::string path;               // リクエストするpath
    std::stringstream ss;           // stringstream
    struct sockaddr_in client_addr; // ソケットアドレス構造体
    in_addr_t addr;            // 第一引数がアドレスか判定用
    SSL *ssl;                       // SSL構造体
    SSL_CTX *ctx;
    

    // コマンド引数検証
    if(argc != 3)
    {
        fprintf(stderr, "Usage: %s <hostname> <request path>\n", argv[0]);
        return OK;
    }
    hostname = argv[1];     // コマンドライン第一引数をホストネーム
    path = argv[2];         // コマンドライン第二引数をリクエストパスに

    addr = inet_addr(hostname.c_str());
    if(addr == INADDR_NONE)
    {
        // hostnameがホストネームの時
        ipaddr_str = resolve_hostname(hostname);
    }
    else
    {
        // hostnameがIPアドレスの時
        ipaddr_str = hostname;
    }

    // ソケット生成
    if((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket() failed");
        exit(NG);
    }

    // sockaddr_in構造体に接続するアドレス・ポート番号を設定
    memset(&client_addr, 0, sizeof(struct sockaddr_in));            // 0クリアする
    client_addr.sin_family = AF_INET;                               // IPv4
    client_addr.sin_addr.s_addr = inet_addr(ipaddr_str.c_str());    
    client_addr.sin_port = htons(portNo);

    // ソケットをサーバーに接続
    if(connect(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) != 0)
    {
        perror("connect() failed");
        exit(NG);
    }
    std::cout << "connect to " << ipaddr_str << std::endl;

    // SSLセットアップ
    SSL_load_error_strings();
    SSL_library_init();

    ctx = SSL_CTX_new(SSLv23_client_method());
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);
    SSL_connect(ssl);

    ss << "GET /" << path << " HTTP/1.1\r\n";
    std::cout << ss.str();
    send_size = send_request_by_ssl(ssl, ss.str());
    ss.str("");
    ss << "HOST: " << hostname << "\r\n";
    std::cout << ss.str();
    send_size = send_request_by_ssl(ssl,ss.str());
    send_request_by_ssl(ssl, "Connection: Close\r\n\r\n");

    // レスポンスを受信
    do
    {
        recvBuffer = recv_response_by_ssl(ssl, BUFSIZE);
        recvBuffer[recvBuffer.length()] = '\0';
        printf("%s", recvBuffer.c_str());
    }while(recvBuffer.length() > 0);



    // SSL終了処理
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    ERR_free_strings();

    // ソケットを閉じる
    close(sock);

    return OK;
}

// リクエスト送信
int send_request(int sock, std::string sendMsg)
{
    int send_size;     // 書き込みサイズ
    
    send_size = send(sock, sendMsg.c_str(), sendMsg.length(), 0);
    if(send_size <= 0)
    {
        if(send_size < 0)
        {
            return SEND_ERR;    // エラー発生
        }
        return DNT_WRITE;       // 書き込まれていない
    }

    return send_size;  // 書き込んだサイズを返す
}
// リクエスト送信（SSL）
int send_request_by_ssl(SSL *ssl, std::string sendMsg)
{
    int send_size;

    send_size = SSL_write(ssl, sendMsg.c_str(), sendMsg.length());

    if(send_size <= 0)
    {
        if(send_size < 0)
        {
            return SEND_ERR;
        }
        return DNT_WRITE;
    }
    return send_size;
}
// 受信
std::string recv_response(int sock, int buf_size)
{
    std::string recvstr;
    char recvBuffer[buf_size];
    int recv_size;

    recv_size = recv(sock, recvBuffer, buf_size, 0);
    if(recv_size <= 0)
    {
        recvstr = "";
    }
    else
    {
        recvstr = recvBuffer;
    }
    return recvstr;
}
// 受信（SSL)
std::string recv_response_by_ssl(SSL *ssl, int buf_size)
{
    std::string recvstr;
    char recvBuffer[buf_size];
    int recv_size;

    recv_size = SSL_read(ssl, recvBuffer, buf_size);
    if(recv_size <= 0)
    {
        recvstr = "";
    }
    else
    {
        recvstr = recvBuffer;
    }
    return recvstr;
}
// 名前解決
std::string resolve_hostname(std::string hostname)
{
    struct hostent *host;
    struct sockaddr_in addr;
    std::string ipaddr;
    std::stringstream ss;

    host = gethostbyname(hostname.c_str());
    if(host == NULL)
    {
        ipaddr = "";
        return ipaddr;
    }

    addr.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);
    ipaddr = inet_ntoa(addr.sin_addr);

    return ipaddr;
}

