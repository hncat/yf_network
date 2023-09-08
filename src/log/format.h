#ifndef __YF_FORMAT_H__
#define __YF_FORMAT_H__

#include <string>
#include <vector>

#include "noncopyable.h"

namespace yf {
namespace log {
struct format_item;
class record;
/**
 * @brief 日志的格式化(不可copy，非线程安全)
 * %d 日志时间以及日期
 * %p 进程id
 * %t 线程id
 * %l 日志等级
 * %m 日志信息
 * %n 换行
 */
class format : noncopyable {
  typedef format_item *base_item_ptr;

 public:
  /**
   * @brief 创造自定义格式化
   * @param fmt_str 自定义字符串
   * @param special 一些需要打印的特殊字符
   */
  format(std::string fmt_str = "[%d{}]<%p %t>[%l] %m%n",
         std::string special = " []<>:");
  /**
   * @return 自定义格式化字符串
   */
  inline const std::string &fmt_str() const { return _fmt_str; }
  /**
   * @brief 设置新的格式化
   * @param fmt_str 自定义字符串
   */
  void fmt_str(std::string fmt_str);
  /**
   * @returns 需要打印的特殊字符
   */
  inline const std::string &special() const { return _special; }
  /**
   * @brief 插入需要打印的特殊字符
   * @param c 插入的字符
   */
  void special(char c);
  /**
   * @brief 格式化数据输出
   * @param out 输出字符串
   * @param log 日志基础单元
   */
  void fmt(std::string &out, const record &log);
  /**
   * @brief 回收格式化类中申请的系统资源
   */
  virtual ~format();

 private:
  /**
   * @brief 初始化
   */
  void init_fmt();

 private:
  std::string _fmt_str;               // 格式字符串
  std::string _special;               // 需要打印的特殊字符
  std::vector<base_item_ptr> _items;  // 按序存放的格式化单元
};
}  // namespace log
}  // namespace yf

#endif