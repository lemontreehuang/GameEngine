
#include <stdio.h>
#include "inneraccepter.h"
#include "baselistener.h"
#include "netmanager.h"
#include "common/socketdef.h"

// void InnerAccepter::OnCanRead()
// {
// 	static struct sockaddr_in addr;
// 	static SOCKET_LEN len = sizeof(struct sockaddr);
// 	SOCKET new_sock = accept(m_sock, (struct sockaddr*)&addr, &len);
// 	if (new_sock != INVALID_SOCKET)
// 	{
// 		if (ip != 0 && addr.sin_addr.s_addr != ip)
// 		{
// 			return;
// 		}
// 
// 		printf("connect server ip = %s\n", inet_ntoa(addr.sin_addr));
// 
// 		
// //		listener
// 
// // 		BaseListener *handler = new BaseListener(m_net_manager, call_back);
// // 		handler->m_sock = new_sock;
// // 		m_net_manager->AddNetHandler(handler);
// 	}
// }
