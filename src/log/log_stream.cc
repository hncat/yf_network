#include "log_stream.h"

#include "log.h"

namespace yf {
namespace log {
log_stream::log_stream(level lv, logger *log) : _level{lv}, _logger{log} {}

log_stream::~log_stream() { _logger->_log(_level, std::move(_message)); }
}  // namespace log
}  // namespace yf