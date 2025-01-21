#ifndef CONTAINERS_EXCEPTIONS_H
#define CONTAINERS_EXCEPTIONS_H

#include <iostream>

namespace Insomnia {

class FileSystemException final : std::exception {
protected:
  const char *_detail;
public:
  FileSystemException();
  FileSystemException(const char *detail);
  FileSystemException(const std::string &detail);
  const char *what() const noexcept final;
};
} // namespace Insomnia


#endif // CONTAINERS_EXCEPTIONS_H