#include "lib_exceptions.h"

namespace Insomnia {

// class FileSystemException

FileSystemException::FileSystemException(): _detail("") {}

FileSystemException::FileSystemException(const char *detail): _detail(detail) {}

FileSystemException::FileSystemException(const std::string &detail): _detail(detail.c_str()) {}

const char *FileSystemException::what() const noexcept {return _detail;}
} // namespace Insomnia