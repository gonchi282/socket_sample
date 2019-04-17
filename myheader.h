#ifndef MYHEADER_H
#define MYHEADER_H

#include <stdio.h>          // printf(), fprintf(), perror()
#include <stdlib.h>         // atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>         // memset()
#include <errno.h>          // errno

#ifdef __linux__
#include <sys/socket.h>     // socket(), connect(), recv()
#include <arpa/inet.h>      // struct sockaddr_in, struct sockaddr, inet_ntoa(), inet_aton() 
#include <unistd.h>         // close()
#include <netdb.h>          // gethostbyname()
#endif

#ifdef __WIN32
#include <winsock2.h>
#endif

#define MSGSIZE 256
#define MAX_MSGSIZE 1024
#define BUFSIZE (MSGSIZE + 1)
#define IPSIZE  16
#define HOSTNAMESIZE 256

typedef struct Request
{
    char path[MSGSIZE]; // 取得ファイルパス
    char host[MSGSIZE]; // ホストネーム
    char connection[MSGSIZE];   // 応答送信後にTCPコネクションをどうするか
    char if_Modified_Since[MSGSIZE];    // ここに指定された日付以降に更新された情報のみを用空
    char referer[MSGSIZE];  // 現在のページを取得するときにユーザーが使ったリンクを含むページURL
    char accept[MSGSIZE];   // ブラウザが欲しいMIMEタイプ
    char accept_encoding[MSGSIZE];  // ブラウザがデコードできるエンコーディング
    char accept_language[MSGSIZE];  // ブラウザが予期している言語
}REQUEST; 

typedef enum Reqtype
{
    COMMAND = 0,
    HOST,
    USERAGENT,
    CONTENTTYPE,
}REQTYPE;

// 関数のプロトタイプ宣言
int resolve_host_ipv4(const char *, char *);
int send_request(int, REQUEST*, REQTYPE);

#endif
