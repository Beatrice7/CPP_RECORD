#include "QueryServer.h"
using namespace std;
using namespace std::placeholders;


QueryServer::QueryServer(const InetAddress &addr, 
                         const string &Chinesefilename,
                         const string &Englishfilename)
        :server_(addr),
         query_(Chinesefilename, Englishfilename),
         pool_(1000, 4)
{
    server_.setConnection(bind(&QueryServer::onConnection, this, _1));
    server_.setMessage(bind(&QueryServer::onMessage, this, _1));
}

void QueryServer::start()
{
    pool_.start();
    server_.start();
}



void QueryServer::onConnection(const TcpConnectionPtr &conn)
{
    conn->send("welcome, please input word:\r\n");
}

void QueryServer::onMessage(const TcpConnectionPtr &conn)
{
    string word(conn->receive());
    pool_.addTask(bind(&QueryServer::runQuery, this, word, conn));
}


void QueryServer::runQuery(const string &s, const TcpConnectionPtr &conn)
{
    string word = s;
    if(word.substr(word.size()-2, 2) == "\r\n")
    {
            word.erase(word.size()-1);
            word.erase(word.size()-1);
        }
    string res = query_.runQuery(word);
    conn->send(res + "\r\n");
}
