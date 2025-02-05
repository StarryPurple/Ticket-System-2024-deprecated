#ifndef CONST_STRING_H
#define CONST_STRING_H

#include "lib_exceptions.h"

#include <iostream>
#include <cstring>

namespace Insomnia{

/* A length-fixed string type which acts like "const char [length]".
 * Supports ASCII characters (CharT = char) and UTF-8 characters (CharT = wchar_t).
 * @param length: Commonly an ASCII char takes 1 byte, a UTF-8 char takes
 */
template<int length>
class ConstString {
private:
  char _str[length + 1];
public:
  ConstString();
  ConstString(const char *str);
  ConstString(const std::string &str);
  ConstString(const ConstString &);
  ConstString(ConstString &&);
  ~ConstString() = default;
  ConstString &operator=(const char *str);
  ConstString &operator=(const std::string &str);
  ConstString &operator=(const ConstString &other);
  ConstString &operator=(ConstString &&other);
  const char *to_str() const;
  // template<int other_length>
  // ConstString<length + other_length, CharT> operator+(const ConstString<CharT, other_length>& other);
  bool operator==(const ConstString &) const;
  bool operator!=(const ConstString &) const;
  bool operator<(const ConstString &) const;
  bool operator>(const ConstString &) const;
  bool operator<=(const ConstString &) const;
  bool operator>=(const ConstString &) const;

};

// Why can't I make them friend functions?

template<int length>
std::istream &operator>>(std::istream &is, ConstString<length> &cstr);

template<int length>
std::ostream &operator<<(std::ostream &os, ConstString<length> &cstr);

template<int length>
using ascii_string = ConstString<length>;

template<int length>
using utf8_string = ConstString<length * 4>;

} // namespace Insomnia

#include "conststring.tcc"

#endif // CONST_STRING_H