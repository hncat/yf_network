#ifndef __YF_PROCESS_H__
#define __YF_PROCESS_H__

#include <sys/types.h>

#include <string>

#include "noncopyable.h"
#include "singleton.h"

namespace yf {
class process : noncopyable {
  SINGLETON_FRIEND_CLASS(process);

 public:
  /**
   * @returns 进程id
   */
  inline const std::string &pid_str() const { return _pid_str; }
  /**
   * @returns 进程id
   */
  inline const char *pid_c_str() const { return _pid_str.c_str(); }
  /**
   * @returns 进程id
   */
  pid_t pid() const { return _pid; }
  /**
   * @return 当前时间点
   */
  static time_t now_time();

 private:
  /**
   * @brief 私有化(单例模板调用)
   */
  process();

 private:
  pid_t _pid;            // 进程id
  std::string _pid_str;  // 进程id
};

typedef singleton<process>::type process_t;
static auto process_ptr = process_t::instance();
}  // namespace yf

#endif