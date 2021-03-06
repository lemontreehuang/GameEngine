
#include "game.h"
#include "db/dbthread.h"
#include "db/module/rolemodule.h"
#include "db/module/role/test.h"
#include "scene/scenemanager.h"
#include "global/global.h"
#include "log/loggold.h"
#include "protocol/tproto.h"
#include "global/proto.h"
#include "common/datastructure/msgqueue.h"
#include "common/protocol/messageheader.h"
#include "lib/include/frame/main.h"
#include "lib/include/gate/gatethread.h"
#include "lib/include/common/serverconfig.h"
#include "lib/include/db/rolefield.h"


Game::Game()
: m_cur_thread_id(0)
{

}

Game::~Game()
{

}

// 框架初始化

bool Game::Init()
{
	m_global_id = m_thread_manager.Register(new Global(&m_thread_manager));

	m_thread_manager.Register(new GateThread(&m_thread_manager, 0, m_global_id, TProto::GLOBAL_REG_ROLE));
	m_thread_manager.Register(new GateThread(&m_thread_manager, 1, m_global_id, TProto::GLOBAL_REG_ROLE));

	//m_thread_manager.Register(new LoginScene(&m_thread_manager));
	
	m_db_id[0] = m_thread_manager.Register(new DBThread(&m_thread_manager));
	m_db_id[1] = m_thread_manager.Register(new DBThread(&m_thread_manager));
	m_log_id = m_thread_manager.Register(New::_Log(&m_thread_manager, sizeof(LogHandler::reg)/sizeof(LogMsg::LogRegister), LogHandler::reg));
	
	m_thread_manager.Register(new SceneManager(&m_thread_manager));
	m_thread_manager.Register(new SceneManager(&m_thread_manager));
	return m_thread_manager.Init();
}


bool Game::Start()
{
	m_thread_manager.Start();
	return true;
}

// 构架更新
void Game::Update(unsigned int interval, time_t now)
{
}

// ThreadID Game::GetThreadID()
// {
// 	if (m_cur_thread_id >= MAX_DB_THREAD)
// 	{
// 		m_cur_thread_id = 0;
// 	}
// 	return Game::Instance().db_thread_id[m_cur_thread_id++];
// }

void Game::Exit()
{
	m_thread_manager.Exit();
}

void Game::Wait()
{

}

bool Game::Cmd(char *buf)
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
		return true;
	}
	
	return false;
}

GAME_MAIN(Game);


