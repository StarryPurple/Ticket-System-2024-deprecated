#include "conststring.h"
#include "pair.hpp"
#include "map.h"
#include "set.h"
#include "vector.h"
#include "rb_tree.h"
#include "multimap_db.h"

#include <iostream>
#include <cstring>
#include <map>
#include <filesystem>

void test_filestream() {
  std::filesystem::path working_dir = std::filesystem::current_path().parent_path();
  std::filesystem::path data_dir = working_dir / "data";
  std::filesystem::path data_file = data_dir / "filestream_test.txt";
  using name_t = Insomnia::utf8_string<4>;
  Insomnia::filestream<name_t, int> fs;
  fs.renew(data_file);
  fs.open(data_file);
  if(!fs.is_open()) {
    std::cout << "Failed to open file." << std::endl;
    return;
  }
  name_t nameA = "李华", nameB = "小明", nameC = "Andy", nameD = "一见生财";
  int number = -1;
  fs.read_info(number); std::cout << number << std::endl;
  number = 4;
  fs.write_info(number);
  number = 0;
  fs.read_info(number); std::cout << number << std::endl;
  auto ptrA = fs.alloc(nameA), ptrB = fs.alloc(nameB), ptrC = fs.alloc(nameC), ptrD = fs.alloc(nameD);
  name_t name;
  fs.read(ptrA, name); std::cout << name << std::endl;
  fs.read(ptrB, name); std::cout << name << std::endl;
  fs.read(ptrC, name); std::cout << name << std::endl;
  fs.read(ptrD, name); std::cout << name << std::endl;
  name_t nameA2 = "李明";
  fs.write(ptrA, nameA2);
  fs.read(ptrA, name); std::cout << name << std::endl;
  fs.dealloc(ptrB);
  try {
    fs.read(ptrB, name);
  } catch(Insomnia::FileSystemException &ex) {
    std::cout << "File system error: " << ex.what() << std::endl;
  }
  std::cout << fs.occupancy() << std::endl;
}

int main() {
  // test_filestream();

  return 0;
}
