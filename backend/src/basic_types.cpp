#include "basic_types.h"

namespace TicketSystem {

// class User

User::privilege_t User::privilege() const {
  return _privilege;
}


} // namespace TicketSystem