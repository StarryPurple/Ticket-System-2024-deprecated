#ifndef CONSTSTRING_TCC
#define CONSTSTRING_TCC

#include "conststring.h"

namespace Insomnia {

// class ConstString

template<int length>
ConstString<length>::ConstString() {
  _str[0] = '\0';
}

template<int length>
ConstString<length>::ConstString(const char *str): ConstString() {
  if(strlen(str) > length)
    throw FileSystemException(
      "String" + std::string(str) + " too long for const string of length " + std::to_string(length));
  strcpy(_str, str);
}

template<int length>
ConstString<length>::ConstString(const std::string &str): ConstString(str.c_str()) {}

template<int length>
ConstString<length>::ConstString(const ConstString &other): ConstString() {
  strcpy(_str, other._str);
}

template<int length>
ConstString<length>::ConstString(ConstString &&other): _str(other._str) {}

template<int length>
ConstString<length> &ConstString<length>::operator=(const char *str) {
  if(strlen(str) > length)
    throw FileSystemException(
      "String" + std::string(str) + " too long for const string of length " + std::to_string(length));
  strcpy(_str, str);
  return *this;
}

template<int length>
ConstString<length> &ConstString<length>::operator=(const std::string &str) {
  *this = str.c_str();
  return *this;
}

template<int length>
ConstString<length> &ConstString<length>::operator=(const ConstString &other) {
  if(this == &other) return *this;
  strcpy(_str, other._str);
  return *this;
}

template<int length>
ConstString<length> &ConstString<length>::operator=(ConstString &&other) {
  if(this == &other) return *this;
  strcpy(_str, other._str);
  other._str[0] = '\0';
  return *this;
}

template<int length>
const char *ConstString<length>::cstr() const {
  return _str;
}

template<int length>
std::string ConstString<length>::str() const {
  return _str;
}


template<int length>
bool ConstString<length>::operator==(const ConstString &other) const {
  return std::string(_str) == std::string(other._str);
}

template<int length>
bool ConstString<length>::operator!=(const ConstString &other) const {
  return std::string(_str) != std::string(other._str);
}

template<int length>
bool ConstString<length>::operator<(const ConstString &other) const {
  return std::string(_str) < std::string(other._str);
}

template<int length>
bool ConstString<length>::operator>(const ConstString &other) const {
  return std::string(_str) > std::string(other._str);
}

template<int length>
bool ConstString<length>::operator<=(const ConstString &other) const {
  return std::string(_str) <= std::string(other._str);
}

template<int length>
bool ConstString<length>::operator>=(const ConstString &other) const {
  return std::string(_str) >= std::string(other._str);
}

template<int length>
std::istream &operator>>(std::istream &is, ConstString<length> &cstr) {
  std::string str;
  is >> str;
  cstr = str;
  return is;
}

template<int length>
std::ostream &operator<<(std::ostream &os, ConstString<length> &cstr) {
  os << cstr.to_str();
  return os;
}
} // namespace Insomnia


#endif // CONSTSTRING_TCC