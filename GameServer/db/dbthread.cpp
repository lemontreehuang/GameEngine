﻿
#include "dbthread.h"
#include "lib/include/thread/threadmanager.h"


DBThread::~DBThread()
{

}

DBThread::DBThread(ThreadManager *manager)
: BaseThread(manager)
{

}

void DBThread::Init()
{

}

int dbi = 0;
bool DBThread::Run()
{
	return false;
}

void DBThread::RecvMsg(unsigned char sid, int len, const char *data)
{

}


