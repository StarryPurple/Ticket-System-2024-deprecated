#include "algorithm.h"

namespace Insomnia {

int stoi(const std::string &str) {
  if(str.empty()) return 0;
  int res = 0; bool flag = (str[0] == '-');
  for(int i = (flag ? 1 : 0); i < str.length(); ++i)
    res = res * 10 + (str[i] - '0');
  return res * (flag ? -1 : 1);
}

std::string itos(int n) {
  if(n == 0) return "0";
  bool flag = false;
  if(n < 0) {
    flag = true;
    n = -n;
  }
  std::string rev, str;
  while(n > 0) {
    rev += ('0' + n % 10);
    n /= 10;
  }
  if(flag) str += '-';
  for(int i = rev.length() - 1; i >= 0; --i)
    str += rev[i];
  return str;
}

vector<std::string> string_split(const std::string &str, const char separator) {
  vector<std::string> res;
  std::string res_str;
  for(const char &ch: str) {
    if(ch == separator) {
      if(!res_str.empty()) res.push_back(res_str);
      res_str.clear();
    } else res_str += ch;
  }
  if(!res_str.empty()) res.push_back(res_str);
  return res;
}


} // namespace Insomnia