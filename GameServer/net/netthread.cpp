
#include "netthread.h"
#include "callback.h"
#include "lib/include/thread/threadmanager.h"
#include "lib/include/frame/baseaccepter.h"
#include "lib/include/common/serverconfig.h"

NetThread::NetThread(ThreadManager *manager)
: BaseThread(manager)
{

}

void NetThread::Init()
{
	ServerInfo info = GatawayConfig::Instance().m_server[m_id - ThreadManager::ID_NET];
 	CallBack *call = new CallBack(this);
 	m_net_manager.InitServer("127.0.0.1", 2345, 16, new BaseAccepter(&m_net_manager), call);
}


bool NetThread::Run()
{

	return m_net_manager.Update();
}

void NetThread::RecvMsg(unsigned char sid, int len, const char *data)
{

}

