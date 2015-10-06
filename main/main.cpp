#include <iostream>
#include <boost/asio.hpp>

#include <io/strings.h>
#include <io/template.h>
#include <linux/file.h>
#include <linux/status.h>
#include <server/routes.h>
#include <server/server.h>
#include <system/log.h>
#include <system/system.h>

int main(int argc, char* argv[]) {
	try {
		if (argc != 2) {
			std::cerr << "Usage: server <port>\n";
			return 1;
		}

		// start the log file
		auto log_path = "/tmp/web_log";
		sys::LogFile::init(log_path);

		sys::log() << "create monitors\n";
		sys::System::create();


		sys::log() << "start web server\n";
		std::string content_path = io::read_file("src_path");
		content_path = io::split(content_path, '\n')[0] + "/content";
		http::routes routes(content_path);

		// a page template
		io::tmpl index_tmpl(io::read_file(content_path + "/index.html"));
		io::tmpl sys_tmpl(io::read_file(content_path + "/system.html"));

		// status object
		type::node test("test");

		// the routes to use
		routes.add_routes("/index.html", [&index_tmpl, &test](const http::route_args_t &, const http::request &request) {
			return http::static_content(index_tmpl.render(test), "text/html");
		});
		routes.add_routes("/system.html", [&sys_tmpl](const http::route_args_t &, const http::request &request) {
			auto sys = sys::System::get();
			return http::static_content(sys_tmpl.render(sys->make_node()), "text/html");
		});
		routes.add_routes("/idletime", [](const http::route_args_t &, const http::request &request) {
			int new_sec = stoi(request.data.at("minutes")) * 60;
			auto sys = sys::System::get();
			sys->get_power_ctrl()->set_idle_seconds(new_sec);
			return http::static_content("shutdown time set to "+std::to_string(new_sec), "text/html");
		});
		routes.add_routes("/off", [](const http::route_args_t &, const http::request &request) {
			auto sys = sys::System::get();
			sys->get_power_ctrl()->shutdown();
			return http::static_content("system shut down now", "text/plain");
		});
		routes.add_routes("/status", [](const http::route_args_t &, const http::request &request) {
			return http::static_content("system shut down now", "text/plain");
		});

		// server with port and root web directory
		server::server server(routes, std::atoi(argv[1]));
		server.run(); // blocks until exit

		sys::log() << "Closing log file\n";
		sys::LogFile::close();
	}
	catch (std::exception& e) {
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}
