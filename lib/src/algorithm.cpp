#include "algorithm.h"

namespace Insomnia {

int stoi(const std::string &str) {
  int res = 0; bool flag = (str[0] == '-');
  for(int i = (flag ? 1 : 0); i < str.length(); ++i)
    res = res * 10 + (str[i] - '0');
  return res * (flag ? -1 : 1);
}

} // namespace Insomnia