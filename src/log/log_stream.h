#ifndef __YF_LOG_STREAM_H__
#define __YF_LOG_STREAM_H__

#include <sstream>
#include <string>

#include "json.h"
#include "record.h"

namespace yf {
namespace log {
class logger;
/**
 * @brief 流式日志输出的工具类
 */
class log_stream {
 public:
  /**
   * @brief 创建log_stream
   * @param lv 日志等级
   * @param log 日志收集器
   */
  log_stream(level lv, logger *log);
  /**
   * @brief 回收系统资源
   */
  ~log_stream();

  template <typename T>
  inline log_stream &operator<<(T v) {
    _message.append(std::to_string(v));
    return *this;
  }

  template <typename T>
  inline log_stream &operator<<(T *v) {
    std::stringstream sstr;
    sstr << std::hex << (void *)v;
    _message += sstr.str();
    return *this;
  }

  inline log_stream &operator<<(char c) {
    _message.push_back(c);
    return *this;
  }

  inline log_stream &operator<<(char *c_str) {
    _message.append(c_str);
    return *this;
  }

  inline log_stream &operator<<(const char *c_str) {
    _message.append(c_str);
    return *this;
  }

  inline log_stream &operator<<(std::string &&str) {
    _message.append(str);
    return *this;
  }

  inline log_stream &operator<<(std::string &str) {
    _message.append(str);
    return *this;
  }

  inline log_stream &operator<<(const std::string &str) {
    _message.append(str);
    return *this;
  }

  inline log_stream &operator<<(nlohmann::json &json) {
    _message += json;
    return *this;
  }

 private:
  level _level;            // 日志等级
  std::string _message{};  // 日志记录
  logger *_logger;         // 日志收集器
};
}  // namespace log
}  // namespace yf

#endif