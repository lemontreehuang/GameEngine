
#include <stdio.h>
#include <string>
#include "testdatastructure.h"
#include "testlog.h"
#include "testserializer.h"
#include "testmemory.h"
#include "testsocket.h"
#include "testother.h"
#include "testcache.h"
#include "testthread.h"

<<<<<<< HEAD
class A
{
public:

	virtual void Show()
	{
		printf("class A\n");
	}
};

class B : public A
{
public:

	void Show()
	{
		A::Show();
		printf("class B\n");
	}
};

=======
>>>>>>> 5a49417596d423012ff6d1d328a4dca2cdeb9a26
int main()
{
// 	A *a = new B;
// 	a->Show();
// 	delete a;
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
	//TestSerializer::NormalDB();
	//TestMemory::TestMemoryVL();
	//TestSocket::Connect();
	//TestDataStructure::TestArray();
	//TestDataStructure::TestArrayCmpMap();
	//TestDataStructure::TestVector();
	//TestDataStructure::TestVector2();
	//TestDataStructure::TestVector3();
	//TestDataStructure::TestRankList();
	//TestDataStructure::TestSortMap2();
	//TestDataStructure::TestGameMap();
	//TestDataStructure::TestGameMapCmpMap();
	//TestDataStructure::TestList();
	//TestDataStructure::TestList2();
	//TestDataStructure::TestList3();
	//TestDataStructure::TestHash();
	
	//TestDataStructure::TestMsgQueue2();
	//TestOther::Test1();
	//TestOther::Test2();
	//TestOther::Test3();
	TestOther::Test4();
	//TestCache::TestDataQueue();
	//TestCache::TestDataMap();
	//TestCache::TestDataCache();
	//TestThread::Test1();
<<<<<<< HEAD
	std::string a = "122";
	std::string b = "123";
	if (a < b)
	{
		printf("fsdf");
	}
	else
	{
		printf("fffff");
	}
=======
<<<<<<< HEAD


=======
>>>>>>> 5a49417596d423012ff6d1d328a4dca2cdeb9a26
>>>>>>> acf3e0c81b5752c558b9b0fc45ff9e0a46b57e84
	return 0;
}

