#include "interface_system.h"
#include "config.h"

#include <iostream>
#include <cstring>
#include <map>
#include <filesystem>

void test_filestream() {
  std::filesystem::path working_dir = std::filesystem::current_path().parent_path();
  std::filesystem::path data_dir = working_dir / "data";
  std::filesystem::path data_file = data_dir / "filestream_test.txt";

  using name_t = insomnia::utf8_string<4>;
  insomnia::filestream<name_t, int> fs;
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
  } catch(insomnia::FileSystemException &ex) {
    std::cout << "File system error: " << ex.what() << std::endl;
  }
  std::cout << fs.occupancy_number() << std::endl;
}

void test_database() {
  std::string path = "database_test.txt";

  using index_t = insomnia::ascii_string<64>;
  insomnia::bplustree<index_t, int> db;
  // db.renew(path);
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
      auto res = db.list(index);
      if(res.empty()) std::cout << "null" << std::endl;
      else {
        for(auto it = res.begin(); it != res.end();) {
          std::cout << *it;
          if(++it != res.end()) std::cout << ' ';
        }
        std::cout << std::endl;
      }
    }
  }
  db.close();
}

void test_ticket_system_interface() {
  // std::cerr << "Debug here" << std::endl;
  ism::stoi("-0"); // ?
  std::filesystem::path current_dir = std::filesystem::current_path();
  std::filesystem::path data_dir = current_dir.parent_path() / "data";

  TicketSystem::InterfaceSystem interface_system;
  // interface_system.interface(current_dir.string());
   interface_system.interface(data_dir.string());
  // interface_system.interface(".");
}

std::vector<std::pair<std::string, std::vector<int>>> parseJsonToVector(const std::string& filename) {
  std::vector<std::pair<std::string, std::vector<int>>> result;
  std::ifstream file(filename);
  if (!file.is_open()) {
    return result;
  }

  std::string line;
  std::string jsonContent;
  while (std::getline(file, line)) {
    jsonContent += line;
  }
  file.close();

  // Remove whitespace
  std::string cleanContent;
  for (char c : jsonContent) {
    if (!std::isspace(c)) {
      cleanContent += c;
    }
  }

  size_t pos = 0;
  // Skip until the first '{'
  pos = cleanContent.find('{', pos);
  if (pos == std::string::npos) {
    return result;
  }
  pos++;

  while (pos < cleanContent.size()) {
    // Find key
    size_t quoteStart = cleanContent.find('"', pos);
    if (quoteStart == std::string::npos) break;
    size_t quoteEnd = cleanContent.find('"', quoteStart + 1);
    if (quoteEnd == std::string::npos) break;

    std::string key = cleanContent.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
    pos = quoteEnd + 1;

    // Find ':'
    pos = cleanContent.find(':', pos);
    if (pos == std::string::npos) break;
    pos++;

    // Find '['
    pos = cleanContent.find('[', pos);
    if (pos == std::string::npos) break;
    pos++;

    // Find numbers
    std::vector<int> numbers;
    while (pos < cleanContent.size()) {
      // Skip non-digit characters
      while (pos < cleanContent.size() && !std::isdigit(cleanContent[pos])) {
        if (cleanContent[pos] == ']') break;
        pos++;
      }
      if (pos >= cleanContent.size() || cleanContent[pos] == ']') break;

      // Extract number
      size_t numStart = pos;
      while (pos < cleanContent.size() && std::isdigit(cleanContent[pos])) {
        pos++;
      }
      std::string numStr = cleanContent.substr(numStart, pos - numStart);
      numbers.push_back(std::stoi(numStr));

      // Skip comma
      if (cleanContent[pos] == ',') pos++;
    }

    // Add to result
    result.emplace_back(key, numbers);

    // Find next entry or end
    pos = cleanContent.find(']', pos);
    if (pos == std::string::npos) break;
    pos++;
    if (cleanContent[pos] == ',') pos++;
    if (cleanContent[pos] == '}') break;
  }

  return result;
}

void global_test() {
  auto json = parseJsonToVector("../testcases/config.json");
  std::string path = "../testcases";
  auto original_cin = std::cin.rdbuf();
  auto original_cout = std::cout.rdbuf();
  for(auto &[subdir, tests]: json) {
    std::system("cd ../data && ./clear.bash");
    for(auto test: tests) {
      std::string prefix = path + '/' + subdir + '/' + std::to_string(test);
      std::ifstream fin(prefix + ".in");
      std::cin.rdbuf(fin.rdbuf());
      std::ofstream fout(prefix + ".ism");
      std::cout.rdbuf(fout.rdbuf());
      test_ticket_system_interface();
      std::cout.flush();
      std::cin.rdbuf(original_cin);
      fin.close();
      std::cout.rdbuf(original_cout);
      fout.close();
      std::system(std::string("diff -bB " + prefix + ".ism " + prefix + ".out").c_str());
    }
  }
}

int main() {
  // test_database();
  // test_ticket_system_interface();
  global_test();
  return 0;
}
