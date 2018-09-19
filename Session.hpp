#ifndef __SESSION__
#define __SESSION__

#include <iostream>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <jsoncpp/json/json.h>
#include <jsoncpp/json/value.h>

#include "DBHandler.hpp"

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

// To safely generate new shared_ptr instances
// pointing the same object, inherit from
// enable_shared_from_this
class Session : public boost::enable_shared_from_this<Session>
{
public:
  Session(boost::asio::io_service& io_service,
      boost::asio::ssl::context& context);

  ssl_socket::lowest_layer_type& socket();

  void start();

  void echo();
  void write();

private:

  // Handlers
  void handle_handshake(const boost::system::error_code& error);

  void handle_read(const boost::system::error_code &error,
      size_t bytes_transferred);

  void handle_write(const boost::system::error_code &error);

private:
  ssl_socket socket_;
  enum
  {
    max_length = 1024
  };
  char data_[max_length];
  int bytes_transferred_;
};

typedef boost::shared_ptr<Session> SessionPtr;

#endif
