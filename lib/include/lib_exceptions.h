#ifndef CONTAINERS_EXCEPTIONS_H
#define CONTAINERS_EXCEPTIONS_H

#include <iostream>

namespace Insomnia {

class FileSystemException final : std::exception {
protected:
  const char *_detail;
public:
  FileSystemException();
  FileSystemException(const char *);
  FileSystemException(const std::string &);
  const char *what() const noexcept final;
};

class ContainerException: std::exception {
protected:
  const char *_detail;
public:
  ContainerException();
  ContainerException(const char *);
  ContainerException(const std::string &);
  const char *what() const noexcept override;
};

class invalid_iterator final: ContainerException {
public:
  invalid_iterator() = default;
  invalid_iterator(const char *);
  invalid_iterator(const std::string &);
  const char *what() const noexcept override final;
};

class index_out_of_bound final: ContainerException {
public:
  index_out_of_bound() = default;
  index_out_of_bound(const char *);
  index_out_of_bound(const std::string &);
  const char *what() const noexcept override final;
};

class container_is_empty final: ContainerException {
public:
  container_is_empty() = default;
  container_is_empty(const char *);
  container_is_empty(const std::string &);
  const char *what() const noexcept override final;
};

} // namespace Insomnia


#endif // CONTAINERS_EXCEPTIONS_H