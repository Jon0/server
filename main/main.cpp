#include <iostream>

#include <linux/socket.h>
#include <server/routes.h>
#include <server/server.h>

void start_server(int port) {
	http::routes routes;
	routes.add_routes("/.+", [](const http::request &request) {
		std::string type = request.split[1];
		return http::static_content(type, "text/plain");
	});



	server::tcp_server<os::tcp_acceptor> server(routes, port);
	server.run();
}


int main(int argc, char* argv[]) {
	start_server(8080);
	return 0;
}
