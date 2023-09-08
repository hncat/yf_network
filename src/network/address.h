#ifndef __YF_ADDRESS_H__
#define __YF_ADDRESS_H__

#include <arpa/inet.h>

#include <string>
#include <unordered_map>
#include <vector>

/**
 * @brief 对ip地址和port的封装
 */
#include "noncopyable.h"

namespace yf {
class address : noncopyable {
 public:
  address() = default;
  virtual ~address() = default;
  /**
   * @returns ip地址
   */
  virtual std::string ip() const = 0;
  /**
   * @returns 设置ip地址
   * @param __ip ip地址
   */
  virtual void ip(const std::string &__ip) = 0;
  /**
   * @returns 端口
   */
  virtual uint16_t port() const = 0;
  /**
   * @brief 设置端口
   * @param __port 端口
   */
  virtual void port(uint16_t __port) = 0;
  /**
   * @returns 地址蔟
   */
  virtual uint16_t family() const = 0;
  /**
   * @returns 地址类型名称
   */
  virtual std::string type_name() const = 0;
  /**
   * @returns sockaddr
   */
  virtual const sockaddr *addr() const = 0;
  /**
   * @brief 设置ip/port结构
   * @param __addr ip/port结构
   */
  virtual void addr(const sockaddr *__addr) = 0;

 public:
  /**
   * @brief 通过域名解析获取address
   * @param __addrs address集合信息
   * @param __host 域名相关信息(www.baidu.com[http|https|ftp])
   */
  static void host_address(std::vector<address *> &__addrs,
                           const std::string &__host);
  /**
   * @brief 获取网卡
   * @param __addrs address集合信息
   */
  static void interface_address(
      std::unordered_map<std::string, address *> &__addrs);
  /**
   * @brief 创建address
   * @param __addr ip/port 结构
   * @brief address *
   */
  static address *create_address(const sockaddr *__addr);
  /**
   * @brief 创建address
   * @param __ip ip地址
   * @param __port 端口
   * @param __family ip蔟类型
   * @brief address *
   */
  static address *create_address(const std::string &__ip, uint16_t __port,
                                 uint16_t __family);
};

class ipv4_address : public address {
 public:
  /**
   * @param __ip ip地址
   * @param __port 端口号
   */
  ipv4_address(const std::string &__ip, uint16_t __port);
  /**
   * @param __port 端口号
   * @param __ip ip地址
   */
  ipv4_address(uint16_t __port, uint32_t __ip = INADDR_LOOPBACK);
  /**
   * @param __addr ip/port结构
   */
  ipv4_address(const sockaddr *__addr);
  virtual ~ipv4_address() = default;
  /**
   * @returns ip地址
   */
  std::string ip() const override;
  /**
   * @returns 设置ip地址
   * @param __ip ip地址
   */
  virtual void ip(const std::string &__ip) override;
  /**
   * @returns 端口
   */
  virtual uint16_t port() const override;
  /**
   * @brief 设置端口
   * @param __port 端口
   */
  virtual void port(uint16_t __port) override;
  /**
   * @returns 地址蔟
   */
  virtual uint16_t family() const override { return AF_INET; }
  /**
   * @returns 地址类型名称
   */
  virtual std::string type_name() const { return "ipv4"; };
  /**
   * @returns sockaddr
   */
  virtual const sockaddr *addr() const override;
  /**
   * @brief 设置ip/port结构
   * @param __addr ip/port结构
   */
  virtual void addr(const sockaddr *__addr) override;

 private:
  sockaddr_in _addr;
};

class ipv6_address : public address {
 public:
  /**
   * @param __ip ip地址
   * @param __port 端口号
   */
  ipv6_address(const std::string &__ip, uint16_t __port);
  /**
   * @param __addr ip/port结构
   */
  ipv6_address(const sockaddr *__addr);
  virtual ~ipv6_address() = default;
  /**
   * @returns ip地址
   */
  std::string ip() const override;
  /**
   * @returns 设置ip地址
   * @param __ip ip地址
   */
  virtual void ip(const std::string &__ip) override;
  /**
   * @returns 端口
   */
  virtual uint16_t port() const override;
  /**
   * @brief 设置端口
   * @param __port 端口
   */
  virtual void port(uint16_t __port) override;
  /**
   * @returns 地址蔟
   */
  virtual uint16_t family() const override { return AF_INET6; }
  /**
   * @returns 地址类型名称
   */
  virtual std::string type_name() const { return "ipv6"; };
  /**
   * @returns sockaddr
   */
  virtual const sockaddr *addr() const override;
  /**
   * @brief 设置ip/port结构
   * @param __addr ip/port结构
   */
  virtual void addr(const sockaddr *__addr) override;

 private:
  sockaddr_in6 _addr;
};
}  // namespace yf

#endif