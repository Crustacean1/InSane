#ifndef ROUTE
#define ROUTE

#include <vector>
#include <string>
#include <map>

namespace KHttp{
    class Route{
        std::map<std::string,std::string> _params;
        std::vector<std::string> _parsedPath;
        std::string _protocol;
        std::string _domain;

        bool comparePathsInRange(const std::vector<std::string> & a,const std::vector<std::string> & b,size_t range) const;
        
        void parse(const std::string &route);
        void parsePath(std::string path);
        void parseParams(const std::string & params);
        void parsePair(const std::string & str);


        friend int main(int,char**);
        public:
        Route(const std::string &route);
        Route(Route && route);
        Route(const Route & route);
        Route(const std::vector<std::string>& vec);

        Route extractParams(const Route & baseRoute) const;


        size_t match(const Route &route) const;
        size_t size() const;

        bool operator<=(const Route & route) const;
        bool operator>=(const Route & route) const;
        std::string operator[](size_t i) const;

        void debug() const;
    };
}

#endif /*ROUTE*/