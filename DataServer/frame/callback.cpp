
#include "callback.h"
#include "dataframe.h"

void InnerCallBack::Accept()
{
    printf("fuck accept.................\n");
}

void InnerCallBack::Recv(NetMsg *msg)
{
    m_frame->Recv(msg);
}

void InnerCallBack::Disconnect(NetHandle handle)
{
    printf("fuck disconnect.................\n");
}
