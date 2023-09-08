#include "file_append.h"

#include <assert.h>
#include <fcntl.h>
#include <sys/eventfd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace yf {
namespace log {
file_append::file_append(std::string file_name, format *fmt, level lv)
    : append{fmt, lv}, _fd{-1}, _file_name{std::move(file_name)} {
  _fd = ::open(_file_name.c_str(), O_CREAT | O_CLOEXEC | O_APPEND | O_WRONLY,
               0666);
  assert(_fd >= 0);
}

file_append::~file_append() { ::close(_fd); }

void file_append::_handle(const std::string &log_str) { ::write(_fd, log_str.c_str(), log_str.size()); }
}  // namespace log
}  // namespace yf