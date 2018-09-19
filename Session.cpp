#include "Session.hpp"

Session::Session(boost::asio::io_service& io_service,
    boost::asio::ssl::context& context) :
    socket_(io_service, context), bytes_transferred_(0)
{
}

ssl_socket::lowest_layer_type& Session::socket()
{
  return socket_.lowest_layer();
}

void Session::start()
{
  socket_.async_handshake(boost::asio::ssl::stream_base::server,
      boost::bind(&Session::handle_handshake, shared_from_this(),
          boost::asio::placeholders::error));
}

void Session::echo()
{
  boost::asio::async_write(socket_,
      boost::asio::buffer(data_, bytes_transferred_),
      boost::bind(&Session::handle_write, shared_from_this(),
          boost::asio::placeholders::error));

}

void Session::write()
{
}

// Handlers
void Session::handle_handshake(const boost::system::error_code& error)
{
  if (!error) {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        boost::bind(&Session::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
  } else {
  }
}

void Session::handle_read(const boost::system::error_code &error,
    size_t bytes_transferred)
{
  if (!error) {
    std::cout << "Incoming message: \n";
    bytes_transferred_ = bytes_transferred;


    std::string errs;
    Json::Value json, jsonToSend;
    Json::CharReaderBuilder rbuilder;
    Json::StreamWriterBuilder wbuilder;

    rbuilder["collectComments"] = false;
    wbuilder["indentation"] = "\t";


    // TODO: Comparison of parsing performances of two methods
    //std::stringstream ss(data_);
    //bool ok = Json::parseFromStream(rbuilder, ss, &json, &errs);
    std::unique_ptr<Json::CharReader> const reader(rbuilder.newCharReader());
    reader->parse(data_, (data_ + bytes_transferred - 1), &json, &errs);
    std::cout << json << "\n";

    // The message
    if (!json["from"].asString().compare("client")) {
      if (!json["op"].asString().compare("status")) {
        // Get the status data
        std::cout << "STATUS :\n";
        Status status;
        status.fromJson(json);

        // Store status to the DB
        //DBHandler::instance()->putStatus(status);   // Sync. version Blocking!
        socket_.get_io_service().post(boost::bind(
            &DBHandler::putStatus, DBHandler::instance(), status)); // Async. version

        // Return same
        echo();
      }
      /*
       * You may add another operations
       */
    } else if (!json["from"].asString().compare("webserver")) {
      // ....
    }
  } else {
  }
}

// Not necessary in this implementation
void Session::handle_write(const boost::system::error_code &error)
{
  if (!error) {
  } else {
  }
}

