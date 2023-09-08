#ifndef __YF_STREAM_APPEND_H__
#define __YF_STREAM_APPEND_H__

#include "append.h"
#include "mutex.h"

namespace yf {
namespace log {
/**
 * @param 线程安全的标准输出，使用互斥锁实现。在不考虑性能问题可以使用
 */
class outstream_append : public append {
 public:
  /**
   * @brief 创建append
   * @param lv append最小日志等级限制
   * @param fmt 格式化器
   */
  outstream_append(format *fmt = nullptr, level lv = INFO);

 protected:
  /**
   * @brief 日志输出由子类实现具体功能
   * @param log_str 输出日志
   */
  void _handle(const std::string &log_str) override;

 private:
  mutex _mutex;
};

/**
 * @param 线程安全的标准错误，使用互斥锁实现。在不考虑性能问题可以使用
 */
class errstream_append : public append {
 public:
  /**
   * @brief 创建append
   * @param lv append最小日志等级限制
   * @param fmt 格式化器
   */
  errstream_append(format *fmt = nullptr, level lv = INFO);

 protected:
  /**
   * @brief 日志输出由子类实现具体功能
   * @param log_str 输出日志
   */
  void _handle(const std::string &log_str) override;

 private:
  mutex _mutex;
};
}  // namespace log
}  // namespace yf

#endif