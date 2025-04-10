#ifndef INSOMNIA_INFO_RECORDER_TCC
#define INSOMNIA_INFO_RECORDER_TCC

#include "info_recorder.h"

namespace insomnia {

template <class Info>
info_recorder<Info>::~info_recorder() {
  close();
}

template <class Info>
void info_recorder<Info>::renew(const std::string &filename) {
  if(is_open())
    throw FileSystemException("Renew blocked: Close the file first.");
  if(std::filesystem::path file(filename);
    std::filesystem::exists(file)) remove(file);
  _fstream.open(filename, std::ios::out | std::ios::binary);
  _fstream.close();
  _fstream.open(filename, std::ios::in | std::ios::out | std::ios::binary);
  Info info;
  _fstream.write(reinterpret_cast<const char *>(&info), sizeof(info));
  _fstream.close();
}

template <class Info>
void info_recorder<Info>::renew() {
  if(!is_open())
    throw FileSystemException("Renew blocked: Open a file first.");
  _fstream.close();
  _fstream.open(_filename, std::ios::out | std::ios::binary);
  _fstream.close();
  _fstream.open(_filename, std::ios::in | std::ios::out | std::ios::binary);
  Info info;
  _fstream.write(reinterpret_cast<const char *>(&info), sizeof(info));
}

template <class Info>
void info_recorder<Info>::open(const std::string &filename) {
  if(is_open())
    throw FileSystemException("Open blocked: still a file open.");
  _filename = filename;
  _fstream.open(_filename, std::ios::in | std::ios::out | std::ios::binary);
}

template <class Info>
void info_recorder<Info>::close() {
  if(!is_open()) return;
  _fstream.close();
  _filename.clear();
}

template <class Info>
void info_recorder<Info>::read_info(Info &info) {
  if(!is_open())
    throw FileSystemException("No file is open yet");
  _fstream.seekg(std::ios::beg);
  _fstream.read(reinterpret_cast<char *>(&info), sizeof(info));
}

template <class Info>
void info_recorder<Info>::write_info(const Info &info) {
  if(!is_open())
    throw FileSystemException("No file is open yet");
  _fstream.seekp(std::ios::beg);
  _fstream.write(reinterpret_cast<const char *>(&info), sizeof(info));
}

template<class Info>
bool info_recorder<Info>::is_open() const {
  return _fstream.is_open();
}

}


#endif // INSOMNIA_INFO_RECORDER_TCC