#include "filestream.h"

namespace Insomnia {

// class fspointer

fspointer::fspointer()
  : _pos(k_stat_count) {}

fspointer::fspointer(const std::filesystem::path &file, unsigned int pos)
  : _file(file), _pos(pos) {}

std::filesystem::path fspointer::file() const {return _file;}

unsigned int fspointer::pos() const {return _pos;}

bool fspointer::operator==(const nullptr_t &) const {return _pos == k_stat_count;}

bool fspointer::operator!=(const nullptr_t &) const {return _pos != k_stat_count;}

bool fspointer::operator==(const fspointer &other) const {return _file == other._file && _pos == other._pos;}

bool fspointer::operator!=(const fspointer &other) const {return _file != other._file || _pos != other._pos;}

fspointer &fspointer::operator=(const nullptr_t &) {
  _pos = k_stat_count;
  return *this;
}


bool operator==(const nullptr_t &, const fspointer &ptr) {return ptr._pos == fspointer::k_stat_count;}
bool operator!=(const nullptr_t &, const fspointer &ptr) {return ptr._pos != fspointer::k_stat_count;}

} // namespace Insomnia