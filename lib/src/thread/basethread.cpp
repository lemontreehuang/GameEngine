﻿
#include "basethread.h"
#include "threadmanager.h"
#include "lib/include/common/mutex.h"

void *Update(void * arg)
{
	((BaseThread *)arg)->Loop();
	return NULL;
}

BaseThread::BaseThread(ThreadManager *manager)
: m_manager(manager)
, m_thread(NULL)
, m_is_exit(false)
{

}

BaseThread::~BaseThread()
{
	if (m_thread)
	{
		delete m_thread;
	}
}

void BaseThread::Start()
{
	Init();
	m_thread = new std::thread(::Update, this);
}

void BaseThread::Loop(bool sleep)
{
	ThreadMsg *msg;
	bool is_sleep = sleep;
	do 
	{
		is_sleep = sleep;
		Run();
		while (m_recv_queue.Pop(msg))
		{
			if (msg->cmd == ThreadManager::CMD_NOT)
			{
				this->RecvMsg(msg->id, msg->length, msg->data);
			}
			else if (msg->cmd == ThreadManager::CMD_EXIT)
			{
				this->Exit();
			}
			else
			{
				this->CMD(msg->cmd, msg->id, msg->length, msg->data);
			}
			
			delete msg;
			is_sleep = false;
		}
		if (is_sleep)
		{
			GameTime::Sleep(2);
		}

	} while (!m_is_exit);
}

void BaseThread::PushMsg(ThreadMsg *msg)
{
	m_recv_queue.Push(msg);
}

void BaseThread::Exit()
{
	m_is_exit = true;
}

void BaseThread::Wait()
{
	if (m_thread)
	{
		m_thread->join();
	}
	Loop();
}




