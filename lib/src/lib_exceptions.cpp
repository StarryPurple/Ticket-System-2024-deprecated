#include "lib_exceptions.h"

namespace insomnia {

// class FileSystemException

FileSystemException::FileSystemException(): _detail("") {}

FileSystemException::FileSystemException(const char *detail): _detail(detail) {}

FileSystemException::FileSystemException(const std::string &detail): _detail(detail.c_str()) {}

const char *FileSystemException::what() const noexcept {return _detail;}

// class ContainerException

ContainerException::ContainerException(): _detail("") {}

ContainerException::ContainerException(const char *detail): _detail(detail) {}

ContainerException::ContainerException(const std::string &detail): _detail(detail.c_str()) {}

const char *ContainerException::what() const noexcept {return _detail;}

// class invalid_iterator

invalid_iterator::invalid_iterator(const char *detail): ContainerException(detail) {}

invalid_iterator::invalid_iterator(const std::string &detail): ContainerException(detail.c_str()) {}

const char *invalid_iterator::what() const noexcept {return _detail;}

// class index_out_of_bounde

index_out_of_bound::index_out_of_bound(const char *detail): ContainerException(detail) {}

index_out_of_bound::index_out_of_bound(const std::string &detail): ContainerException(detail.c_str()) {}

const char *index_out_of_bound::what() const noexcept {return _detail;}

// class container_is_empty

container_is_empty::container_is_empty(const char *detail): ContainerException(detail) {}

container_is_empty::container_is_empty(const std::string &detail): ContainerException(detail.c_str()) {}

const char *container_is_empty::what() const noexcept {return _detail;}
// class invalid_iterator

}