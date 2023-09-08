#include "socket.h"

#include <unistd.h>

#include "address.h"

namespace yf {
socket::socket(const std::string &__ip, uint16_t __port, uint16_t __family)
    : _fd{-1}, _addr{address::create_address(__ip, __port, __family)} {}

socket::socket(const address &_addr)
    : _fd{-1}, _addr{address::create_address(_addr.addr())} {}

socket::socket(const sockaddr *__addr)
    : _fd{-1}, _addr{address::create_address(__addr)} {}

socket::~socket() {
  if (_fd >= 0) {
    ::close(_fd);
  }
  delete _addr;
}

std::string socket::ip() const { return _addr->ip(); }

void socket::ip(const std::string &__ip) { _addr->ip(__ip); }

uint16_t socket::port() const { return _addr->port(); }

void socket::port(uint16_t __port) { _addr->port(__port); }
}  // namespace yf