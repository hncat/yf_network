#include "format.h"

#include <sstream>

#include "exception.h"
#include "record.h"

namespace yf {
namespace log {
/**
 * @brief 自定义格式化的最小单元
 */
struct format_item {
  format_item() = default;
  virtual ~format_item() = default;
  /**
   * @brief 格式化数据输出
   * @param out 输出字符串
   * @param log 日志基础单元
   */
  virtual void fmt(std::string &out, const record &log) = 0;
};

/**
 * @brief 日期
 */
struct date_format : public format_item {
  date_format(std::string format) : format_item{}, _format{std::move(format)} {
    if (_format.empty()) {
      _format = "%Y:%m:%d %H:%M:%S";
    }
  }
  void fmt(std::string &out, const record &log) override {
    struct tm t;
    localtime_r((time_t *)&log._time, &t);
    char buf[32]{0};
    strftime(buf, sizeof(buf), _format.c_str(), &t);
    out += buf;
  }
  std::string _format;
};

/**
 * @brief 进程id
 */
struct process_format : public format_item {
  void fmt(std::string &out, const record &log) override {
    out += log._process_id;
  }
};

/**
 * @brief 线程id
 */
struct thread_format : public format_item {
  void fmt(std::string &out, const record &log) override {
    out += log._thread_id;
  }
};

/**
 * @brief 日志等级
 */
struct level_format : public format_item {
  void fmt(std::string &out, const record &log) override {
    out += get_level_name(log._level);
  }
};

/**
 * @brief 日志信息
 */
struct message_format : public format_item {
  void fmt(std::string &out, const record &log) override {
    out += log._message;
  }
};

/**
 * @brief 换行
 */
struct enter_format : public format_item {
  void fmt(std::string &out, const record &log) override { out += '\n'; }
};

/**
 * @brief 自定义字符串数据
 */
struct string_format : public format_item {
  string_format(std::string str) : format_item{}, _str{std::move(str)} {}
  void fmt(std::string &out, const record &log) override { out += _str; }
  std::string _str;
};

format::format(std::string fmt_str, std::string special)
    : _fmt_str{std::move(fmt_str)}, _special{std::move(special)} {
  init_fmt();
}

void format::fmt_str(std::string fmt_str) { init_fmt(); }

void format::special(char c) {
  if (_special.find(c) != std::string::npos) return;
  _special.push_back(c);
}

void format::fmt(std::string &out, const record &log) {
  for (auto it : _items) {
    it->fmt(out, log);
  }
}

format::~format() {
  for (auto it : _items) {
    if (it) delete it;
  }
}

void format::init_fmt() {
  if (_fmt_str.empty()) {
    throw format_empty_exception{};
  }
  // 格式字符串处理
  std::size_t begin = 0;
  std::string special_str{""};
  std::stringstream err_stream;
  do {
    if (_fmt_str[begin] == '%') {
      if (!special_str.empty()) {
        _items.push_back(new string_format{special_str});
        special_str.clear();
      }
      if (begin >= _fmt_str.size() - 1) {
        // 结尾的 % 异常抛出
        err_stream << "% appears at the end";
        throw format_error_exception{err_stream.str()};
      }
      ++begin;
      // 匹配字符
      switch (_fmt_str[begin]) {
#define XXX(c, item, ...)                      \
  case #c[0]:                                  \
    _items.push_back(new item{##__VA_ARGS__}); \
    break;
        XXX(p, process_format)
        XXX(t, thread_format)
        XXX(l, level_format)
        XXX(m, message_format)
        XXX(n, enter_format)
#undef XXX
        case 'd': {
          // {%y:%m:%d %H:%M:%S} 日期格式
          ++begin;
          if (_fmt_str[begin] == '{') {
            auto pos = _fmt_str.find('}');
            if (pos == std::string::npos) {
              err_stream << "index[" << begin << "] :'" << _fmt_str[begin]
                         << "' No matching '}'";
              throw format_error_exception{err_stream.str()};
            } else {
              auto date_str = _fmt_str.substr(begin + 1, pos - begin - 1);
              _items.push_back(new date_format{date_str});
              begin = pos;
            }
          } else {
            continue;
          }
        } break;
        default:
          // 任意形式都当做需要打印出的特殊字符
          special_str.push_back(_fmt_str[begin]);
          break;
      }
    } else {
      // 只处理一些特定字符
      if (_special.find(_fmt_str[begin]) != std::string::npos) {
        special_str.push_back(_fmt_str[begin]);
      } else {
        err_stream << "wrong special character, index[" << begin
                   << "]: " << _fmt_str[begin];
        throw format_error_exception{err_stream.str()};
      }
    }
    ++begin;
  } while (begin < _fmt_str.size());
  if (!special_str.empty()) {
    _items.push_back(new string_format{special_str});
  }
}

}  // namespace log
}  // namespace yf