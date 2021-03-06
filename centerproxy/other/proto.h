
#ifndef CENTER_PROXY_PROTO_H
#define CENTER_PROXY_PROTO_H

#pragma pack(push)
#pragma pack(4)

namespace InnerProtocol
{
	struct MessageHeader
	{
		MessageHeader(int type) :msgid(type), outer_netid(-1){}

		unsigned short	msgid;
		unsigned short	outer_netid;
	};

	// 这里的协议要与原来的一一对应
	enum _MsgType
	{
		MT_INTERNAL_INVALID,

		MT_INNER_GATE_SERVER_USER_RECV = 1,
		MT_INNER_SERVER_GATE_USER_SEND_ONE = 4,
		MT_INNER_SERVER_GATE_USER_SEND_MUL,
		MT_INNER_SERVER_GATE_USER_SEND_ALL,
		MT_INNER_SERVER_GATE_USER_DISCONNECT,
		MT_INNER_GAME_TO_CENTER,					// 本地场景与中心场景通信
		MT_INNER_CENTER_TO_GAME,					// 中心场景与本地场景通信
		MT_INNER_CENTER_BACK_TO_GAME,		// 中心服玩家下线，在本地重新上线
	};

	struct WGNetRecvMsg
	{
		WGNetRecvMsg() :header(MT_INNER_GATE_SERVER_USER_RECV){}
		MessageHeader	header;
		// 从该地址开始为收到消息的内容
	};

	struct GWSceneSend
	{
		GWSceneSend() :header(MT_INNER_GAME_TO_CENTER){}
		MessageHeader	header;
		// 后面紧更着消息内容
	};
}

#pragma pack(pop)

#endif // !CENTER_PROXY_PROTO_H
