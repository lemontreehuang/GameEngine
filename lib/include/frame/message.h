﻿
#ifndef MESSAGE_H
#define MESSAGE_H

#include "socketmsg.h"
#include "lib/include/base/netcommon.h"
#include "lib/include/common/memoryvl.h"
#include "common/serverdef.h"

class MsgMemoryManager
{
public:
	MsgMemoryManager();
	~MsgMemoryManager();

	void	Alloc(char **buf, const char* data, unsigned int length);
	char *	Alloc(unsigned int length);
	template<typename T> void Free(T &msg){if (msg.length > 0) memory->Free(msg.data);};
private:
	MemoryVL *memory;
};

// 网络消息
class NetMsg
{
public:
	NetMsg();
	NetMsg(NetHandle _handle, unsigned int _length);
	NetMsg(NetHandle _handle, char *_data, unsigned int _length);
	~NetMsg();

	NetHandle		handle;
	unsigned int	length;
	char *			data;
};

// 线程消息
class ThreadClass;
class ThreadMsg
{
public:
	ThreadMsg();
	ThreadMsg(TPT _type, char *_data);
	ThreadMsg(ThreadID _id, TPT _type, char *_data);
	ThreadMsg(ThreadID _id, TPT _type, int _length, const char *_data, MsgMemoryManager *memory);
	~ThreadMsg();
	
	ThreadID	id;
	TPT			type;	// 消息类型
	int			length;
	char		*data;
};

class MsgCallBack
{
public:
	MsgCallBack(){}
	~MsgCallBack(){}

	virtual void Accept(NetHandle handle, const char *ip){};

	virtual void Recv(NetMsg *msg) = 0;

	virtual void Disconnect(NetHandle handle, int err, int reason) = 0;
};

class BaseMsg
{
public:
	BaseMsg(MsgCallBack *call_back) :m_call_back(call_back){}
	~BaseMsg(){}

	enum MsgType
	{
		MSG_ACCEPT = 0,
		MSG_RECV,
		MSG_DISCONNECT,
		MSG_MAX
	};

	virtual void Recv(NetMsg *msg){};
	MsgCallBack *m_call_back;
};

class AcceptMsg : public BaseMsg
{
public:
	AcceptMsg(MsgCallBack *call_back) :BaseMsg(call_back){}
	~AcceptMsg(){}

	virtual void Recv(NetMsg *msg){ m_call_back->Accept(msg->handle, msg->data); };
};

class RecvMsg : public BaseMsg
{
public:
	RecvMsg(MsgCallBack *call_back) :BaseMsg(call_back){}
	~RecvMsg(){}

	virtual void Recv(NetMsg *msg){ m_call_back->Recv(msg); }
};

class DisconnectMsg : public BaseMsg
{
public:
	DisconnectMsg(MsgCallBack *call_back) :BaseMsg(call_back){}
	~DisconnectMsg(){}

	virtual void Recv(NetMsg *msg)
	{ 
		NetCommon::ErrInfo *info = (NetCommon::ErrInfo *)msg->data;
		m_call_back->Disconnect(msg->handle, info->err, info->reason);
	}
};

#endif