#include <iostream>
#include <boost/asio.hpp>
#include "async_server.h"

int main(int argc, char const *argv[]) {

    if (argc < 3)
    {
      std::cerr << "Usage: chat_server <port> <bulk_size>\n";
      return 1;
    }

    boost::asio::io_service io_service;
    tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));
    bulk_server b(io_service, endpoint, std::atoi(argv[2]));
    io_service.run();
    return 0;
}
