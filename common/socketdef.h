
#ifndef SOCKET_DEF_H
#define SOCKET_DEF_H

#if (defined _WIN32) || (defined _WIN64)

#ifdef FD_SETSIZE
	#undef FD_SETSIZE
#endif

#define FD_SETSIZE		4096

	
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <Windows.h>

#define SOCKET_LEN		int
#define WOULDBLOCK		WSAEWOULDBLOCK

#endif


#ifdef __unix
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define SOCKET_LEN			socklen_t
#define INVALID_SOCKET		-1
#define WOULDBLOCK			EWOULDBLOCK
#define SOCKET_ERROR		-1
#define SOCKET				int

#endif

static const int RECV_BUF_SIZE = 64 * 1024; // 64K


#endif
