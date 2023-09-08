#ifndef __YF_BUFFER_H__
#define __YF_BUFFER_H__

#include <stdint.h>

#include <string>

#include "noncopyable.h"
#include "swapbyte.h"
#include "type_traits.h"

namespace yf {
class buffer : noncopyable {
 public:
  /**
   * @brief 构造buffer
   * @param size buffer的初始大小
   */
  buffer(uint32_t __size = 1024);
  /**
   * @brief 析构
   */
  virtual ~buffer();
  /**
   * @returns 数据开始位置
   */
  inline const char *head() const { return _buffer + _head; }
  /**
   * @returns 数据结束位置
   */
  inline const char *tail() const { return _buffer + _tail; }
  /**
   * @returns 有效数据长度
   */
  inline uint32_t vaild_size() const { return _tail - _head; }
  /**
   * @returns 未使用数据长度
   */
  inline uint32_t unused_size() const { return _size - _tail; }
  /**
   * @returns 总长度
   */
  inline uint32_t size() const { return _size; }
  /**
   * @returns buffer的裸指针
   */
  inline const char *buffer_ptr() const { return _buffer; }
  /**
   * @brief 写入数据
   * @param data 数据内容
   * @param size 数据长度
   */
  void write(const void *data, uint32_t size);
  /**
   * @brief 写入字符串数据
   * @param data 要写入的字符串数据
   */
  void write(const std::string &data);
  /**
   * @brief 写入(2\4\8)byte字节数据(基础数据类型)
   * @param v 要写入的数据
   */
  template <typename T,
            typename = std::enable_if_t<
                is_same_byte_order_v<T> && !std::is_pointer<T>::value &&
                !std::is_reference<T>::value && !std::is_class<T>::value>>
  void write(T v) {
    v = big_endian(v);
    write(&v, sizeof(T));
  }
  /**
   * @brief 读取数据
   * @param data 数据存放内存地址
   * @param size 读取数据长度
   */
  void read(void *data, uint32_t size);
  /**
   * @brief 读取(2\4\8)byte字节数据(基础数据类型)
   * @param v 存放数据的位置
   */
  template <typename T,
            typename = std::enable_if_t<
                is_same_byte_order_v<T> && !std::is_pointer<T>::value &&
                !std::is_reference<T>::value && !std::is_class<T>::value>>
  void read(T &v) {
    read(&v, sizeof(T));
#ifdef YF_LITTLE_ENDIAN
    v = byteswap(v);
#endif
  }

 private:
  /**
   * @brief 扩展缓冲区
   * @param size 追加的缓冲大小
   */
  void expand_buffer(uint32_t size);
  /**
   * @brief 调整buffer
   */
  void adjust_buffer();

 private:
  uint32_t _size;  // buffer大小
  uint32_t _head;  // 开始位置
  uint32_t _tail;  // 结束位置
  char *_buffer;   // buffer地址
};
}  // namespace yf

#endif