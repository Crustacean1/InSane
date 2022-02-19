#ifndef SCHEDULER
#define SCHEDULER

#include <boost/asio.hpp>
#include <functional>

class Scheduler {
    boost::asio::io_context &_ioc;
    static void regularWrapper(std::shared_ptr<boost::asio::deadline_timer> timer,const std::function<bool()> &fnc,size_t timeOffset);
   public:
    Scheduler(boost::asio::io_context &ioc);
    void addRegularTask(const std::function<bool()> &fnc,size_t timeOffset);
};

#endif /*SCHEDULER*/