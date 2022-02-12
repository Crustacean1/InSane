#ifndef ROUTE
#define ROUTE

#include <vector>
#include <string>

namespace KHttp{
    class Route{
        std::vector<std::string> _parsedPath;
        bool comparePathsInRange(const std::vector<std::string> & a,const std::vector<std::string> & b,size_t range) const;
        friend std::hash<Route>;
        public:
        Route(const std::string &route);
        Route(Route && route);
        Route(const Route & route);
        bool operator==(const Route & route) const;
        bool operator<=(const Route & route) const;
        bool operator>=(const Route & route) const;
        std::vector<std::string> extractParams(const Route & baseRoute);
    };
}

#endif /*ROUTE*/