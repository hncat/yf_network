#include "stream_append.h"

#include <iostream>

#include "lock.h"

namespace yf {
namespace log {
outstream_append::outstream_append(format *fmt, level lv) : append{fmt, lv} {}

void outstream_append::_handle(const std::string &log_str) {
  lock_guard<mutex> lock{_mutex};
  std::cout << log_str;
}

errstream_append::errstream_append(format *fmt, level lv) : append{fmt, lv} {}

void errstream_append::_handle(const std::string &log_str) {
  lock_guard<mutex> lock{_mutex};
  std::cerr << log_str;
}
}  // namespace log
}  // namespace yf