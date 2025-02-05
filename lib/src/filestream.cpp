#include "filestream.h"

namespace Insomnia {

// class fspointer

fspointer::fspointer()
  : _pos(k_stat_count) {}

fspointer::fspointer(const std::string &filename, unsigned int pos)
  : _pos(pos) {}
//  : _file(filename), _pos(pos) {}


// std::string fspointer::filename() const {return _file;}

unsigned int fspointer::pos() const {return _pos;}

bool fspointer::operator==(const nullptr_t &) const {return _pos == k_stat_count;}

bool fspointer::operator!=(const nullptr_t &) const {return _pos != k_stat_count;}

bool fspointer::operator==(const fspointer &other) const {return  _pos == other._pos;} // file

bool fspointer::operator!=(const fspointer &other) const {return  _pos != other._pos;} // file

fspointer &fspointer::operator=(const nullptr_t &) {
  _pos = k_stat_count;
  return *this;
}


bool operator==(const nullptr_t &, const fspointer &ptr) {return ptr._pos == fspointer::k_stat_count;}
bool operator!=(const nullptr_t &, const fspointer &ptr) {return ptr._pos != fspointer::k_stat_count;}

} // namespace Insomnia