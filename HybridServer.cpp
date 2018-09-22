#include "HybridServer.hpp"

HybridServer::HybridServer(const std::string& address, const std::string& port,
    std::size_t thread_pool_size) :
    signals_(io_service_), acceptor_(io_service_), context_(
        boost::asio::ssl::context::sslv23), thread_pool_size_(thread_pool_size)
{
  boost::asio::ip::address_v4 addr;
  addr.from_string(address);
  port_ = stoi(port);

  // Initialize an endpoint object on 0.0.0.0:port
  // Listen all interfaces on specific port
  // Initialize the acceptor with the option SO_REUSEADDR and bind the endpoint
  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address(addr),
      port_);
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  // Initialize SSL context
  context_.set_options(
      boost::asio::ssl::context::default_workarounds
          | boost::asio::ssl::context::no_sslv2
          | boost::asio::ssl::context::single_dh_use);
  context_.set_password_callback(
      boost::bind(&HybridServer::get_password, this));
  context_.use_certificate_chain_file("server.cert.pem");
  context_.use_private_key_file("server.key.pem",
      boost::asio::ssl::context::pem);
  context_.use_tmp_dh_file("dhparam4096.pem");

  start_accept();
}

// TODO: Assign different port to each thread
void HybridServer::run()
{
  std::vector<boost::shared_ptr<boost::thread> > thread_pool;

  for (std::size_t i = 0; i < thread_pool_size_; ++i) {
    boost::shared_ptr<boost::thread> thread(
        new boost::thread(
            boost::bind(&boost::asio::io_service::run, &io_service_)));
    thread_pool.push_back(thread);
  }

  // Wait for all threads in the pool to exit.
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
    thread_pool[i]->join();
}

void HybridServer::start_accept()
{
  // Assign new instance to shared_ptr
  new_session.reset(new Session(io_service_, context_));
  acceptor_.async_accept(new_session->socket(),
      boost::bind(&HybridServer::handle_accept, this,
          boost::asio::placeholders::error));
}

void HybridServer::handle_accept(const boost::system::error_code& error)
{
  if (!error) {
    new_session->start();
  } else {
  }

  start_accept();
}

std::string HybridServer::get_password() const
{
  // D34dly p4$$
  return "D34dlyP4$$";
}
