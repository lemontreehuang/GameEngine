
#include "center.h"
#include "net/netthread.h"
#include "db/dbthread.h"
#include "global/global.h"
#include "net/src/proto.h"
#include "lib/include/frame/main.h"
#include "lib/include/gate/gatethread.h"


Center::Center()
{

}

Center::~Center()
{

}

bool Center::Init()
{
	ThreadID id = m_thread_manager.Register(new Global(&m_thread_manager));
	m_thread_manager.Register(New::_GateThread(&m_thread_manager, 0, id, false));
	db_thread_id[0] = m_thread_manager.Register(new DBThread(&m_thread_manager), ThreadManager::EXIT_FINALLY);
	db_thread_id[1] = m_thread_manager.Register(new DBThread(&m_thread_manager), ThreadManager::EXIT_FINALLY);
	return m_thread_manager.Init();
}

bool Center::Start()
{
	m_thread_manager.Start();
	return true;
}

bool Center::Cmd(char *buf)
{
	return false;
}

void Center::Exit()
{

}

void Center::Wait()
{

}

GAME_MAIN(Center);