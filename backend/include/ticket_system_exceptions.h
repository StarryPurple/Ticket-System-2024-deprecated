#ifndef TICKET_SYSTEM_EXCEPTIONS_H
#define TICKET_SYSTEM_EXCEPTIONS_H

#include <iostream>
// #include <stdexcept>

namespace TicketSystem {
class TicketSystemException: std::exception {
protected:
  const char *_detail;
public:
  TicketSystemException();
  TicketSystemException(const char *detail);
  TicketSystemException(const std::string &detail);
  const char *what() const noexcept override;
};

class ValidationFailure final: TicketSystemException {
public:
  ValidationFailure();
  ValidationFailure(const char *detail);
  ValidationFailure(const std::string &detail);
  const char *what() const noexcept final;
};
} // namespace TicketSystem


#endif // TICKET_SYSTEM_EXCEPTIONS_H