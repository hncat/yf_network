#ifndef __YF_RECORD_H__
#define __YF_RECORD_H__

#include <string>

// 日常记录相关
namespace yf {
namespace log {
/**
 * @brief 日志等级
 */
enum level {
  NONE = 0,     // 任意等级
  DEBUG = 1,    // 调试
  INFO = 2,     // 基础信息
  WARNING = 3,  // 警告
  ERROR = 4,    // 异常
  FAILED = 5,   // 严重错误
};

const char *get_level_name(level lv);

/**
 * @brief 日志输出的基础单元
 */
struct record {
  level _level;             // 日志等级
  const char *_thread_id;   // 线程id
  const char *_process_id;  // 进程id
  uint64_t _time;           // 日志输出时间
  std::string _message;     // 日志信息
};
}  // namespace log
}  // namespace yf

#endif