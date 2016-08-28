
#include "game.h"
#include "db/dbthread.h"
#include "db/module/rolemodule.h"
#include "db/module/role/test.h"
#include "net/netthread.h"
#include "scene/scenemanager.h"
#include "global/global.h"
#include "log/loggold.h"
#include "protocol/proto.h"
#include "common/datastructure/msgqueue.h"
#include "common/protocol/messageheader.h"
#include "lib/include/frame/main.h"
#include "lib/include/frame/socketthread.h"
#include "lib/include/common/serverconfig.h"
#include "lib/include/db/rolefield.h"


NewFrame::NewFrame()
{

}

NewFrame::~NewFrame()
{

}

// 框架初始化

bool NewFrame::Init()
{
	m_thread_manager.Register(new NetThread(&m_thread_manager));
	m_thread_manager.Register(new NetThread(&m_thread_manager));
	
	m_db_id[0] = m_thread_manager.Register(new DBThread(&m_thread_manager));
	m_db_id[1] = m_thread_manager.Register(new DBThread(&m_thread_manager));
	m_log_id = m_thread_manager.Register(New::_Log(&m_thread_manager, sizeof(LogHandler::reg)/sizeof(LogMsg::LogRegister), LogHandler::reg));
	m_thread_manager.Register(new Global(&m_thread_manager));
	m_thread_manager.Register(new SceneManager(&m_thread_manager));
	m_thread_manager.Register(new SceneManager(&m_thread_manager));
	return m_thread_manager.Init();
}


bool NewFrame::Start()
{
	m_thread_manager.Start();
	return true;
}

// 构架更新
void NewFrame::Update(unsigned int interval, time_t now)
{
}



void NewFrame::Exit()
{
	m_thread_manager.Exit();
}

void NewFrame::Wait()
{

}

bool NewFrame::Cmd(char *buf)
{
	if (strcmp(buf, "test") == 0)
	{
		// char buf[128];
		// int len = LogRole::MakeLog(0, 112233, buf, "fff=%d,ggg=%s", 223344, "333rrr");
		// m_thread_manager.SendMsg(m_log_id, LogDBMsg::LDM_WRITE, len, buf);
		//m_thread_manager.SendMsg(m_log_id, LogDBMsg::LDM_WRITE, LogGold(112233, 512));
		//LogHandler::DB(m_thread_manager, m_log_id, (char *)new LogGold(112233, 512));
		LogHandler::DB(m_thread_manager, m_log_id, LogNew(LogGold(112233, 512)));
		//LogHandler::Write(m_thread_manager, m_log_id, LogGold(112233, 512));
		return true;
	}
	else if (strcmp(buf, "test1") == 0)
	{
		RoleModule *rm = new RoleModule();

		rm->rf.rid = 123;
		MTest *mt = new MTest();
		mt->item_id = 3421;
		rm->rf.fields.push_back(mt);
		m_thread_manager.SendMsg(m_db_id[0], rm);
		return true;
	}
	else if (strcmp(buf, "test2") == 0)
	{
		LogHandler::Error(m_thread_manager, m_log_id, "ddddddffffff123");
		return true;
	}
	else if (strcmp(buf, "test3") == 0)
	{
		m_thread_manager.SendMsg(m_db_id[1], TProto::DB_LOAD_MODULE, (RoleID)123);
	}
	
	return false;
}

GAME_MAIN(NewFrame);


