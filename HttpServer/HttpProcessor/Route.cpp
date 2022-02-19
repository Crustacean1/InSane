#include "Route.h"
#include "AsciiParser.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>

using namespace KHttp;

Route::Route(const std::string &route) { parse(route); }

bool Route::operator<=(const Route &route) const {
  return size() == match(route);
}

bool Route::operator>=(const Route &route) const {
  return route.size() == route.match(*this);
}

std::string Route::operator[](size_t i) const { return _parsedPath[i]; }
size_t Route::size() const { return _parsedPath.size(); }

void Route::parse(const std::string &route) {
  auto protocolEnd = route.find("://");
  if (protocolEnd == std::string::npos) {
    protocolEnd = 0;
  } else {
    size_t protocolSepSize = 3;
    _protocol = route.substr(0, protocolEnd);
    protocolEnd += protocolSepSize;
  }

  auto domainEnd = route.find_first_of("/", protocolEnd);
  if (domainEnd == std::string::npos) {
    return;
  }
  _domain = route.substr(protocolEnd, domainEnd - protocolEnd);

  auto pathEnd = route.find("?");
  if (pathEnd == std::string::npos) {
    pathEnd = route.length();
  } else {
    parseParams(route.substr(pathEnd + 1));
  }
  parsePath(route.substr(domainEnd, pathEnd - domainEnd));
}

void Route::parseParams(const std::string &params) {
  size_t prev, next;
  prev = 0;
  std::string pair;
  for (next = params.find("&", prev + 1); next != std::string::npos;
       next = params.find("&", prev + 1)) {
    pair = params.substr(prev, next - prev);
    parsePair(pair);
    prev = next + 1;
  }
  pair = params.substr(prev);
  parsePair(pair);
}

void Route::parsePair(const std::string &pair) {
  size_t split = pair.find("=");
  if (split == std::string::npos) {
    throw std::runtime_error(
        "Error in path format, key value pairs must be divided by '=': \n" +
        pair);
  }
  std::string key = pair.substr(0, split);
  std::string value = pair.substr(split + 1);
  AsciiParser::unescapeString(key);
  AsciiParser::unescapeString(value);
  _params[key] = value;
}

void Route::parsePath(std::string path) {
  path.erase(path.begin(), std::find_if(path.begin(), path.end(),
                                        [](char chr) { return chr != '/'; }));
  path.erase(std::find_if(path.rbegin(), path.rend(),
                          [](char chr) { return chr != '/'; })
                 .base(),
             path.end());

  size_t prev, next;
  prev = 0;

  std::string subpath;
  for (next = path.find("/", prev); next != std::string::npos;
       next = path.find("/", prev)) {
    if (next == prev + 1) {
      continue;
    }
    subpath = path.substr(prev, next - prev);
    AsciiParser::unescapeString(subpath);
    _parsedPath.push_back(subpath);
    prev = next + 1;
  }

  subpath = path.substr(prev);
  AsciiParser::unescapeString(subpath);
  _parsedPath.push_back(subpath);
}

Route::Route(const std::vector<std::string> &vec) : _parsedPath(vec) {}

Route::Route(Route &&route) : _parsedPath(std::move(route._parsedPath)) {}
Route::Route(const Route &route) : _parsedPath(route._parsedPath) {}

size_t Route::match(const Route &route) const {
  for (size_t i = 0; i < size() && i < route.size(); ++i) {
    if (_parsedPath[i] != route._parsedPath[i]) {
      return i;
    }
  }
  return std::min(size(), route.size());
}

Route Route::extractParams(const Route &route) const {
  if (match(route) == size()) {
    return Route(std::vector<std::string>(route._parsedPath.begin() + size(),
                                          route._parsedPath.end()));
  }
  return Route(std::vector<std::string>{});
}

void Route::debug() const {
  std::cout << "----- ROUTE DUMP -----" << std::endl;
  std::cout << "protocol: " << _protocol << "\t domain: " << _domain
            << std::endl;
  std::cout << "path:\n";
  for (const auto &e : _parsedPath) {
    std::cout << "route: " << e << std::endl;
  }
  std::cout << "params: " << std::endl;
  for (const auto &[key, value] : _params) {
    std::cout << "param: " << key << " = " << value << std::endl;
  }
  std::cout << "----- END OF ROUTE DUMP -----" << std::endl;
}
