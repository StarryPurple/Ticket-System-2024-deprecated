#include "conststring.h"
#include "ticket_system_settings.h"

#include <iostream>
#include <cstring>
#include <map>
#include <filesystem>

int main() {
  Insomnia::utf8_string<7> info = "测试信息010";
  std::cout << info << std::endl;
  return 0;
}
