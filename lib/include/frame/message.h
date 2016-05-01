﻿
#ifndef MESSAGE_H
#define MESSAGE_H

#include "socketmsg.h"
#include "common/socketdef.h"
#include "lib/include/common/memoryvl.h"

// 网络消息
class GameMsg
{
public:
	GameMsg(unsigned short _msg_index, unsigned short _msg_type, NetHandle _handle, char* _data, unsigned int _length);
	~GameMsg();

	void *		operator new(size_t c);
	void		operator delete(void *m);

	unsigned short	msg_index;
	unsigned short	msg_type;
	NetHandle		handle;
	unsigned int	length;
	char *			data;
};

class GameMsgManager
{
public:
	GameMsgManager();
	~GameMsgManager();

	GameMsg *Alloc(unsigned short msg_index, unsigned short msg_type, NetHandle handle, const char* data, unsigned int length);
	void	Free(GameMsg *msg);
private:
	MemoryVL *memory;
};


class ThreadMsg
{
public:
	ThreadMsg(short _type, int _id, int _length, const char *_data);
	~ThreadMsg();

	void *		operator new(size_t c);
	void		operator delete(void *m);

	short	type;	// 消息类型
	int		id;
	int		length;
	char	*data;
};

class MsgCallBack
{
public:
	MsgCallBack(){}
	~MsgCallBack(){}

	virtual void Accept(NetHandle handle, const char *ip){};

	virtual void Recv(GameMsg *msg) = 0;

	virtual void Disconnect(NetHandle handle, int reason) = 0;

	virtual void Connect(NetHandle handle, int flag){}
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
		MSG_CONNECT,
		MSG_MAX
	};

	virtual void Recv(GameMsg *msg){};
	MsgCallBack *m_call_back;
};

class AcceptMsg : public BaseMsg
{
public:
	AcceptMsg(MsgCallBack *call_back) :BaseMsg(call_back){}
	~AcceptMsg(){}

	virtual void Recv(GameMsg *msg){ m_call_back->Accept(msg->handle, msg->data); };
};

class RecvMsg : public BaseMsg
{
public:
	RecvMsg(MsgCallBack *call_back) :BaseMsg(call_back){}
	~RecvMsg(){}

	virtual void Recv(GameMsg *msg){ m_call_back->Recv(msg); }
};

class DisconnectMsg : public BaseMsg
{
public:
	DisconnectMsg(MsgCallBack *call_back) :BaseMsg(call_back){}
	~DisconnectMsg(){}

	virtual void Recv(GameMsg *msg){ m_call_back->Disconnect(msg->handle, *(int*)msg->data); }
};

class ConnectMsg : public BaseMsg
{
public:
	ConnectMsg(MsgCallBack *call_back) :BaseMsg(call_back){}
	~ConnectMsg(){}

	virtual void Recv(GameMsg *msg)
	{ 
		SocketMsg::AddHandlerRet::Data *data = (SocketMsg::AddHandlerRet::Data *)msg->data;
		m_call_back->Connect(data->handle, data->flag);
	}
};
#endif