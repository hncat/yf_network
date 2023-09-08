#include "log.h"
#include "stream_append.h"
#include "buffer.h"

int main() {
  LOG_ADD_APPEND("out", new yf::log::outstream_append);
  yf::buffer *buf = new yf::buffer{32};
  buf->write(10);
  int rint = 0;
  buf->read(rint);
  LOG_INFO << rint;
  char str[] = "hello world!";
  buf->write(str, sizeof(str));
  char rstr[sizeof(str)];
  buf->read(rstr, sizeof(rstr));
  LOG_INFO << rstr;
  delete buf;
  return 0;
}