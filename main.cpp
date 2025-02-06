#include "conststring.h"
#include "pair.h"
#include "map.h"
#include "set.h"
#include "vector.h"
#include "rb_tree.h"
#include "database.h"

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
  // fs.renew(data_file.string());
  fs.open(data_file.string());
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
  std::cout << fs.occupancy_number() << std::endl;
}

void test_database() {
  std::string path = "database_test.txt";

  using index_t = Insomnia::ascii_string<64>;
  Insomnia::database<index_t, int> db;
  db.open(path);
  if(!db.is_open()) {
    db.renew(path);
    db.open(path);
  }
  assert(db.is_open());
  int n; std::cin >> n;
  std::string opt; index_t index; int value;
  while(n--) {
    std::cin >> opt;
    if(opt == "insert") {
      std::cin >> index >> value;
      db.insert(index, value);
    } else if(opt == "delete") {
      std::cin >> index >> value;
      db.erase(index, value);
    } else if(opt == "find") {
      std::cin >> index;
      auto res = db[index];
      if(res.empty()) std::cout << "null" << std::endl;
      else {
        for(const auto &val: res)
          std::cout << val << ' ';
        std::cout << std::endl;
      }
    }
  }
  db.close();
}

int main() {
  try {
    // test_filestream();
    test_database();
  } catch(Insomnia::FileSystemException &ex) {
    std::cout << ex.what() << std::endl;
  }
  return 0;
}
