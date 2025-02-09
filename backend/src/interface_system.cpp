#include "interface_system.h"

namespace TicketSystem {

void InterfaceSystem::interface(const std::string &data_dir) {
  _backend_mgr.startup(data_dir);
  while(true) {
    std::string command;
    getline(std::cin, command);
    if(const auto signal = _backend_mgr.run_command(command);
      signal == BackendSystem::Signal::sig_terminal) break;
  }
  _backend_mgr.shutdown();
}

} // namespace TicketSystem