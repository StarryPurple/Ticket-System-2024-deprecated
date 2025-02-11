#ifndef INSOMNIA_FILESTREAM_H
#define INSOMNIA_FILESTREAM_H

#include <fstream>
#include <filesystem>

#include "lib_exceptions.h"
namespace Insomnia {

class fspointer;
template<class Tp, class Info>
class filestream;

class fs_constant {
  friend class fspointer;
  template<class Tp, class Info>
  friend class filestream;
  static constexpr int k_stat_bytes_count = 1 << 15;
  static constexpr int k_stat_count = k_stat_bytes_count << 5;
};


/** @brief A manual in-file allocation system.
 *  @note Temporarily use a linear manage system instead of heap-like ones.
 *
 *  The whole file is divided into four parts:
 *    #stat, recording which part of the file is occupied.
 *    #last_visited, the last visited empty slot.
 *    #size, the number of occupied blocks.
 *    #info, a InfoTp lies here.
 *    #storage, much Tp values lie here.
 */
template<class Tp, class Info>
class filestream {
  static_assert(std::is_default_constructible_v<Info>,
    "Info is required to be default_constructible in Insomnia::filestream.");

  static constexpr int k_stat_bytes_count = fs_constant::k_stat_bytes_count;
  static constexpr int k_stat_count = fs_constant::k_stat_count;

  static constexpr size_t k_info_beg = (k_stat_bytes_count + 2) * sizeof(unsigned int);
  static constexpr size_t k_storage_beg = k_info_beg + sizeof(Info);

private:
  std::fstream _fstream;
  std::string _filename;
  unsigned int _stat[k_stat_bytes_count];
  unsigned int _last_visited;
  unsigned int _size;

  void find_unallocated();

public:

  filestream() = default;
  ~filestream();

  filestream(const filestream &) = delete;
  filestream(filestream &&fs) = delete;
  filestream& operator=(const filestream &) = delete;
  filestream& operator=(filestream &&) = delete;

  /** can be used in creating new files and cleaning existing files.
   * will first delete the existing one if it exists, then create a new one and do initialization.
   * remember to check whether the operation succeeded by std::filesystem::exists(file).
   * @attention is_open() == false required.
   */
  void renew(const std::string &filename);
  /** can be used in renewing current file.
   * @attention is_open() == true required.
   */
  void renew();
  /** fails if file not exist.
   * close the current opening one first.
   */
  void open(const std::string &filename);
  void close();
  fspointer alloc();
  fspointer alloc(const Tp &);
  void dealloc(const fspointer &);
  void write_info(const Info &);
  void read_info(Info &);
  void write(const fspointer &ptr, const Tp &value);
  void read(const fspointer &ptr, Tp &value);

  bool is_open() const;
  size_t occupancy_number() const;
  double occupancy_rate() const;
  bool empty() const;
};
/** @brief Index which behave like the pointers in heap memory management.
 *  @warning No memory-leak protections are implemented so far.
 */
class fspointer {
  template<class Tp, class InfoTp>
  friend fspointer filestream<Tp, InfoTp>::alloc();

  static constexpr int k_stat_count = fs_constant::k_stat_count;

private:
  // std::string _file;
  unsigned int _pos;

  fspointer(const std::string &filename, unsigned int pos);

public:
  fspointer();
  ~fspointer() = default;
  fspointer(const fspointer &) = default;
  fspointer(fspointer &&) = default;
  fspointer& operator=(const fspointer &) = default;
  fspointer& operator=(fspointer &&) = default;
  fspointer& operator=(const nullptr_t &);


  bool operator==(const fspointer &) const;
  bool operator!=(const fspointer &) const;
  bool operator==(const nullptr_t &) const;
  bool operator!=(const nullptr_t &) const;
  friend bool operator==(const nullptr_t &, const fspointer &);
  friend bool operator!=(const nullptr_t &, const fspointer &);

  // std::string filename() const;
  unsigned int pos() const;
};

}


#include "filestream.tcc"

#endif // INSOMNIA_FILESTREAM_H