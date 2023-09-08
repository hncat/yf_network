#include "log.h"

#include <stdarg.h>

#include "append.h"
#include "lock.h"
#include "process.h"
#include "thread.h"

namespace yf {
namespace log {
#define INIT_LOG_RECORD(r, l, m)                      \
  r._level = l;                                       \
  r._message = std::move(m);                          \
  r._process_id = process_ptr->pid_c_str();           \
  r._thread_id = this_thread::id_to_string().c_str(); \
  r._time = process_ptr->now_time();

logger::~logger() { unlock_clear_append(_append_map); }

void logger::add_append(const std::string &name, append *ptr) {
  lock_guard<mutex> lock{_mutex};
  auto tmp = find_append(name);
  if (tmp) delete tmp;
  _append_map[name] = ptr;
}

void logger::del_append(const std::string &name) {
  lock_guard<mutex> lock{_mutex};
  if (!find_append(name)) return;
  _append_map.erase(name);
}

void logger::clear_append() {
  std::unordered_map<std::string, append *> _tmp;
  {
    lock_guard<mutex> lock{_mutex};
    _append_map.swap(_tmp);
  }
  unlock_clear_append(_tmp);
}

void logger::unlock_clear_append(
    std::unordered_map<std::string, append *> &appends) {
  for (auto it : appends) {
    delete it.second;
  }
}

append *logger::find_append(const std::string &name) {
  auto it = _append_map.find(name);
  if (it == _append_map.end()) return nullptr;
  return it->second;
}

void logger::log(level lv, const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);
  char *log_buf = nullptr;
  vasprintf(&log_buf, fmt, va);
  _log(lv, log_buf);
  delete log_buf;
  va_end(va);
}

void logger::_log(level lv, std::string message) {
  record r;
  INIT_LOG_RECORD(r, lv, message)
  for (auto it : _append_map) {
    it.second->log(r);
  }
}

log_stream logger::log(level lv) { return log_stream{lv, this}; }
}  // namespace log
}  // namespace yf