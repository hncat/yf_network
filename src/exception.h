#ifndef __YF_EXCEPTION_H__
#define __YF_EXCEPTION_H__

#include <exception>
#include <string>

namespace yf {
namespace log {
class format_empty_exception : public std::exception {
 public:
  const char *what() const noexcept { return "format string is empty!!!"; }
};

class format_error_exception : public std::exception {
 public:
  format_error_exception(std::string error) noexcept
      : std::exception{}, _error{std::move(error)} {}
  format_error_exception(const char *error) noexcept
      : std::exception{}, _error{error} {}
  virtual ~format_error_exception() noexcept = default;
  format_error_exception(const format_error_exception &error)
      : std::exception{}, _error{error._error} {}
  format_error_exception &operator=(const format_error_exception &error) {
    _error = error._error;
    return *this;
  }
  format_error_exception(format_error_exception &&error)
      : std::exception{}, _error{std::move(error._error)} {}
  format_error_exception &operator=(format_error_exception &&error) {
    _error = std::move(error._error);
    return *this;
  }
  const char *what() const noexcept { return _error.c_str(); }

 private:
  std::string _error;
};
}  // namespace log
}  // namespace yf

#endif