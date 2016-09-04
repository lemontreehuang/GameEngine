
#ifndef GATE_THREAD_H
#define GATE_THREAD_H

#include <queue>
#include "lib/include/frame/socketthread.h"
#include "lib/include/common/serverconfig.h"
#include "common/datastructure/gamehash.h"
#include "common/datastructure/msgqueue.h"
#include "common/datastructure/gamearray.h"

class ThreadManager;
class GateCreator;
class TimerQueue;
class GateThread : public SocketThread
{
public:
	virtual ~GateThread();
	GateThread(ThreadManager *manager);

	static const unsigned char THREAD_TYPE = 1;

	enum
	{
		GATE_ROLE_BIND = BASE_THREAD_MSG_TYPE(THREAD_TYPE)
	};

	void		Dispatch(unsigned int msg_id, NetMsg &msg);
	unsigned int RegRole(NetHandle handle);
	void		DelRole(unsigned int index);
	void		ChangeChannel(NetHandle handle);
	void		PushTimer(NetHandle handle);

	//ThreadID	GetGlobal(){ return m_global; }
protected:
	bool	Init();
	void	Ready();
	void	RecvData(TPT type, ThreadID sid, int len, const char *data);
	bool	DoSomething();
private:

	NetHandle		m_cneter_handle;
	/*
		新建立的role在这里注册一个消息队列，将索引设置给listener
	*/
	typedef game::Array<MsgQueue<NetMsg> *> ROLE_MSG;
	ROLE_MSG		m_role_msg;
	//ThreadID		m_global;
	TimerQueue		*m_timer_queue;
};

#endif