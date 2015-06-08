
#ifndef SYSTEM_DEF_H
#define SYSTEM_DEF_H

#ifdef WIN32
	#ifdef FD_SETSIZE
		#undef FD_SETSIZE
	#endif
	#define FD_SETSIZE		4096
	#include <winsock2.h>
	#pragma comment(lib,"ws2_32.lib")
	#include <Windows.h>
#endif


#define MAX_EPOLL_SIZE		8192

static const int RECV_BUF_SIZE = 64 * 1024; // 64K


typedef char			IP[64];
typedef unsigned int	NetHandle;	// �ڲ������ʶ

#endif // !WINDOWSDEF_H
