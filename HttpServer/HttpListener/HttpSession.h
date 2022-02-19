#ifndef HTTP_SESSION
#define HTTP_SESSION

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <memory>
#include "../HttpProcessor/Processor.h"
#include "Context.h"

namespace KHttp{


class ProcessorBuilder;

class HttpSession : public std::enable_shared_from_this<HttpSession>{
    boost::beast::tcp_stream _tcpStream;
    boost::beast::flat_buffer _buffer;
    Context _context;

    const ProcessorBuilder & _processorBuilder;

    void onRead(boost::beast::error_code ec, size_t length);
    void onWrite(bool close, boost::beast::error_code, size_t lenght);
    void read();
    void write();

    public:
    HttpSession(boost::asio::ip::tcp::socket && socket,const ProcessorBuilder & builder);
    void run();
    ~HttpSession();
};
}

#endif /*HTTP_SESSION*/