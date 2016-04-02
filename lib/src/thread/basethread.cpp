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
: m_id(-1)
, m_arg(NULL)
, m_manager(manager)
, m_thread(NULL)
, m_is_exit(false)
, m_is_start(false)
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
	if (!m_is_start)
	{
		Init(m_arg);
		if (m_arg != NULL)
		{
			delete m_arg;
			m_arg = NULL;
		}
		m_thread = new std::thread(::Update, this);
		m_is_start = true;
	}
}

void BaseThread::Loop(bool sleep)
{
	ThreadMsg *msg;
	bool is_sleep = sleep;
	do 
	{
		is_sleep = sleep;
		while (m_recv_queue.Pop(msg) && msg != NULL)
		{
			if (msg->type > ThreadSysID::MAX_ID)
			{
				this->RecvMsg(msg->type, msg->id, msg->length, msg->data);
			}
			else if (msg->type == ThreadSysID::TSID_EXIT)
			{
				this->Exit();
			}
			
			delete msg;
			is_sleep = false;
		}
		if (is_sleep && !this->Run())
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




