
#ifndef CALL_BACK_H
#define CALL_BACK_H

#include "lib/include/frame/msgcallback.h"

class NetThread;
class CallBack : public MsgCallBack
{
public:
	CallBack(NetThread *t)
		: MsgCallBack()
		, m_thread(t){}
	~CallBack(){}

	void	Accept(NetHandle handle, const char *ip);

    void	Recv(GameMsg *msg);

	void	Disconnect(NetHandle handle, int reason);

private:
	NetThread *m_thread;
};

#endif