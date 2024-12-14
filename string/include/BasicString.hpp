#ifndef STRING_H
#define STRING_H

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <stddef.h>
#include <stdexcept>
#include <string_view>

// TODO: implement iterators, operator+ for string_view, const char*,
// BasicString, optimize work with allocator(select, propagate), SSO

template <typename CharT, typename Traits = std::char_traits<CharT>,
          typename Allocator = std::allocator<CharT>>
class BasicString {
public:
  using value_type = CharT;
  using allocator_type = Allocator;
  using size_type = typename std::allocator_traits<allocator_type>::size_type;
  using pointer = typename std::allocator_traits<allocator_type>::pointer;
  using const_pointer =
      typename std::allocator_traits<allocator_type>::const_pointer;
  using reference = CharT &;
  using const_reference = const CharT &;
  using difference_type =
      typename std::allocator_traits<allocator_type>::difference_type;
  using traits_type = Traits;

  static const size_type npos = static_cast<size_type>(-1);

private:
  pointer data_;
  size_type size_;
  size_type capacity_;
  allocator_type allocator_;

public:
  /* constructor */
  BasicString();
  BasicString(const BasicString &);
  BasicString(const BasicString &other, size_type pos, size_type len = npos);
  BasicString(const CharT *);
  BasicString(size_t n, CharT c);
  BasicString(std::nullptr_t) = delete;

  /* desturctor */
  ~BasicString();

  /* operator= */
  BasicString &operator=(BasicString);

  Allocator get_allocator() const;

  /* element access */
  const_pointer c_str() const;
  const_pointer data() const;
  constexpr const_reference operator[](size_type index) const;
  constexpr reference operator[](size_type index);
  constexpr const_reference at(size_type index) const;
  constexpr reference at(size_type index);
  operator std::basic_string_view<CharT, Traits>() const noexcept;

  /* capacity */
  constexpr size_type size() const;
  constexpr size_type length() const;
  constexpr size_type capacity() const;
  constexpr bool empty() const;
  constexpr void reserve(size_type new_cap);

  /* modifiers */
  constexpr void pop_back();
  constexpr void push_back(CharT ch);
  constexpr void replace(size_type pos, size_type len, const BasicString &str);
  constexpr void resize(size_type count, CharT ch);
  constexpr void erase(size_type pos, size_type len);
  constexpr void clear();

  /* search */
  size_type find(const BasicString &sub, size_type pos = 0) const;

  /* operations */
  int compare(const BasicString &other) const;
  bool starts_with(const BasicString &prefix) const;
  bool ends_with(const BasicString &sub) const;

  std::weak_ordering operator<=>(const BasicString &) const;
  std::weak_ordering operator<=>(const char *) const;
  bool operator==(const BasicString &) const;
  bool operator!=(const BasicString &) const;
  bool operator==(const char *) const;
  bool operator!=(const char *) const;

private:
  using allocator_traits_type = std::allocator_traits<allocator_type>;

  void allocate_and_copy(const CharT *str, size_t len);
  void deallocate();

  template <typename T, typename Tr, typename Al>
  friend std::basic_ostream<T, Tr> &
  operator<<(std::basic_ostream<T, Tr> &os, const BasicString<T, Tr, Al> &str);

  template <typename T, typename Tr, typename Al>
  friend std::basic_istream<T, Tr> &operator>>(std::basic_istream<T, Tr> &is,
                                               BasicString<T, Tr, Al> &str);

  template <typename T, typename Tr, typename Al>
  friend constexpr void swap(BasicString<T, Tr, Al> &lhs,
                             BasicString<T, Tr, Al> &rhs) noexcept;
};

#endif

// template <typename CharT, typename Traits, typename Allocator, typename U>
// inline BasicString<CharT, Traits, Allocator>::size_type
// erase(BasicString<CharT, Traits, Allocator>& str, const U& value)
// {
//   auto it = std::remove(c.begin(), c.end(), value);
//   auto r = c.end() - it;
//   c.erase(it, c.end());
//   return r;
// }

// template <typename CharT, typename Traits, typename Allocator, typename Pred>
// inline BasicString<CharT, Traits, Allocator>::size_type
// erase_if(BasicString<CharT, Traits, Allocator>& str, Pred pred)
// {
//   auto it = std::remove_if(c.begin(), c.end(), pred);
//   auto r = c.end() - it;
//   c.erase(it, c.end());
//   return r;
// }

template <typename CharT, typename Traits, typename Allocator>
inline void
BasicString<CharT, Traits, Allocator>::allocate_and_copy(const CharT *str,
                                                         size_t len) {
  data_ = allocator_traits_type::allocate(allocator_, len + 1);
  std::memcpy(data_, str, len * sizeof(CharT));
  data_[len] = '\0';
  size_ = len;
  capacity_ = len + 1;
}

template <typename CharT, typename Traits, typename Allocator>
inline void BasicString<CharT, Traits, Allocator>::deallocate() {
  if (data_) {
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
inline BasicString<CharT, Traits, Allocator>::BasicString(
    const BasicString &other)
    : data_(nullptr), size_(other.size_), capacity_(other.capacity_),
      allocator_(other.allocator_) {
  if (size_ > 0) {
    data_ = allocator_traits_type::allocate(allocator_, size_ + 1);
    std::memcpy(data_, other.data_, size_ * sizeof(CharT));
    data_[size_] = '\0';
  } else {
    data_ = nullptr;
  }
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::BasicString(
    const BasicString &other, size_type pos, size_type len)
    : allocator_(other.allocator_) {
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
inline BasicString<CharT, Traits, Allocator>::BasicString(const CharT *copy) {
  size_type len = traits_type::length(copy);
  allocate_and_copy(copy, len);
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::BasicString(size_t n, CharT c) {
  data_ = allocator_traits_type::allocate(allocator_, n + 1);
  std::memset(data_, c, n);
  size_ = n;
  capacity_ = n + 1;
  data_[size_] = '\0';
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::~BasicString() {
  deallocate();
}

template <typename CharT, typename Traits, typename Allocator>
inline typename BasicString<CharT, Traits, Allocator>::size_type
BasicString<CharT, Traits, Allocator>::find(const BasicString &sub,
                                            size_type pos) const {
  if (pos > size_)
    return npos;

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
  const_pointer result = std::search(start, c_str() + size_, sub.c_str(),
                                     sub.c_str() + sub.size());
  return result != c_str() + size_ ? result - c_str() : npos;
}

template <typename CharT, typename Traits, typename Allocator>
inline int
BasicString<CharT, Traits, Allocator>::compare(const BasicString &other) const {
  return Traits::compare(c_str(), other.c_str(), std::min(size_, other.size_));
}

template <typename CharT, typename Traits, typename Allocator>
inline bool BasicString<CharT, Traits, Allocator>::starts_with(
    const BasicString &prefix) const {
  if (prefix.size_ > size_)
    return false;

  return find(prefix, 0) == 0;
}

template <typename CharT, typename Traits, typename Allocator>
inline bool
BasicString<CharT, Traits, Allocator>::ends_with(const BasicString &sub) const {
  if (sub.size_ > size_)
    return false;

  size_type pos = size_ - sub.size_;
  return find(sub, pos) == pos;
}

template <typename CharT, typename Traits, typename Allocator>
inline std::weak_ordering
BasicString<CharT, Traits, Allocator>::operator<=>(const char *other) const {
  return std::lexicographical_compare_three_way(data_, data_ + size_, other,
                                                other + std::strlen(other));
}

template <typename CharT, typename Traits, typename Allocator>
inline bool
BasicString<CharT, Traits, Allocator>::operator==(const char *other) const {
  return (this->operator<=>(other)) == std::weak_ordering::equivalent;
}

template <typename CharT, typename Traits, typename Allocator>
inline bool
BasicString<CharT, Traits, Allocator>::operator!=(const char *other) const {
  return (this->operator<=>(other)) != std::weak_ordering::equivalent;
}

template <typename CharT, typename Traits, typename Allocator>
inline bool BasicString<CharT, Traits, Allocator>::operator==(
    const BasicString &other) const {
  return (this->operator<=>(other)) == std::weak_ordering::equivalent;
}

template <typename CharT, typename Traits, typename Allocator>
inline bool BasicString<CharT, Traits, Allocator>::operator!=(
    const BasicString &other) const {
  return (this->operator<=>(other)) != std::weak_ordering::equivalent;
}

template <typename CharT, typename Traits, typename Allocator>
inline std::weak_ordering BasicString<CharT, Traits, Allocator>::operator<=>(
    const BasicString &other) const {
  // if (size_ < other.size_) {
  //   return std::weak_ordering::less;
  // } else if (size_ > other.size_) {
  //   return std::weak_ordering::greater;
  // }

  // for (typename BasicString<CharT, Traits, Allocator>::size_type i = 0;
  //      i < size_; ++i) {
  //   if (Traits::lt(data_[i], other.data_[i])) {
  //     return std::weak_ordering::less;
  //   } else if (Traits::eq(data_[i], other.data_[i])) {
  //     continue;
  //   } else {
  //     return std::weak_ordering::greater;
  //   }
  // }

  // return std::weak_ordering::equivalent;
  return std::lexicographical_compare_three_way(
      data_, data_ + size_, other.data_, other.data_ + other.size_);
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr void BasicString<CharT, Traits, Allocator>::pop_back() {
  if (size_ > 0) {
    size_ -= 1;
    data_[size_] = '\0';
  }
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr void
BasicString<CharT, Traits, Allocator>::push_back(CharT ch) {
  if (size_ + 1 >= capacity_) {
    size_type new_cap = capacity_ == 0 ? 1 : capacity_ * 2;
    pointer new_data = allocator_traits_type::allocate(allocator_, new_cap + 1);

    if (data_) {
      std::memcpy(new_data, data_, size_ * sizeof(CharT));
      allocator_traits_type::deallocate(allocator_, data_, capacity_);
    }

    new_data[size_] = ch;
    new_data[size_ + 1] = '\0';
    data_ = new_data;
    size_ += 1;
    capacity_ = new_cap;
  } else {
    data_[size_] = ch;
    size_ += 1;
  }
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr void
BasicString<CharT, Traits, Allocator>::replace(size_type pos, size_type len,
                                               const BasicString &str) {
  if (pos > size_) {
    throw std::out_of_range("Position is out of range.");
  }

  if (len > size_) {
    len = size_;
  }

  size_type new_size = size_ - len + str.size_;

  if (new_size > capacity_) {
    size_type new_cap = new_size + 1;
    pointer new_data = allocator_traits_type::allocate(allocator_, new_cap);

    std::memcpy(new_data, data_, pos * sizeof(CharT));
    std::memcpy(new_data + pos, str.data_, str.size_ * sizeof(CharT));
    std::memcpy(new_data + pos + str.size_, data_ + pos + len,
                (size_ - len - pos) * sizeof(CharT));

    allocator_traits_type::deallocate(allocator_, data_, capacity_);

    data_ = new_data;
    size_ = new_size;
    capacity_ = new_cap;
  } else {
    std::memcpy(data_ + pos, str.data_, str.size_ * sizeof(CharT));
    std::memcpy(data_ + pos + str.size_, data_ + pos + len,
                (size_ - len - pos) * sizeof(CharT));
    size_ = new_size;
  }

  data_[size_] = '\0';
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr void
BasicString<CharT, Traits, Allocator>::resize(size_type count, CharT ch) {
  if (count > size_) {
    if (count > capacity_) {
      pointer new_data = allocator_traits_type::allocate(allocator_, count + 1);

      std::memcpy(new_data, data_, size_ * sizeof(CharT));

      std::memset(new_data + size_, ch, (count - size_) * sizeof(CharT));

      allocator_traits_type::deallocate(allocator_, data_, capacity_);

      data_ = new_data;
      capacity_ = count + 1;
    } else {
      std::memset(data_ + size_, ch, (count - size_) * sizeof(CharT));
    }
  } else if (count < size_) {
    size_ = count;
  }

  data_[count] = '\0';
  size_ = count;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr void
BasicString<CharT, Traits, Allocator>::erase(size_type pos, size_type len) {
  if (pos > size_) {
    throw std::out_of_range("Position is out of range.");
  }

  len = std::min(len, size_ - pos);

  std::memmove(data_ + pos, data_ + pos + len,
               (size_ - len - pos) * sizeof(CharT));
  size_ -= len;
  data_[size_] = '\0';
}

template <typename CharT, typename Traits, typename Allocator>
inline std::basic_ostream<CharT, Traits> &
operator<<(std::basic_ostream<CharT, Traits> &os,
           const BasicString<CharT, Traits, Allocator> &str) {
  os << str.c_str();
  return os;
}

template <typename CharT, typename Traits, typename Allocator>
inline std::basic_istream<CharT, Traits> &
operator>>(std::basic_istream<CharT, Traits> &is,
           BasicString<CharT, Traits, Allocator> &str) {
  str.clear();

  typename BasicString<CharT, Traits, Allocator>::size_type n = 0;
  constexpr size_t buffer_size = 128;

  CharT buffer[buffer_size];

  while (is.read(buffer, buffer_size)) {
    size_t num_read = is.gcount();
    if (num_read > 0) {
      str.allocate_and_copy(buffer, num_read);
      n += num_read;
    }
  }

  if (is.gcount() > 0) {
    str.allocate_and_copy(buffer, is.gcount());
  }

  return is;
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator> &
BasicString<CharT, Traits, Allocator>::operator=(BasicString other) {
  swap(*this, other);
  return *this;
}

template <typename CharT, typename Traits, typename Allocator>
inline Allocator BasicString<CharT, Traits, Allocator>::get_allocator() const {
  return allocator_;
  ;
}

template <typename CharT, typename Traits, typename Allocator>
inline typename BasicString<CharT, Traits, Allocator>::const_pointer
BasicString<CharT, Traits, Allocator>::c_str() const {
  return data_;
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::const_pointer
BasicString<CharT, Traits, Allocator>::data() const {
  return data_;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr BasicString<CharT, Traits, Allocator>::const_reference
BasicString<CharT, Traits, Allocator>::operator[](size_type index) const {
  return data_[index];
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr BasicString<CharT, Traits, Allocator>::reference
BasicString<CharT, Traits, Allocator>::operator[](size_type index) {
  return data_[index];
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr BasicString<CharT, Traits, Allocator>::const_reference
BasicString<CharT, Traits, Allocator>::at(size_type index) const {
  if (index >= size_) {
    throw std::out_of_range("BasicString::at: position out of range");
  }
  return data_[index];
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr BasicString<CharT, Traits, Allocator>::reference
BasicString<CharT, Traits, Allocator>::at(size_type index) {
  if (index >= size_) {
    throw std::out_of_range("BasicString::at: position out of range");
  }
  return data_[index];
}

template <typename CharT, typename Traits, typename Allocator>
inline BasicString<CharT, Traits, Allocator>::operator std::basic_string_view<
    CharT, Traits>() const noexcept {
  return std::basic_string_view<CharT, Traits>(data_, size_);
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr typename BasicString<CharT, Traits, Allocator>::size_type
BasicString<CharT, Traits, Allocator>::size() const {
  return size_;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr typename BasicString<CharT, Traits, Allocator>::size_type
BasicString<CharT, Traits, Allocator>::length() const {
  return size_;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr typename BasicString<CharT, Traits, Allocator>::size_type
BasicString<CharT, Traits, Allocator>::capacity() const {
  return capacity_;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr bool BasicString<CharT, Traits, Allocator>::empty() const {
  return size_ == 0;
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr void
BasicString<CharT, Traits, Allocator>::reserve(size_type new_cap) {
  if (new_cap <= capacity_)
    return;

  pointer new_data = allocator_traits_type::allocate(allocator_, new_cap);

  if (data_ != nullptr) {
    std::memcpy(new_data, data_, size_ * sizeof(CharT));
  }

  allocator_traits_type::deallocate(allocator_, data_, capacity_);

  data_ = new_data;
  capacity_ = new_cap;
  data_[size_] = '\0';
}

template <typename CharT, typename Traits, typename Allocator>
inline constexpr void BasicString<CharT, Traits, Allocator>::clear() {
  deallocate();
  size_ = 0;
  capacity_ = 0;
}

template <typename T, typename Tr, typename Al>
inline constexpr void swap(BasicString<T, Tr, Al> &lhs,
                           BasicString<T, Tr, Al> &rhs) noexcept {
  std::swap(lhs.data_, rhs.data_);
  std::swap(lhs.size_, rhs.size_);
  std::swap(lhs.capacity_, rhs.capacity_);
  std::swap(lhs.allocator_, rhs.allocator_);
}

inline BasicString<char> operator"" _s(const char *str, size_t length) {
  return BasicString<char>(str);
}

inline BasicString<wchar_t> operator"" _s(const wchar_t *str, size_t length) {
  return BasicString<wchar_t>(str);
}

inline BasicString<char16_t> operator"" _s(const char16_t *str, size_t length) {
  return BasicString<char16_t>(str);
}
