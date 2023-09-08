#include <sys/time.h>

#include <iostream>

#include "file_append.h"
#include "log.h"
#include "stream_append.h"

time_t get_time() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void test_log_append(int line, int is_fmt) {
  LOG_ADD_APPEND("file append", new yf::log::file_append{"test.log"});
  if (is_fmt == 0) {
    // LOG_ADD_APPEND("out", new yf::log::outstream_append{});
    auto begin = get_time();
    for (int i = 0; i < line; ++i) {
      // LOG_FMT_INFO("hello logger: %d", 123);
      LOG_INFO << "hello logger: " << 123;
    }
    auto end = get_time();
    std::cout << "stream log: " << end - begin << " ms\n";
  } else {
    auto begin1 = get_time();
    for (int i = 0; i < line; ++i) {
      LOG_FMT_INFO("hello logger: %d", 123);
      // LOG_INFO << "hello logger: " << 123;
    }
    auto end1 = get_time();
    std::cout << "fmt log: " << end1 - begin1 << " ms\n";
  }

}

int main(int argc, char **argv) {
  if (argc < 3) {
    return 1;
  }
  auto line = atoi(argv[1]);
  auto is_fmt = atoi(argv[2]);
  test_log_append(line, is_fmt);
  return 0;
}