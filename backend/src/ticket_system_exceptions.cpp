#include "ticket_system_exceptions.h"

namespace TicketSystem {

// class TicketSystemException

TicketSystemException::TicketSystemException(): _detail("") {}

TicketSystemException::TicketSystemException(const char *detail): _detail(detail) {}

TicketSystemException::TicketSystemException(const std::string &detail): _detail(detail.c_str()) {}

const char *TicketSystemException::what() const noexcept {return _detail;}

// class ValidationFailure

ValidationFailure::ValidationFailure() {}

ValidationFailure::ValidationFailure(const char *detail): TicketSystemException(detail) {}

ValidationFailure::ValidationFailure(const std::string &detail): TicketSystemException(detail) {}

const char *ValidationFailure::what() const noexcept {return _detail;}
} // namespace TicketSystem