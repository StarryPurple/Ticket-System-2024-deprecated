// Actually all these classes can and should be nestled in one class (and one file).
// But that's toooo~ large and humble.
// TAs say multiple files are better. Make them happy.

#ifndef TICKET_SYSTEM_INTERFACE_SYSTEM_H
#define TICKET_SYSTEM_INTERFACE_SYSTEM_H

#include "backend_system.h"

#include <iostream>

namespace TicketSystem {

class InterfaceSystem {
  BackendSystem _backend_mgr;
public:
  void interface(const std::string &data_dir);
};

} // namespace TicketSystem

#endif // TICKET_SYSTEM_INTERFACE_SYSTEM_H