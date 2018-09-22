#include "DBHandler.hpp"
#include "HybridServer.hpp"

int main(int argc, char* argv[])
{
  try {
    if (argc != 2) {
      std::cerr << "Usage: server <port>\n";
    }

    DBHandler::instance()->init("dbname=hybridserver user=repomaster");

    HybridServer s("127.0.0.1", std::string(argv[1]), 10);
    s.run();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}

