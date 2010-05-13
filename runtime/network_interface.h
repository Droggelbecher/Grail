// vim: set noexpandtab:

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <boost/asio.hpp>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <ctime>
#include <vector>
#include "lib/debug.h"

using boost::asio::ip::tcp;

class NetworkInterface {
	public:
		class Connection : public boost::enable_shared_from_this<Connection> {
			private:
				size_t bufferSize;
				
				tcp::socket _socket;
				size_t chunkSize;
				uint8_t *buffer;
				uint8_t *split;
				
				void executeChunk();
				size_t isReadComplete(const boost::system::error_code& error, size_t bytes_transferred);
				void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);
				void startRead();
				void handleRead(const boost::system::error_code& error, size_t bytes_transferred);
			
			public:
				typedef boost::shared_ptr<Connection> Ptr;
				
				Connection(boost::asio::io_service& ioService);
				~Connection();
				tcp::socket& socket() { return _socket; }
				
				void start();
				void send(luabind::object o, std::string type);
		}; // class Connection
		
	private:
		boost::asio::io_service& ioService;
		tcp::acceptor acceptor;
		std::list<NetworkInterface::Connection::Ptr> connections;
		
		void startAccept();
		
	public:
		NetworkInterface(boost::asio::io_service&);
		~NetworkInterface();
		
		void handleAccept(Connection::Ptr connection, const boost::system::error_code& error);
		void broadcast(luabind::object o);
}; 

#endif // NETWORK_INTERFACE_H

