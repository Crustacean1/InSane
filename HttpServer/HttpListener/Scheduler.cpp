#include "Scheduler.h"
#include <iostream>

Scheduler::Scheduler(boost::asio::io_context &ioc) : _ioc(ioc){}

void Scheduler::regularWrapper(std::shared_ptr<boost::asio::deadline_timer> timer,const std::function<bool()> & fnc,size_t timeOffset){
    if(!fnc()){return;}
    timer->expires_at(timer->expires_at() + boost::posix_time::milliseconds(timeOffset));
    timer->async_wait(std::bind(Scheduler::regularWrapper,timer,fnc,timeOffset));
}
void Scheduler::addRegularTask(const std::function<bool()> & fnc,size_t offsetTime){
    auto timer = std::make_shared<boost::asio::deadline_timer>(_ioc,boost::posix_time::milliseconds(offsetTime));
    timer->async_wait(std::bind(Scheduler::regularWrapper,timer,fnc,offsetTime));
}