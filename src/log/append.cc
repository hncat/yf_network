#include "append.h"

namespace yf {
namespace log {
append::append(format *fmt, level lv)
    : _level{lv}, _fmt{fmt ? fmt : new format{}} {}

append::~append() { delete _fmt; }

void append::set_format(format *fmt) {
  if (_fmt) delete _fmt;
  _fmt = fmt;
}

void append::log(const record &r) {
  if (r._level < _level) return;
  std::string out;
  _fmt->fmt(out, r);
  _handle(out);
}
}  // namespace log
}  // namespace yf