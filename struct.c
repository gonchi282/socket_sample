#include "myheader.h"

typedef struct ReqMessage
{
    int alloc_size;
    char *data;
}REQMESSAGE;

// リクエストメッセージセット
int set_reqmessage(REQMESSAGE *reqmessage, const char *message)
{
    // NULLチェック
    if((reqmessage == NULL) || (message == NULL))
    {
        return -1;  // 異常終了
    }

    // データが入っている場合
    if(reqmessage->data != NULL)
    {
        // メモリ解放
        destroy_reqmessage_data(reqmessage);
    }

    // メモリ確保
    alloc_size = strlen(message) + 1;
    req->data = (char *)malloc(sizeof(alloc_size));
    if(data == NULL)
    {
        // メモリ確保失敗
        return -1;  // 異常終了
    }
    // メッセージ格納
    strcpy(req->data, message);

    return 0;   // 正常終了
}

int destroy_reqmessage_data(REQMESSAGE *reqmessage)
{
    // NULLチェック
    if(reqmessage == NULL)
    {
        return -1;  // 異常終了
    }

    reqmessage->alloc_size = 0;
    free(reqmessage->data);
    reqmessage->data = NULL;

    return 0;   // 正常終了
}

// リクエストメッセージ構造体メモリ解放
int destroy_reqmessage(REQMESSAGE *reqmessage)
{
    // NULLチェック
    if(reqmessage == NULL)
    {
        return -1;
    }

    // データが入っている場合
    reqmessage->alloc_size = 0;
    free(reqmessage->data);
    reqmessage->data = NULL;
    free(reqmessage);
    reqmessage = NULL;

    return 0;   // 正常終了
}
