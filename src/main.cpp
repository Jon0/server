// asio async server

#include <boost/asio.hpp>

#include "server.h"

int main(int argc, char* argv[]) {
	try {
		if (argc != 2) {
			std::cerr << "Usage: server <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		// server with port and root web directory
		io::server s(io_service, std::atoi(argv[1]));


		// todo: use multiple run threads
		io_service.run(); // blocks until exit
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}