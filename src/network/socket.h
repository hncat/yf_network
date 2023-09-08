#ifndef __YF_SOCKET_H__
#define __YF_SOCKET_H__

#include <string>

#include "noncopyable.h"

namespace yf {
class address;
class socket : noncopyable {
 public:
  /**
   * @param __ip ip地址
   * @param __port 端口
   * @param __family ip蔟类型
  */
  socket(const std::string &__ip, uint16_t __port, uint16_t __family);
  /**
   * @param __addr address
   */
  socket(const address &__addr);
  /**
   * @param __addr ip/port结构
   */
  socket(const sockaddr *__addr);
  /**
   * @brief 析构
   */
  virtual ~socket();
  /**
   * @returns socket文件描述符
   */
  inline int fd() const { return _fd; }
  /**
   * @returns address refrence
   */
  inline const address &addr() const { return *_addr; }
  /**
   * @returns ip地址
   */
  std::string ip() const;
  /**
   * @brief 设置ip地址
   * @param __ip ip地址
   */
  void ip(const std::string &__ip);
  /**
   * @returns 端口
   */
  uint16_t port() const;
  /**
   * @brief 设置端口
   * @param __port 端口
   */
  void port(uint16_t port);

 private:
  int _fd;
  address *_addr;
};
}  // namespace yf

#endif