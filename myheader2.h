#ifndef MYHEADER_H
#define MYHEADER_H

#include <stdio.h>          // printf(), fprintf(), perror()
#include <iostream>         // I/O stream
#include <sstream>          // String stream
#include <stdlib.h>         // atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h>         // memset()
#include <errno.h>          // errno

#ifdef __linux__
#include <sys/socket.h>     // socket(), connect(), recv()
#include <arpa/inet.h>      // struct sockaddr_in, struct sockaddr, inet_ntoa(), inet_aton() 
#include <unistd.h>         // close()
#include <netdb.h>          // gethostbyname()
#include <openssl/ssl.h>    // openssl
#include <openssl/err.h>    // openssl error
#endif

#ifdef __WIN32
#include <winsock2.h>
#endif

#define MSGSIZE 255
#define MAX_MSGSIZE 1024
#define BUFSIZE (MSGSIZE + 1)
#define ADDRESS_SIZE 4

// リクエスト構造体
class Request
{
public:
    std::string path; // 取得ファイルパス
    std::string host; // ホストネーム
    std::string connection;   // 応答送信後にTCPコネクションをどうするか
    std::string if_Modified_Since;    // ここに指定された日付以降に更新された情報のみを用空
    std::string referer;  // 現在のページを取得するときにユーザーが使ったリンクを含むページURL
    std::string accept;   // ブラウザが欲しいMIMEタイプ
    std::string accept_encoding;  // ブラウザがデコードできるエンコーディング
    std::string accept_language;  // ブラウザが予期している言語
}; 


// リクエストタイプ
typedef enum Reqtype
{
    METHOD = 0,        // メソッド
    HOST,               // ホストネーム
    USERAGENT,          // ユーザーエージェント
    CONTENTTYPE,        // コンテントタイプ
}REQTYPE;


// 関数のプロトタイプ宣言
std::string resolve_host_ipv4(std::string&);
int send_request(int, Request&, REQTYPE);
#endif
