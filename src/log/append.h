#ifndef __YF_APPEND_H__
#define __YF_APPEND_H__

#include "format.h"
#include "noncopyable.h"
#include "record.h"

namespace yf {
namespace log {
class append : public noncopyable {
 public:
  /**
   * @brief 创建append
   * @param lv append最小日志等级限制
   * @param fmt 格式化器
   */
  append(format *fmt = nullptr, level lv = INFO);
  /**
   * @brief 回收append使用过程中申请的各类系统资源
   */
  virtual ~append();
  /**
   * @return 日志等级限制
   */
  inline level get_level() const { return _level; }
  /**
   * @brief 设置日志等级
   * @param lv 日志等级
   */
  inline void set_level(level lv) { _level = lv; }
  /**
   * @returns 格式化器
   */
  inline format *get_format() { return _fmt; }
  /**
   * @brief 设置格式
   */
  void set_format(format *fmt);
  /**
   * @brief 输出日志信息
   * @param r 日志单元
   */
  void log(const record &r);

 protected:
  /**
   * @brief 日志输出由子类实现具体功能
   * @param log_str 输出日志
   */
  virtual void _handle(const std::string &log_str) = 0;

 private:
  level _level;  // 日志等级限制
  format *_fmt;  // 格式化日志

 protected:
  std::string _log_str;  // 日志信息
};
}  // namespace log
}  // namespace yf

#endif