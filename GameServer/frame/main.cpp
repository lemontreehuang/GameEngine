
#include <stdio.h>
#include <time.h>
#include "gameframe.h"
#include "common/protocol/messageheader.h"
#include "libtinyxml.h"


bool GameFrame::Init()		// 框架初始化
{
	return true;
}


void GameFrame::Recv(NetHandle handle, char *msg, unsigned int length)
{

}


void GameFrame::Update(time_t now)	// 构架更新
{
	//m_time_event_manager.Update(now);
	//StmtSelect();
	//StmtInsert();
	//StmtUpdate();
	//StmtDelete();
	//exit(0);
}

void GameFrame::Exit()
{
	// 进程退出，线程也随之退出

	printf("game server exit\n");
}

