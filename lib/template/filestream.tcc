#ifndef INSOMNIA_FILESTREAM_TCC
#define INSOMNIA_FILESTREAM_TCC

#include "filestream.h"

namespace Insomnia {

// class filestream

template<class Tp, class Info>
filestream<Tp, Info>::~filestream() {close();}

template<class Tp, class Info>
void filestream<Tp, Info>::open(const std::string &filename) {
  if(_fstream.is_open())
    throw FileSystemException("Current file not closed yet");
  _fstream.open(filename, std::ios::in | std::ios::out | std::ios::binary);
  if(!_fstream.is_open()) return;
  _fstream.seekg(std::ios::beg);
  _fstream.read(reinterpret_cast<char *>(&_stat), sizeof(_stat));
  _fstream.read(reinterpret_cast<char *>(&_last_visited), sizeof(_last_visited));
  _fstream.read(reinterpret_cast<char *>(&_size), sizeof(_size));
  _filename = filename;
}

template<class Tp, class Info>
void filestream<Tp, Info>::close() {
  if(!_fstream.is_open()) return;
  _fstream.seekp(std::ios::beg);
  _fstream.write(reinterpret_cast<const char *>(&_stat), sizeof(_stat));
  _fstream.write(reinterpret_cast<const char *>(&_last_visited), sizeof(_last_visited));
  _fstream.write(reinterpret_cast<const char *>(&_size), sizeof(_size));
  _fstream.close();
  _filename.clear();
}

template<class Tp, class Info>
void filestream<Tp, Info>::renew(const std::string &filename) {
  if(_fstream.is_open())
    throw FileSystemException("File renew blocked: Current file not closed yet");
   // if(!std::filesystem::is_regular_file(file))
   //  throw FileSystemException("Invalid file path: " + file.string());
  if(std::filesystem::path file(filename);
    std::filesystem::exists(file)) remove(file);
  _fstream.open(filename, std::ios::out | std::ios::binary);
  _fstream.close();
  _fstream.open(filename, std::ios::in | std::ios::out | std::ios::binary);
  unsigned int ph_uint = 0;
  for(int i = 0; i < k_stat_bytes_count; ++i) // _stat
    _fstream.write(reinterpret_cast<const char *>(&ph_uint), sizeof(ph_uint));
  _fstream.write(reinterpret_cast<const char *>(&ph_uint), sizeof(ph_uint)); // _last_visited
  _fstream.write(reinterpret_cast<const char *>(&ph_uint), sizeof(ph_uint)); // _size
  Info info;
  _fstream.write(reinterpret_cast<const char *>(&info), sizeof(info));
  _fstream.close();
}

template<class Tp, class Info>
void filestream<Tp, Info>::read_info(Info &info) {
  if(!_fstream.is_open())
    throw FileSystemException("No file is open yet");
  _fstream.seekg(k_info_beg);
  _fstream.read(reinterpret_cast<char *>(&info), sizeof(info));
}

template<class Tp, class Info>
void filestream<Tp, Info>::write_info(const Info &info) {
  if(!_fstream.is_open())
    throw FileSystemException("No file is open yet");
  _fstream.seekp(k_info_beg);
  _fstream.write(reinterpret_cast<const char *>(&info), sizeof(info));
}

template<class Tp, class Info>
void filestream<Tp, Info>::find_unallocated() {
  while(_last_visited < k_stat_count) {
    if(_stat[_last_visited >> 5] & (1 << (_last_visited & 31))) {
      ++_last_visited; continue;
    }
    return;
  }
  _last_visited = 0;
  while(_last_visited < fs_constant::k_stat_count) {
    if(_stat[_last_visited >> 5] & (1 << (_last_visited & 31))) {
      ++_last_visited; continue;
    }
    return;
  }
  throw FileSystemException("Current file full.");
}


template<class Tp, class Info>
fspointer filestream<Tp, Info>::alloc() {
  if(!_fstream.is_open())
    throw FileSystemException("No file is open yet");
  find_unallocated();
  _stat[_last_visited >> 5] |= (1 << (_last_visited & 31));
  ++_size;
  fspointer res{_filename, _last_visited};
  return res;
}

template<class Tp, class Info>
fspointer filestream<Tp, Info>::alloc(const Tp &value) {
  if(!_fstream.is_open())
    throw FileSystemException("No file is open yet");
  fspointer fsptr = alloc();
  write(fsptr, value);
  return fsptr;
}

template<class Tp, class Info>
void filestream<Tp, Info>::dealloc(const fspointer &ptr) {
  if(!_fstream.is_open())
    throw FileSystemException("No file is open yet");
  // if(ptr.filename() != _filename)
  //   throw FileSystemException("Invalid fspointer: wrong file.");
  if(ptr == nullptr)
    throw FileSystemException("Freeing nullptr");
  unsigned int pos = ptr.pos();
  if(!(_stat[pos >> 5] & (1 << (pos & 31))))
    throw FileSystemException("Double free or corruption: deallocating unallocated fspointer.");
  _stat[pos >> 5] &= ~(1 << (pos & 31));
  --_size;
  // _last_visited = pos;
}

template<class Tp, class Info>
void filestream<Tp, Info>::read(const fspointer &ptr, Tp &value) {
  if(!_fstream.is_open())
    throw FileSystemException("No file is open yet");
  // if(ptr.filename() != _filename)
  //   throw FileSystemException("Invalid fspointer: wrong file.");
  if(ptr == nullptr)
    throw FileSystemException("Read corruption: reading nullptr");
  unsigned int pos = ptr.pos();
  if(!(_stat[pos >> 5] & (1 << (pos & 31))))
    throw FileSystemException("Read corruption: reading unallocated position.");
  _fstream.seekg(k_storage_beg + pos * sizeof(value));
  _fstream.read(reinterpret_cast<char *>(&value), sizeof(value));
  // _last_visited = pos;
}

template<class Tp, class Info>
void filestream<Tp, Info>::write(const fspointer &ptr, const Tp &value) {
  if(!_fstream.is_open())
    throw FileSystemException("No file is open yet");
  // if(ptr.filename() != _filename)
  //   throw FileSystemException("Invalid fspointer: wrong file.");
  if(ptr == nullptr)
    throw FileSystemException("Write corruption: writing nullptr");
  unsigned int pos = ptr.pos();
  if(!(_stat[pos >> 5] & (1 << (pos & 31))))
    throw FileSystemException("Write corruption: writing unallocated position.");
  _fstream.seekp(k_storage_beg + pos * sizeof(value));
  _fstream.write(reinterpret_cast<const char *>(&value), sizeof(value));
  // _last_visited = pos;
}

template<class Tp, class Info>
bool filestream<Tp, Info>::is_open() const {return _fstream.is_open();}

template <class Tp, class Info>
size_t filestream<Tp, Info>::occupancy_number() const {
  if(!_fstream.is_open())
    throw FileSystemException("No file is open yet");
  return _size;
}


template<class Tp, class Info>
double filestream<Tp, Info>::occupancy_rate() const {
  if(!_fstream.is_open())
    throw FileSystemException("No file is open yet");
  return static_cast<double>(_size) / static_cast<double>(k_stat_count);
}


} // namespace Insomnia


#endif // INSOMNIA_FILESTREAM_TCC