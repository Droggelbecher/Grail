
#include <string>
#include <lua.hpp>

#include "string.h" // memset
#include "network_interface.h"
#include "lib/utils.h"
#include "lib/debug.h"
#include "interpreter.h"
#include "lua_exception.h"

using namespace grail;

NetworkInterface::NetworkInterface(uint32_t port) : port(port), listenSocket(-1) {
  #ifdef WIN32
  WSAData wsaData;
  int r = WSAStartup(MAKEWORD(1, 1), &wsaData);
  if(r != 0) {
    throw Exception("WSAStartup failed with code " + toString(r) + ".");
  }
  #endif
  FD_ZERO(&sockets);
}

NetworkInterface::~NetworkInterface() {
  #ifdef WIN32
  WSACleanup();
  #endif
}


void NetworkInterface::bindLocal() {
  using std::string;

  int r;
  sockaddr_in addr;

  memset(&addr, 0, sizeof(sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY; //INADDR_LOOPBACK;

  listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if(listenSocket == -1) {
    throw Exception("Couldnt bind to local socket port " + toString(port));
  }

  int yes = 1;
  r = setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  if(r == -1) {
    throw Exception("setsockopt() failed.");
  }

  r = bind(listenSocket, (sockaddr*)&addr, sizeof(sockaddr_in));
  if(r == -1) {
    close(listenSocket);
    throw Exception("bind() failed.");
  }
}

void NetworkInterface::listen() {
  int r = ::listen(listenSocket, 10);
  if(r == -1) {
    throw Exception("I won't listen().");
  }
  FD_SET(listenSocket, &sockets);
  maxSocket = listenSocket;
}

void NetworkInterface::select() {
  uint8_t buffer[1024];
  fd_set tmp = sockets;
  timeval timeout;

  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  int r = ::select(maxSocket + 1, &tmp, 0, 0, &timeout);
  if(r == -1) {
    throw Exception("select() failed.");
  }

  for(int i=0; i<=maxSocket; i++) {
    if(FD_ISSET(i, &tmp)) {
      if(i == listenSocket) { // New connection on listen socket
        struct sockaddr_in client_address;
        socklen_t client_address_length;
        int new_socket = accept(listenSocket, reinterpret_cast<sockaddr *>(&client_address), &client_address_length);

        if(new_socket != -1) {
          FD_SET(new_socket, &sockets);
          if(new_socket > maxSocket) {
            maxSocket = new_socket;
          }
        }
      } // if listenSocket
      else { // New data from a client
        int n = recv(i, buffer, sizeof(buffer)-1, 0);
        if(n == 0) {
          // connection closed
          close(i);
          FD_CLR(i, &sockets);
        }
        else if(n < 0) {
          close(i);
          FD_CLR(i, &sockets);
          throw Exception("recv() failed.");
        }
        else { // n > 0
          buffer[n] = 0;
          const char* pos = strchr((const char*)buffer, '\n');
          size_t oldsize = chunk.size();
          chunk += (char*)buffer;
          if(pos) {
            executeChunk(oldsize + (pos - (char*)buffer));
          }
        }
      } // else
    } // if FD_ISSET
  } // for

}

void NetworkInterface::executeChunk(size_t n) {
  cdbg << "Got data chunk: " << chunk.substr(0, n) << "\n";
  lua_State *L = interpreter.L;
  luaL_loadbuffer(L, chunk.c_str(), chunk.size(), "Remote lua code");
  int error = lua_pcall(L, 0, LUA_MULTRET, 0);
  chunk = chunk.substr(n+1);
  cdbg << "Executed with code " << error << "\n";
  if(error) {
    throw LuaException(L);
  }
}

