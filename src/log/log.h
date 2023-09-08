#ifndef __YF_LOG_H__
#define __YF_LOG_H__

#include <string>
#include <unordered_map>

#include "log_stream.h"
#include "mutex.h"
#include "noncopyable.h"
#include "record.h"
#include "singleton.h"

namespace yf {
namespace log {
#define LOG_ADD_APPEND(name, append) \
  yf::log::logger_ptr->add_append(name, append)
#define LOG_DEL_APPEND(name) yf::log::logger_ptr->del_append(name)
#define LOG_CLEAR_APPEND() yf::log::logger_ptr->clear_append()

#define LOG_FMT_NONE(fmt, ...) \
  yf::log::logger_ptr->log(yf::log::NONE, fmt, ##__VA_ARGS__)
#define LOG_FMT_DEBUG(fmt, ...) \
  yf::log::logger_ptr->log(yf::log::DEBUG, fmt, ##__VA_ARGS__)
#define LOG_FMT_INFO(fmt, ...) \
  yf::log::logger_ptr->log(yf::log::INFO, fmt, ##__VA_ARGS__)
#define LOG_FMT_WARNING(fmt, ...) \
  yf::log::logger_ptr->log(yf::log::WARNING, fmt, ##__VA_ARGS__)
#define LOG_FMT_ERROR(fmt, ...) \
  yf::log::logger_ptr->log(yf::log::ERROR, fmt, ##__VA_ARGS__)
#define LOG_FMT_FAILED(fmt, ...) \
  yf::log::logger_ptr->log(yf::log::FAILED, fmt, ##__VA_ARGS__)

#define LOG_NONE yf::log::logger_ptr->log(yf::log::NONE)
#define LOG_DEBUG yf::log::logger_ptr->log(yf::log::DEBUG)
#define LOG_INFO yf::log::logger_ptr->log(yf::log::INFO)
#define LOG_WARNING yf::log::logger_ptr->log(yf::log::WARNING)
#define LOG_ERROR yf::log::logger_ptr->log(yf::log::ERROR)
#define LOG_FAILED yf::log::logger_ptr->log(yf::log::FAILED)

class append;
/**
 * @brief 线程安全的append，建议使用前初始化好需要的append
 * 内部通过使用unordered_map管理所有的append，通过使用mutex保证线程安全
 */
class logger : noncopyable {
  SINGLETON_FRIEND_CLASS(logger)
  friend class log_stream;

 public:
  /**
   * @brief 回收系统资源
   */
  virtual ~logger();
  /**
   * @brief 新增一个append
   * @param name append的名称
   * @param ptr append对象
   */
  void add_append(const std::string &name, append *ptr);
  /**
   * @brief 移除一个append
   * @param name 要移除的append名称
   */
  void del_append(const std::string &name);
  /**
   * @brief 清除所有append
   */
  void clear_append();
  /**
   * @brief c风格的日志输出
   * @param lv 日志等级
   * @param fmt 输出格式
   * @param ... 输出格式对应的输出参数
   */
  void log(level lv, const char *fmt, ...);
  /**
   * @brief 流式的日志输出
   */
  log_stream log(level lv);

 private:
  /**
   * @brief 查找并返回append
   * @param name append的名称
   * @returns append *
   */
  append *find_append(const std::string &name);
  /**
   * @brief 输出日志
   * @param lv 日志等级
   * @param message 日志信息
   */
  void _log(level lv, std::string message);
  /**
   * @brief 清除所有append(无锁)
   * @param appends 要清除的append集合
   */
  void unlock_clear_append(std::unordered_map<std::string, append *> &appends);

 private:
  /**
   * @brief 创建logger
   */
  logger() = default;

 private:
  mutex _mutex;
  std::unordered_map<std::string, append *> _append_map;
};

typedef singleton<logger>::type logger_t;
static auto logger_ptr = logger_t::instance();
}  // namespace log
}  // namespace yf

#endif