#ifndef INSOMNIA_FILESTREAM_H
#define INSOMNIA_FILESTREAM_H

#include <fstream>
#include <filesystem>

#include "lib_exceptions.h"
namespace Insomnia {

/** A manual allocation system, but in files.
 *  Temporarily use a linear manage system instead of heap-like ones.
 */
template<class Tp, class InfoTp>
class filestream {
  static constexpr int k_stat_bytes_count = 1048576;
private:
  std::fstream _fstream;
  int stat[k_stat_bytes_count];

public:

  /** Behave like the pointers in heap memory management.
   *  @warning No memory-leak protections are implemented so far.
   */
  class pointer {
    friend void Insomnia::filestream<Tp, InfoTp>::alloc();
    friend void Insomnia::filestream<Tp, InfoTp>::alloc(const Tp &value);
    static constexpr int k_null_pos = Insomnia::filestream<Tp, InfoTp>::k_stat_bytes_count;
  private:
    const std::filesystem::path _filepath;
    size_t _pos;

    pointer(size_t pos);

  public:
    pointer() = delete;
    ~pointer() = default;
    pointer(const pointer &other) = default;
    pointer(pointer &&other) = default;
    pointer& operator=(const pointer &other) = default;
    pointer& operator=(pointer &&other) = default;

    bool operator==(const pointer &other) const;
    bool operator!=(const pointer &other) const;
    bool operator==(const nullptr_t &) const;
    bool operator!=(const nullptr_t &) const;

    std::filesystem::path filepath() const;
    size_t pos() const;
  };
  filestream() = delete;
  filestream(const std::filesystem::path &filepath);
  ~filestream();
  filestream(const filestream &fs) = delete;
  filestream(filestream &&fs) = delete;
  filestream& operator=(const filestream &fs) = delete;
  filestream& operator=(filestream &&fs) = delete;

  void open();
  void close();
  pointer alloc();
  pointer alloc(const Tp &value);
  void dealloc(const pointer &ptr);
  void write_info(const InfoTp &info);
  void read_info(InfoTp &info);
  void write(const pointer &ptr, const Tp &value);
  void read(const pointer &ptr, Tp &value);
};
}


#include "filestream.tcc"

#endif // INSOMNIA_FILESTREAM_H