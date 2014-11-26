#ifndef QUERY_SERVER_H
#define QUERY_SERVER_H 
#include <echo/TcpServer.h>
#include <echo/ThreadPool.h>
#include "TextQuery.h"

class QueryServer : NonCopyable
{
    public:
        QueryServer(const InetAddress &addr, 
                    const std::string &Chinesefilename,
                    const std::string &Englishfilename);
        void start();
    private:
        void onConnection(const TcpConnectionPtr &conn);
        void onMessage(const TcpConnectionPtr &conn);

        void runQuery(const std::string &word, const TcpConnectionPtr &conn);

        TcpServer server_;
        TextQuery query_;
        ThreadPool pool_;
};



#endif  /*QUERY_SERVER_H*/
