
#ifndef THREAD_NET_H
#define THREAD_NET_H

#include "nethandler.h"
#include "common/datastructure/gamearray.h"
#include "lib/include/thread/basethread.h"

class NetManager;
class ThreadNet : public BaseThread
{
public:
	ThreadNet(ThreadManager *manager, void *arg);
	virtual ~ThreadNet();

	struct RemoveInfo
	{
		NetHandle handle;
		int reason;
	};

	NetHandle		AddNetHandler(NetHandler *handler);
	void			RemoveHandler(NetHandle handle, int reason);

protected:
	virtual void	InitNetHandler(NetHandler *handler) = 0;
	void			Init(void *arg);
	void			RecvData(short type, int sid, int len, const char *data);
	void			AddHandler(const char *data);
	void			SendMsg(NetHandle handle, int length, const char *data);
protected:
	NetManager		*m_net_manager;

	typedef game::Array<NetHandler*>	NET_HANDLER_ARRAY;
	typedef game::Vector<RemoveInfo>	INVALID_HANDLE;

	NET_HANDLER_ARRAY		m_net_handler;
	INVALID_HANDLE			m_invalid_handle;
};

#endif