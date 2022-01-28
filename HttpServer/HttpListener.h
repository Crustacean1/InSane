#ifndef POST_SERVER
#define POST_SERVER

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>

class HttpListener{

    boost::asio::io_context & _ioc;
    boost::asio::ip::tcp::acceptor _acceptor;
    
    void onAccept(boost::beast::error_code ec,boost::asio::ip::tcp::socket socket);
    public:
    HttpListener(boost::asio::io_context & context,boost::asio::ip::tcp::endpoint endpoint);
    void run();
};

#endif /*POST_SERVER*/