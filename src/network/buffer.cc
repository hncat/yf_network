#include "buffer.h"

#include <memory.h>

#include "util.h"

namespace yf {
buffer::buffer(uint32_t size)
    : _size{size}, _head{0}, _tail{0}, _buffer{new char[_size]} {}

buffer::~buffer() { delete[] _buffer; }

void buffer::write(const void *data, uint32_t size) {
  uint32_t unused = unused_size();
  // 扩展buffer
  if (size > unused) expand_buffer(size - unused);
  ::memcpy(_buffer + _tail, data, size);
  _tail += size;
  adjust_buffer();
}

void buffer::write(const std::string &data) {
  write(data.c_str(), data.size());
}

void buffer::read(void *data, uint32_t size) {
  uint32_t vaild = vaild_size();
  // 小概率(一般情况下都是小于或等于)
  if (unlikely(size > vaild)) size = vaild;
  ::memcpy(data, _buffer + _head, size);
  _head += size;
  adjust_buffer();
}

void buffer::expand_buffer(uint32_t size) {
  _size += size;
  char *buffer = new char[_size];
  uint32_t vaild = vaild_size();
  ::memcpy(buffer, _buffer + _head, vaild);
  delete[] _buffer;
  _buffer = buffer;
  _head = 0;
  _tail = vaild;
}

void buffer::adjust_buffer() {
  uint32_t adjust_size = _size / 4;
  // 头部可复用空间占比超过1/4
  if (_head < adjust_size) return;
  // 尾部可用空间小于1/4
  if (unused_size() > adjust_size) return;
  // 当满足以上条件时既内存可复用
  uint32_t size = vaild_size();
  ::memcpy(_buffer, _buffer + _head, size);
  _head = 0;
  _tail = size;
}
}  // namespace yf