#include "Route.h"
#include <stdexcept>

using namespace KHttp;

Route::Route(const std::string & route){
    size_t prev, next;
    prev = next = 0;
    for(;next != std::string::npos;next=route.substr(prev + 1).find("/")){
        _parsedPath.push_back(route.substr(prev,next));
        prev=next;
    }
}
Route::Route(Route && route) : _parsedPath(std::move(route._parsedPath)){}
Route::Route(const Route & route) : _parsedPath(route._parsedPath){}

bool Route::comparePathsInRange(const std::vector<std::string> & a,const std::vector<std::string> & b,size_t range) const{
    if(a.size() < range || b.size() < range){
        throw std::runtime_error("In comparePathsInRange: range exceeds size of path vector");
    }
    for(size_t i = 0;i<range;++i){
        if(a[i] != b[i]){
            return false;
        }
    }
    return true;
}
bool Route::operator==(const Route & route) const{
    return _parsedPath == route._parsedPath;
}
bool Route::operator<=(const Route & route) const{
    if(_parsedPath.size()>route._parsedPath.size()){return false;}
    return comparePathsInRange(_parsedPath,route._parsedPath,_parsedPath.size());
}
bool Route::operator>=(const Route &route) const{
    if(_parsedPath.size()<route._parsedPath.size()){return false;}
    return comparePathsInRange(_parsedPath,route._parsedPath,route._parsedPath.size());
}

std::vector<std::string> Route::extractParams(const Route & route){
    if(*this>=route){
        return std::vector<std::string>(_parsedPath.begin() + route._parsedPath.size(), _parsedPath.end());
    }
    return std::vector<std::string>{};
}