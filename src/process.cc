#include "process.h"

#include <sys/time.h>
#include <unistd.h>

namespace yf {
process::process() : _pid(getpid()), _pid_str(std::to_string(_pid)) {}

time_t process::now_time() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return (tv.tv_sec + tv.tv_usec / 1000000);
}
}  // namespace yf