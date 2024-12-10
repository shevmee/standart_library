#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <algorithm>
#include <iostream>

template <
  typename CharT, 
  typename Traits = std::char_traits<CharT>, 
  typename Allocator = std::allocator<CharT>
>
class BasicString {
public:
  using value_type = CharT;
  using allocator_type = Allocator;
  using size_type = typename std::allocator_traits<allocator_type>::size_type;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
  using reference = CharT&;
  using const_reference = const CharT&;
  using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
  using traits_type = Traits;

  static const size_type npos = static_cast<size_type>(-1);
private:
  pointer data_;
  size_type size_;
  size_type capacity_;
  allocator_type allocator_;
public:
  BasicString();
	BasicString(const BasicString&);
	BasicString(const BasicString& other, size_type pos, size_type len = npos);
	BasicString(const CharT*);
	BasicString(size_t n, CharT c);

  BasicString& operator=(BasicString);

  ~BasicString();

  size_type find(const BasicString& sub, size_type pos = 0) const;
  int compare(const BasicString& other) const;
  bool starts_with(const BasicString& prefix) const;
  bool ends_with(const BasicString& sub) const;

  const_pointer c_str() const;
  constexpr size_type size() const;
  constexpr size_type length() const;
  constexpr size_type capacity() const;
  constexpr bool empty() const;
  constexpr void clear();
private:
  using allocator_traits_type = std::allocator_traits<allocator_type>;

  void allocate_and_copy(const CharT* str, size_t len);
  void deallocate();

  template <typename T, typename Tr, typename Al>
  friend std::ostream& operator<<(std::ostream& os, const BasicString<T, Tr, Al>& str);

  template <typename T, typename Tr, typename Al>
  friend std::istream& operator>>(std::istream& is, BasicString<T, Tr, Al>& str);
};

#endif

template <typename CharT, typename Traits, typename Allocator>
inline void BasicString<CharT, Traits, Allocator>::allocate_and_copy(const CharT* str, size_t len)
{ 
  data_ = allocator_traits_type::allocate(allocator_, len + 1);
  std::memcpy(data_, str, len * sizeof(CharT));
  data_[len] = '\0';
  size_ = len;
  capacity_ = len + 1;
}

template <typename CharT, typename Traits, typename Allocator>
inline void BasicString<CharT, Traits, Allocator>::deallocate()
{
  if (data_)
  {
    allocator_traits_type::deallocate(allocator_, data_, capacity_);
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
  }
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::BasicString()
    : data_(nullptr), size_(0), capacity_(0) {}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::BasicString(const BasicString& other)
    : data_(nullptr)
    , size_(other.size_)
    , capacity_(other.capacity_)
    , allocator_(other.allocator_) 
{
  if (size_ > 0)
  {
    data_ = allocator_traits_type::allocate(allocator_, capacity_);
    std::memcpy(data_, other.data_, size_ * sizeof(CharT));
    data_[size_] = '\0';
  } else {
    data_ = nullptr;
  }
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::BasicString(const BasicString &other, size_type pos, size_type len)
    : allocator_(other.allocator_)
{
  if (pos > other.size_) {
    throw std::out_of_range("Position is out of range.");
  }

  size_ = (len == npos) ? other.size_ - pos : std::min(len, other.size_ - pos);
  capacity_ = size_ + 1;
  data_ = allocator_traits_type::allocate(allocator_, capacity_);

  std::memcpy(data_, other.data_ + pos, size_ * sizeof(CharT));
  data_[size_] = '\0';
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::BasicString(const CharT* copy)
{
  size_type len = traits_type::length(copy);
  allocate_and_copy(copy, len);
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::BasicString(size_t n, CharT c)
{
  data_ = allocator_traits_type::allocate(allocator_, n + 1);
  std::memset(data_, c, n);
  size_ = n;
  capacity_ = n + 1;
  data_[size_] = '\0';
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::~BasicString()
{
  deallocate();
}

template <typename CharT, typename Traits, typename Allocator>
inline typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::find(const BasicString &sub, size_type pos) const
{
  if (pos > size_) return npos;

  // for (size_type i = pos; i <= size_ - sub.size_; ++i)
  // {
  //   bool match = true;
  //   for (size_type j = 0; j < sub.size_; ++j)
  //   {
  //     if (Traits::eq(data_[i + j], sub.data_[j]))
  //     {
  //       continue;
  //     } else {
  //       match = false;
  //       break;
  //     }
  //   }

  //   if (match) return i;
  // }

  // return npos;

  const_pointer start = c_str() + pos;
  const_pointer result = std::search(start, c_str() + size_, sub.c_str(), sub.c_str() + sub.size());
  return result != c_str() + size_ ? result - c_str() : npos;
}

template <typename CharT, typename Traits, typename Allocator>
inline int BasicString<CharT, Traits, Allocator>::compare(const BasicString &other) const
{
  return Traits::compare(c_str(), other.c_str(), std::min(size_, other.size_));
}

template <typename CharT, typename Traits, typename Allocator>
inline bool BasicString<CharT, Traits, Allocator>::starts_with(const BasicString &prefix) const
{
  if (prefix.size_ > size_) return false;

  return find(prefix, 0) == 0;
}

template <typename CharT, typename Traits, typename Allocator>
inline bool BasicString<CharT, Traits, Allocator>::ends_with(const BasicString &sub) const
{
  if (sub.size_ > size_) return false;

  size_type pos = size_ - sub.size_;
  return find(sub, pos) == pos;
}

template <typename CharT, typename Traits, typename Allocator>
std::ostream& operator<<(std::ostream& os, const BasicString<CharT, Traits, Allocator>& str) {
  os << str.c_str();
  return os;
}

template <typename CharT, typename Traits, typename Allocator>
std::istream& operator>>(std::istream& is, BasicString<CharT, Traits, Allocator>& str) {
  str.clear();

  typename BasicString<CharT, Traits, Allocator>::size_type n = 0;
  constexpr size_t buffer_size = 128;

  CharT buffer[buffer_size];

  while (is.read(buffer, buffer_size))
  {
    size_t num_read = is.gcount();
    if (num_read > 0)
    {
      str.allocate_and_copy(buffer, num_read);
      n += num_read;
    }
  }

  if (is.gcount() > 0)
  {
    str.allocate_and_copy(buffer, is.gcount());
  }

  return is;
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator> &BasicString<CharT, Traits, Allocator>::operator=(BasicString other)
{
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
  std::swap(allocator_, other.allocator_);

  return *this;
}

template <typename CharT, typename Traits, typename Allocator>
inline typename BasicString<CharT, Traits, Allocator>::const_pointer BasicString<CharT, Traits, Allocator>::c_str() const
{
  return data_;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::size() const
{
  return size_;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::length() const
{
  return size_;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr typename BasicString<CharT, Traits, Allocator>::size_type BasicString<CharT, Traits, Allocator>::capacity() const
{
  return capacity_;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr bool BasicString<CharT, Traits, Allocator>::empty() const
{
  return size_ == 0;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr void BasicString<CharT, Traits, Allocator>::clear()
{
  deallocate();
  size_ = 0;
  capacity_ = 0;
}
