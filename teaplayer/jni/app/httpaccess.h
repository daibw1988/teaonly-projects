#ifndef _HTTPACCESS_H_
#define _HTTPACCESS_H_

#include "access.h"
#include "talk/base/thread.h" 
#include "talk/base/messagequeue.h"

namespace talk_base{
    class AsyncSocket;
};

class HttpAccess : public TeaAccess, talk_base::MessageHandler{ 
public:
    HttpAccess(const std::string ipaddr);
    virtual ~HttpAccess();

    virtual bool Open();
    virtual void Close();

protected:
    void OnMessage(talk_base::Message *msg);
    void OnConnectEvent(talk_base::AsyncSocket* socket);
    void OnCloseEvent(talk_base::AsyncSocket* socket, int err);
    void OnReadEvent(talk_base::AsyncSocket* socket);

private:
    enum {
        MSG_ACCESS_OPEN,
    };
    void httpQuery();
    void doConnect();
    void createSocket();

private:
    talk_base::Thread *thread;
    talk_base::AsyncSocket* http_socket_;
    bool is_ready_;
    std::string target_ipaddr;
};

#endif
