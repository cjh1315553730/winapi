#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mstcpip.h>
#include <comdef.h>

#include <windows.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <wchar.h>
#include <psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <lua.hpp>
#include "winapi.h"


static int print_lua_stack(lua_State *L) {
    const char *header_flag = "\n---------------------------------------------";
    lua_getglobal(L, "print");
    lua_pushstring(L, header_flag);
    lua_call(L, 1, 0);

    lua_getglobal(L, "print");
    lua_pushstring(L, "行号");
    lua_pushstring(L, "索引");
    lua_pushstring(L, "索引2");
    lua_pushstring(L, "类型");
    lua_pushstring(L, "值");
    lua_call(L, 5, 0);

    int len = lua_gettop(L);
    int row = 1;
    for (int i = len; i > 0; i--)
    {
        const char* type = luaL_typename(L, i);
        lua_getglobal(L, "print");
        lua_pushinteger(L, row);
        lua_pushinteger(L, i);
        lua_pushinteger(L, -row);
        lua_pushstring(L, type);
        lua_pushvalue(L, i);
        lua_call(L, 5, 0);
        row++;
    }

    const char *footer_flag = "---------------------------------------------\n";
    lua_getglobal(L, "print");
    lua_pushstring(L, footer_flag);
    lua_call(L, 1, 0);

    return 0;
}


int log(std::string str) {
	int lastError = WSAGetLastError();
std::cout << "出错了: " << str << "::" << lastError << std::endl;
	return lastError;
}

static int lua_network_capture_start(lua_State *L) {
    WSADATA wsa;
	int error = 0;
    error = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (error != 0) {
		log("WSAStartup: ");
	}

	char name[100];
	gethostname(name, sizeof(name));

	struct addrinfo *result = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_IP;

    error = getaddrinfo(name, 0, &hints, &result);
	if (error != 0) {
		std::cout << error << std::endl;
		log("getaddrinfo: ");
		WSACleanup();
	}

	SOCKET rawSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (rawSocket == INVALID_SOCKET) {
		std::cout << rawSocket << std::endl;
		log("socket: ");
		freeaddrinfo(result);
		WSACleanup();
	}

	// struct sockaddr_in6 addr;
	// addr.sin6_family = AF_INET6;
	// addr.sin6_port = 0;
	// addr.sin6_flowinfo = 0;
	// addr.sin6_addr = in6addr_any;
	// addr.sin6_scope_id = 0;

    error = bind(rawSocket, result->ai_addr, (int)result->ai_addrlen);
	if (error == SOCKET_ERROR) {
	// if (error = bind(rawSocket, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cout << error << std::endl;
		log("bind: ");
		freeaddrinfo(result);
		closesocket(rawSocket);
		WSACleanup();
	}

	// BOOL bValue = TRUE;
	// if (error = setsockopt(rawSocket, result->ai_protocol, IP_HDRINCL, (char*)&bValue, sizeof(bValue)) == SOCKET_ERROR) {
	// 	std::cout << error << std::endl;
	// 	log("setsockopt: ");
	// 	freeaddrinfo(result);
	// 	closesocket(rawSocket);
	// 	WSACleanup();
	// }

	DWORD dwValue = RCVALL_ON;
    error = ioctlsocket(rawSocket, SIO_RCVALL, &dwValue);
	if (error != 0) {
		std::cout << error << std::endl;
		log("ioctlsocket: ");
		freeaddrinfo(result);
		closesocket(rawSocket);
		WSACleanup();
	}

  

	char buff[4096];
	int count;
	while (true) {
		count = recv(rawSocket, buff, 4096, 0);
		std::cout << count << std::endl;
		if (count == SOCKET_ERROR) {
			log( "recv: " );
		}
		else {
            lua_getglobal(L, "print");
            lua_pushinteger(L, buff[0]);
            lua_pushinteger(L, buff[1]);
            lua_call(L, 2, 0);
		}
	}

  freeaddrinfo(result);
  closesocket(rawSocket);
  WSACleanup();

  return 0;
}


















// 注册模块函数
static const struct luaL_Reg winapi[] = {
    {"print_lua_stack", print_lua_stack},
    {"lua_network_capture_start", lua_network_capture_start},

    {NULL, NULL} // 数组结束标志
};

LUA_winapi int luaopen_winapi(lua_State *L)
{
    luaL_newlib(L, winapi);

    return 1;
}