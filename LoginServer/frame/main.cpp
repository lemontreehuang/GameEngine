
#include "loginframe.h"

bool LoginFrame::Init()
{
	return true;
}


void LoginFrame::Recv(GameMsg *msg)
{
	struct test
	{
		int a;
		int b;
	};

	test *c = (test *)msg->data;
	Send(msg->handle, (const char *)c, sizeof(test));
	delete msg;
}

void LoginFrame::Update(unsigned int interval, time_t now)
{

}


void LoginFrame::Exit()
{

}


void LoginFrame::Wait()
{

}
