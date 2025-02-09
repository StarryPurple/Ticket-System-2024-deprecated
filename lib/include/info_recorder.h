#ifndef INSOMNIA_INFO_RECORDER_H
#define INSOMNIA_INFO_RECORDER_H

namespace Insomnia {

template<class Info>
class info_recorder {
  std::fstream _fstream;
  std::string _filename;
public:
  ~info_recorder();
  void renew(const std::string &filename);
  void renew();
  void open(const std::string &filename);
  void close();
  void write_info(const Info &info);
  void read_info(Info &info);
  bool is_open() const;
};

} // namespace Insomnia

#include "info_recorder.tcc"

#endif // INSOMNIA_INFO_RECORDER_H