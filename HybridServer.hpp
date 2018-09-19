#ifndef __HYBRID_SERVER__
#define __HYBRID_SERVER__

#include <cstdlib>
#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "Session.h"


/*
 *  Event based and threaded hybrid server implementation
 */
class HybridServer
{
public:

  HybridServer(const std::string& address, const std::string& port,
      std::size_t thread_pool_size);

  // TODO: Assign different port to each thread
  void run();

  void start_accept();


private:
  // HANDLERS
  void handle_accept(const boost::system::error_code& error);
  std::string get_password() const;


private:
  SessionPtr new_session;

  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ssl::context context_;
  boost::asio::signal_set signals_;

  unsigned short port_;

  const std::string chain_file;
  const std::string private_key_file;
  const std::string dhparam_file;
  const std::string private_key_password;

  std::size_t thread_pool_size_;
};

#endif
