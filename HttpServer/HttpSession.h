#ifndef HTTP_SESSION
#define HTTP_SESSION

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include "HttpProcessor/HttpProcessor.h"

class HttpSession : public std::enable_shared_from_this<HttpSession>{
    boost::beast::tcp_stream _tcpStream;
    boost::beast::flat_buffer _buffer;
    boost::beast::http::request<boost::beast::http::string_body> _req;
    boost::beast::http::response<boost::beast::http::string_body> _res;

    void onRead(boost::beast::error_code ec, size_t length);
    void onWrite(bool close, boost::beast::error_code, size_t lenght);
    void read();
    void write();

    public:
    HttpSession(boost::asio::ip::tcp::socket && socket);
    void run();
    ~HttpSession();
};

#endif /*HTTP_SESSION*/