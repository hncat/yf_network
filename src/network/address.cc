#include "address.h"

#include <ifaddrs.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>

#include "log.h"
#include "swapbyte.h"

namespace yf {
void address::host_address(std::vector<address *> &__addrs,
                           const std::string &__host) {
  std::string host;
  std::vector<std::string> services;
  auto service_pos = __host.find('[');
  if (service_pos == std::string::npos) {
    host = __host;
  } else {
    host = __host.substr(0, service_pos);
    auto end_pos = __host.size() - 1;
    if (__host[end_pos] != ']') {
      LOG_ERROR << "host address error no find ']'!!!";
      return;
    }
    service_pos += 1;
    std::string service_str = __host.substr(service_pos, end_pos - service_pos);
    LOG_INFO << service_str;
    do {
      auto node_pos = service_str.find('|');
      if (node_pos == std::string::npos) {
        services.push_back(service_str);
        break;
      }
      services.push_back(service_str.substr(0, node_pos));
      auto size = service_str.size();
      node_pos += 1;
      service_str = service_str.substr(node_pos, size - node_pos);
    } while (true);
  }
  if (services.empty()) {
    services.push_back("");
  }
  for (auto service : services) {
    struct addrinfo hint, *ailist;
    ::bzero(&hint, sizeof(addrinfo));
    // 查找所有ipv4和ipv6
    hint.ai_flags = AI_ALL | AI_V4MAPPED;
    int herror = ::getaddrinfo(host.c_str(), service.c_str(), &hint, &ailist);
    if (herror) {
      LOG_ERROR << "getaddrinfo error: " << ::gai_strerror(herror);
      continue;
    }
    struct addrinfo *next = ailist;
    while (next) {
      auto addr = create_address(next->ai_addr);
      if (addr) __addrs.push_back(addr);
      next = next->ai_next;
    }
    ::freeaddrinfo(ailist);
  }
}

void address::interface_address(
    std::unordered_map<std::string, address *> &__addrs) {
  struct ifaddrs *ifap;
  if (::getifaddrs(&ifap) == -1) {
    LOG_ERROR << "interface address error: " << strerror(errno);
    return;
  }
  for (struct ifaddrs *ifa = ifap; ifa != nullptr; ifa = ifa->ifa_next) {
    if (nullptr == ifa->ifa_addr) continue;
    std::string ifa_name{ifa->ifa_name};
    auto it = __addrs.find(ifa_name);
    if (it != __addrs.end()) {
      LOG_WARNING << "duplicate NIC name! " << ifa_name;
      continue;
    }
    auto addr = create_address(ifa->ifa_addr);
    if (addr) {
      __addrs[ifa_name] = addr;
    } else {
      LOG_ERROR << "unidentified network card! " << ifa_name;
    }
  }
  ::freeifaddrs(ifap);
}

address *address::create_address(const sockaddr *__addr) {
  if (__addr->sa_family == AF_INET) {
    return new ipv4_address{__addr};
  } else if (__addr->sa_family == AF_INET6) {
    return new ipv6_address{__addr};
  } else {
    LOG_ERROR << "Unrecognized ip address type, family: " << __addr->sa_family;
    return nullptr;
  }
}

address *address::create_address(const std::string &__ip, uint16_t __port,
                                 uint16_t __family) {
  if (__family == AF_INET) {
    return new ipv4_address{__ip, __port};
  } else if (__family == AF_INET6) {
    return new ipv6_address{__ip, __port};
  } else {
    LOG_ERROR << "Unrecognized ip address type, family: " << __family
              << " ip: " << __ip << " port: " << __port;
    return nullptr;
  }
}

std::string ipv4_address::ip() const {
  char buf[32]{0};
  ::inet_ntop(AF_INET, &_addr.sin_addr.s_addr, buf, sizeof(buf));
  return buf;
}

void ipv4_address::ip(const std::string &__ip) {
  ::inet_pton(AF_INET, __ip.c_str(), &_addr.sin_addr.s_addr);
}

uint16_t ipv4_address::port() const {
#ifdef YF_BIG_ENDIAN
  reutrn _addr.sin_port;
#else
  return byteswap(_addr.sin_port);
#endif
}

void ipv4_address::port(uint16_t __port) {
  _addr.sin_port = big_endian(__port);
}

const sockaddr *ipv4_address::addr() const {
  return reinterpret_cast<const sockaddr *>(&_addr);
}

void ipv4_address::addr(const sockaddr *__addr) {
  ::memcpy(&_addr, __addr, sizeof(sockaddr_in));
}

ipv4_address::ipv4_address(const std::string &__ip, uint16_t __port) {
  _addr.sin_family = AF_INET;
  inet_pton(AF_INET, __ip.c_str(), &_addr.sin_addr.s_addr);
  _addr.sin_port = big_endian(__port);
}

ipv4_address::ipv4_address(uint16_t __port, uint32_t __ip) {
  _addr.sin_family = AF_INET;
  _addr.sin_addr.s_addr = big_endian(__ip);
  _addr.sin_port = big_endian(__port);
}

ipv4_address::ipv4_address(const sockaddr *__addr) {
  ::memcpy(&_addr, __addr, sizeof(sockaddr_in));
}

std::string ipv6_address::ip() const {
  char buf[32]{0};
  ::inet_ntop(AF_INET6, &_addr.sin6_addr.s6_addr, buf, sizeof(buf));
  return buf;
}

void ipv6_address::ip(const std::string &__ip) {
  ::inet_pton(AF_INET6, __ip.c_str(), &_addr.sin6_addr.s6_addr);
}

uint16_t ipv6_address::port() const {
#ifdef YF_BIG_ENDIAN
  reutrn _addr.sin6_port;
#else
  return byteswap(_addr.sin6_port);
#endif
}

void ipv6_address::port(uint16_t __port) {
  _addr.sin6_port = big_endian(__port);
}

const sockaddr *ipv6_address::addr() const {
  return reinterpret_cast<const sockaddr *>(&_addr);
}

void ipv6_address::addr(const sockaddr *__addr) {
  ::memcpy(&_addr, __addr, sizeof(sockaddr_in6));
}

ipv6_address::ipv6_address(const std::string &__ip, uint16_t __port) {
  _addr.sin6_family = AF_INET6;
  ::inet_pton(AF_INET6, __ip.c_str(), &_addr.sin6_addr.s6_addr);
  _addr.sin6_port = big_endian(__port);
}

ipv6_address::ipv6_address(const sockaddr *__addr) {
  ::memcpy(&_addr, __addr, sizeof(sockaddr_in6));
}

}  // namespace yf