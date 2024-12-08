#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <cstring>
#include <memory>
#include <stdexcept>

template <typename Allocator = std::allocator<char>>
class String {
public:
  using size_type = typename std::allocator_traits<Allocator>::size_type;;
  static const size_t npos = -1;
private:
  char* m_arr = nullptr;
  size_type m_size = 0;
  size_type m_capacity = 0;
  Allocator alloc;
  size_type increase_by = 15;
public:
  String();
	String(const String&);
	String(const String& other, size_type pos, size_t len = npos);
	String(const char*);
	String(const char* s, size_t n);
	String(size_t n, char c);

  String& operator= (const String&);

  ~String();

  // String& append(const String& str);
	String& append(const String& str, size_type subpos, size_t sublen = npos);
	// String& append(const char* s);
	// String& append(const char* s, size_t n);
	// String& append(size_type n, char c);
private:
  void _setSize(const size_t size);
  void _setCapasity(const size_t cap);
  void _clearStr(const size_t pos);
  void _increaseCapacity(const size_t n);
  void _fillStr(char* other, const size_t len, const size_t pos, char c) const;
  void _allocateCString(char*& buffer, const size_t n);
  void _append(const char* other);
  void _append(const char* other, size_t n);
  size_t _getLength(const String& str, size_type pos, size_t len) const;
  void _substr(char*& buffer, const char* other, size_type pos, size_t len);
};

#endif

template <typename Allocator>
String<Allocator>::String()
{
  _setCapasity(0);
  _setSize(0);
}

template <typename Allocator>
String<Allocator>::String(const String& other)
{
  append(other.m_arr, other.m_size);
}

template <typename Allocator>
String<Allocator>::String(const String& other, size_type pos, size_t len)
{
  append(other, pos, len);
}

template <typename Allocator>
String<Allocator>::String(const char* copy)
{
}

template <typename Allocator>
String<Allocator>::String(const char* s, size_t n)
{
}

template <typename Allocator>
String<Allocator>::String(size_t n, char c)
{
}

template <typename Allocator>
String<Allocator>& String<Allocator>::operator=(const String &)
{
  
}

template <typename Allocator>
String<Allocator>::~String()
{
  delete m_arr;
  m_arr = nullptr;
  m_size = 0;
  m_capacity = 0;
  increase_by = 0;
}

template <typename Allocator>
void String<Allocator>::_setSize(const size_t size) 
{
  if (m_size > size) {
    _clearStr(size);
  }
  else if (m_size < size) {
    _increaseCapacity(size);
  }

  m_size = size;
}

template <typename Allocator>
void String<Allocator>::_setCapasity(const size_t cap)
{
  if (m_capacity == cap && m_arr) return;
  if (m_capacity > cap) return;
  m_capacity = cap;

  char* buffer = m_arr;
  m_arr = nullptr;
  _allocateCString(m_arr, m_capacity);
  if (buffer)
  {
    std::memcpy(m_arr, buffer, m_size);
    m_arr[m_size] = '\0';
  }

  delete buffer;
  buffer = nullptr;
}

template <typename Allocator>
void String<Allocator>::_clearStr(const size_t pos) 
{
  _fillStr(m_arr, m_size, pos, '\0');
  m_size = pos;
}

template <typename Allocator>
void String<Allocator>::_increaseCapacity(const size_t n)
{
  if (m_capacity > n && m_arr) return;

  size_type cap = m_capacity;
  while (cap <= n) {
    cap += increase_by;
  }
  ++increase_by;

  _setCapasity(cap);
}

template <typename Allocator>
void String<Allocator>::_fillStr(char* other, const size_t len, const size_t pos, char c) const
{
  // size_type begin = pos;
  // while (begin != len)
  // {
  //   other[begin++] = c;
  // }

  // other[begin] = '\0';
  std::memset(other + pos, c, len - pos);
  other[len] = '\0';
}

template <typename Allocator>
void String<Allocator>::_allocateCString(char*& buffer, const size_t n) {
    if (buffer) {
        throw std::runtime_error("Buffer is already allocated");
    }

    buffer = alloc.allocate(n + 1);

    std::uninitialized_fill_n(buffer, n + 1, '\0');
}

template <typename Allocator>
String<Allocator>& String<Allocator>::append(const String& str, size_type subpos, size_t sublen)
{
  sublen = _getLength(str, subpos, sublen);
  char* buffer = nullptr;
  _substr(buffer, str.m_arr, subpos, sublen);
  _append(buffer, sublen);
  delete buffer;
  buffer = nullptr;
  return *this;
}

template <typename Allocator>
inline void String<Allocator>::_append(const char *other)
{
  append(other, strlen(other));
}

template <typename Allocator>
void String<Allocator>::_append(const char* other, size_t n)
{
  if (!other || n == 0) return;

  increaseCapacity(m_size + n);
  std::memcpy(m_arr + m_size, other, n);

  operator[](m_size + n) = '\0';

  _setLength(m_size + n);
}

template <typename Allocator>
size_t String<Allocator>::_getLength(const String& str, size_type pos, size_t len) const
{
  if (len == npos)
  {
    len = m_size - pos;
  }

  if (pos + len > str.m_size)
  {
    throw;
  }
  return len;
}

template <typename Allocator>
void String<Allocator>::_substr(char *&buffer, const char *other, size_type pos, size_t len)
{
  if (other == nullptr) {
        throw std::invalid_argument("Input string (other) is null.");
    }

    size_t other_len = std::strlen(other);
    if (pos > other_len) {
        throw std::out_of_range("Position is out of bounds.");
    }

    len = std::min(len, other_len - pos);

    _allocateCString(buffer, len);

    std::memcpy(buffer, other + pos, len);

    buffer[len] = '\0';
}
