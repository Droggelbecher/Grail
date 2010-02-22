
#include <algorithm>
#include <cstring>
#include <string>

#include <lua.hpp>

#include "lib/debug.h"
#include "lib/utils.h"

#include "interpreter.h"
#include "lua_exception.h"
#include "network_interface.h"
using namespace grail;

using namespace boost::asio;
using boost::asio::ip::tcp;
using std::string;

size_t NetworkInterface::Connection::isReadComplete(const boost::system::error_code& error, size_t bytes_transferred) {
  chunkSize = bytes_transferred;

  buffer[chunkSize] = '\0';
  split = (uint8_t*)strchr((char*)buffer, '\n');
  if(!split) {
    split = buffer + chunkSize;
    return bufferSize - chunkSize;
  }
  return 0;
}

void NetworkInterface::Connection::handleRead(const boost::system::error_code& error, size_t bytes_transferred) {
  executeChunk();
}


void NetworkInterface::Connection::handleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
  startRead();
}

bool returnTrue(char c) { return true; }

void NetworkInterface::Connection::executeChunk() {
  // Load buffer
  lua_State *L = interpreter.L;
  luaL_loadbuffer(L, (char*)(buffer), split - buffer, "Remote lua code");

  // Delete executed part from buffer
  int32_t sz = chunkSize - ((split + 1) - buffer);
  if(sz > 0) {
    memmove(buffer, split+1, sz);
    chunkSize = sz;
  }
  else {
    chunkSize = 0;
  }
  split = buffer;

  // Execute lua code
  //int stack_top = lua_gettop(L);
  int error = lua_pcall(L, 0, 1, 0); //LUA_MULTRET);

  // Was there a problem with the code?
  cdbg << "Executed remote lua code with return_code " << error << "\n";

  std::string answer = "{\n  [\"return_code\"] = " + toString(error) + ",\n";

  if(error) {
    std::string err(lua_tostring(L, -1));
    answer += "  [\"error_message\"] = " + interpreter.toLuaString(err) + ",\n";
    answer += "  [\"return_value\"] = nil,\n";
  }
  else {
    answer += "  [\"error_message\"] = nil,\n";

    // Fetch return value
    luabind::object return_value(luabind::from_stack(L, -1));
    lua_pop(L, 1);

    // Send answer to client
    std::string ret = interpreter.toLuaString(return_value, "  ");
    answer += "  [\"return_value\"] = " + ret + ",\n";

  }
  answer += "}\n";

  boost::asio::async_write(
        _socket,
        boost::asio::buffer(answer),
        boost::bind(
          &NetworkInterface::Connection::handleWrite,
          shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred
        )
  );
}


NetworkInterface::Connection::Connection(boost::asio::io_service& ioService) :
  bufferSize(64 * 1024),
  _socket(ioService), chunkSize(0), buffer(new uint8_t[bufferSize]), split(buffer) {
}

NetworkInterface::Connection::~Connection() {
  delete[] buffer;
}

void NetworkInterface::Connection::start() {
  startRead();
}

void NetworkInterface::Connection::startRead() {
  boost::asio::async_read(
        _socket,
        boost::asio::buffer((void*)buffer, bufferSize),
        boost::bind(
          &NetworkInterface::Connection::isReadComplete,
          shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred
        ),
        boost::bind(
          &NetworkInterface::Connection::handleRead,
          shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred
        )
  );
}

NetworkInterface::NetworkInterface(io_service& io_service)
    : acceptor(io_service, tcp::endpoint(tcp::v4(), 12345)) {
  startAccept();
}

NetworkInterface::~NetworkInterface() {
}

void NetworkInterface::startAccept() {
  Connection::Ptr connection(new Connection(acceptor.io_service()));
  acceptor.async_accept(
      connection->socket(),
      boost::bind(&NetworkInterface::handleAccept, this, connection,
        boost::asio::placeholders::error)
  );
}

void NetworkInterface::handleAccept(Connection::Ptr connection, const boost::system::error_code& error) {
  if(!error) {
    connection->start();
    startAccept();
  }
}

