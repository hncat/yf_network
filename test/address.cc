#include "address.h"

#include "log.h"
#include "stream_append.h"

int main(int argc, char **argv) {
  LOG_ADD_APPEND("out", new yf::log::outstream_append);
  std::vector<yf::address *> addrs;
  yf::address::host_address(addrs, "baidu.com[http|https]");
  for (auto addr : addrs) {
    LOG_INFO << addr->type_name() << '[' << addr->ip() << ':' << addr->port()
             << ']';
    delete addr;
  }
  std::unordered_map<std::string, yf::address *> addrs1;
  yf::address::interface_address(addrs1);
  for (auto it : addrs1) {
    auto addr = it.second;
    LOG_INFO << it.first << ':' << addr->type_name() << '[' << addr->ip() << ':'
             << addr->port() << ']';
    delete addr;
  }
  return 0;
}