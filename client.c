#include "myheader.h"

int main(int argc, char *argv[]){

    SSL *ssl;                           // SSL
    SSL_CTX *ctx;                       // SSL_CTX
    REQUEST req;                        // リクエスト
    int sock;                           // socketディスクリプタ
    int status;                         // 変数ステータス
    struct sockaddr_in servSockAddr;    // ソケットアドレス構造体
    unsigned short servPort;            // サーバーポート番号
    char recvBuffer[BUFSIZE];           // 受信一時バッファ
    char sendBuffer[BUFSIZE];           // 送信一時バッファ
    char ipv4address[IPSIZE];           // IPv4アドレス文字列の格納先
    char hostname[HOSTNAMESIZE];        // ホストネーム
    char reqmsg[MAX_MSGSIZE];           // リクエストメッセージ
    int byteRcvd, totalBytesRcvd;       // 受信バッファサイズ

    // 引数を数える
    if(argc != 3){
        fprintf(stderr, "argument count mismatch error.\n");
        exit(EXIT_FAILURE);
    }

    // servSockAddrを0クリアする
    memset(&servSockAddr, 0, sizeof(servSockAddr));
    // IPv4プロトコルを使用する
    servSockAddr.sin_family = AF_INET;

    // ホストネームを解決する
    strcpy(hostname, argv[1]);
    status = resolve_host_ipv4(hostname, ipv4address); 
    // IPアドレスをネットワーク数にする
    if(inet_aton(ipv4address, &servSockAddr.sin_addr) == 0){
        fprintf(stderr, "Invalid IP Address.\n");
        exit(EXIT_FAILURE);
    }
    // サーバーポートを整数型にする
    servPort = (unsigned short)strtol(argv[2], NULL, 0);
    if(servPort == 0){
        fprintf(stderr, "Invalid port number.\n");
        exit(EXIT_FAILURE);
    }
    servSockAddr.sin_port = htons(servPort);

    // ソケット生成
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("socket() failed.\n");
        exit(EXIT_FAILURE);
    }

    // 接続
    if(connect(sock, (struct sockaddr *) &servSockAddr, sizeof(servSockAddr)) < 0){
        perror("connect() failed.\n");
        exit(EXIT_FAILURE);
    }

    // 
    printf("connect to %s\n", inet_ntoa(servSockAddr.sin_addr));

    strcpy(req.host, hostname);
    strcpy(req.path, "index.html");

    send_request(sock, &req, COMMAND);
    send_request(sock, &req, HOST);

    totalBytesRcvd = 0;
    while(totalBytesRcvd < MAX_MSGSIZE){
        if((byteRcvd = recv(sock, recvBuffer, MSGSIZE, 0)) > 0){
            recvBuffer[byteRcvd] = '\0';
            printf("%s", recvBuffer);
            totalBytesRcvd += byteRcvd;
        }else if(byteRcvd == 0){
            perror("ERR_EMPTY_RESPONSE");
            fprintf(stderr, "errno: %d", errno);
            exit(EXIT_FAILURE);
        }else{
            perror("recv() failed.");
            exit(EXIT_FAILURE);
        }
    }

    printf("\n");
    close(sock);

    return EXIT_SUCCESS;
}
/* url: https://qiita.com/tajima_taso/items/fb5669ddca6e4d022c15/ */

int send_request(int sock, REQUEST* req, REQTYPE reqtype)
{
    char sendBuffer[MSGSIZE];

    switch(reqtype)
    {
    case COMMAND:
        sprintf(sendBuffer, "GET /%s HTTP/1.1\r\n", req->path);
        break;
    case HOST:
        sprintf(sendBuffer, "HOST: %s\r\n\r\n", req->host);
        break;
    default:
        return -1;
    }
    // 送信
    send(sock, sendBuffer, strlen(sendBuffer), 0);
    printf("%s", sendBuffer);

    return 0;
}
