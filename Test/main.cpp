
#include <stdio.h>
// #include "common/commonfunction.h"
// //#include "lua/luatest.h"
// #include "websocket/websockettest.h"
// #include "thread/threadtest.h"
// #include "other/hashlisttest.h"
#include "testdatastructure.h"
// #include "other/bridge.h"
#include "testlog.h"
#include "testserializer.h"
#include "testmemory.h"
#include "testsocket.h"


void login()
{

}

int main()
{
	//printf("%s\n", Function::WorkDir().c_str());
	//VariableSave();
	// WebSocketTest::Listen();
	//ThreadTest::MutexTest();
	//HashListTest::StrHashListTest();
	//Function::WindowsPause();
	//SocketTest::Connect();
	//SocketTest::Connect1();
	//MemoryTest::MallocTest();
	//LogTest::Test();
	//LogTest::MakeDir();
	//CircleQueueTest::Test();
// 	{
// 		Computer computer;
// 		{
// 			CPU cpu = computer.GetCPU1();
// 		}
// 	}

// 	{
// 		Computer computer;
// 		{
// 			CPU *cpu = computer.GetCPU2();
// 		}
// 	}
//	ThreadTest::CreateThread();
	//TestDataStructure::TestGameSet();

	//TestLog::WriteLog();
	//TestSerializer::Normal();
	//TestSerializer::ShowLength();
	TestSerializer::NormalDB();
	//TestMemory::TestMemoryVL();
	//TestSocket::Connect();
	//TestDataStructure::TestArray();
	//TestDataStructure::TestArrayCmpMap();
	//TestDataStructure::TestVector();
	//TestDataStructure::TestRankList();
	//TestDataStructure::TestRankList2();
	//TestDataStructure::TestGameMapOnly();
	//TestDataStructure::TestGameMapCmpMap();
	//TestDataStructure::TestList();
	//TestDataStructure::TestHash();
	return 0;
}
