
#ifndef MAIN_THREAD_H
#define MAIN_THREAD_H

#include "basethread.h"
#include "lib/include/frame/netmanager.h"

class MainThread : public BaseThread
{
public:
	~MainThread(){}
	MainThread();

	void	Update();
private:
	NetManager	m_net_manager;
};

#endif