#ifndef __YF_FILE_APPEND_H__
#define __YF_FILE_APPEND_H__

#include "append.h"

namespace yf {
namespace log {
class file_append : public append {
 public:
  /**
   * @param file_name 文件名称
   * @param lv 日志等级
   * @param fmt 自定义格式化
   */
  file_append(std::string file_name, format *fmt = nullptr, level lv = INFO);
  /**
   * @brief 回收系统系统
   */
  virtual ~file_append();
  /**
   * @returns 文件名
   */
  inline const std::string &get_file_name() const { return _file_name; }

 protected:
  /**
   * @brief 日志输出由子类实现具体功能
   * @param log_str 输出日志
   */
  void _handle(const std::string &log_str) override;

 private:
  int _fd;                 // 日志的文件描述符
  std::string _file_name;  // 文件名称
};
}  // namespace log
}  // namespace yf

#endif