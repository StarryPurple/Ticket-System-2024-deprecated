#include "backend_system.h"

namespace TicketSystem {

// startups / shutdowns

BackendSystem::BackendSystem()
  : func_list{
    [this](const arglist_t &args){return _user_mgr.add_user(args);},
    [this](const arglist_t &args){return _user_mgr.login(args);},
    [this](const arglist_t &args){return _user_mgr.logout(args);},
    [this](const arglist_t &args){return _user_mgr.query_profile(args);},

    [this](const arglist_t &args){return _user_mgr.modify_profile(args);},
    [this](const arglist_t &args){return _train_mgr.add_train(args);},
    [this](const arglist_t &args){return _train_mgr.delete_train(args);},
    [this](const arglist_t &args){return _train_mgr.release_train(args);},

    [this](const arglist_t &args){return _train_mgr.query_train(args);},
    [this](const arglist_t &args){return _train_mgr.query_ticket(args);},
    [this](const arglist_t &args){return _train_mgr.query_transfer(args);},
    [this](const arglist_t &args){return _user_mgr.buy_ticket(args);},

    [this](const arglist_t &args){return _user_mgr.query_order(args);},
    [this](const arglist_t &args){return _user_mgr.refund_ticket(args);},
    [this](const arglist_t &args){return _db_set.clean(args);},
    [this](const arglist_t &args){return exit(args);},
  } {}

BackendSystem::ReturnInfo::ReturnInfo(const std::string &_info, const Signal &_signal)
  : info(_info), signal(_signal) {}

BackendSystem::CommandPair::CommandPair(const key_t &_key, const arg_t &_arg)
  : key(_key), arg(_arg) {}

bool BackendSystem::DatabaseSet::open(const std::string &working_dir) {
  user_db.open(working_dir + "/user_db.dat");
  user_index_db.open(working_dir + "/user_index_db.dat");
  user_ticket_status_db.open(working_dir + "/user_ticket_status_db.dat");
  pending_ticket_status_ds.open(working_dir + "/pending_ticket_status_ds.dat");
  train_db.open(working_dir + "/train_db.dat");
  train_index_db.open(working_dir + "/train_index_db.dat");
  station_db.open(working_dir + "/station_db.dat");
  if(user_db.is_open() && user_index_db.is_open()
    && user_ticket_status_db.is_open() && pending_ticket_status_ds.is_open()
    && train_db.is_open() && train_index_db.is_open()
    && station_db.is_open()) return true;
  user_db.close();
  user_index_db.close();
  user_ticket_status_db.close();
  pending_ticket_status_ds.close();
  train_db.close();
  train_index_db.close();
  station_db.close();
  return false;
}

bool BackendSystem::DatabaseSet::is_open() const {
  return user_db.is_open();
}

void BackendSystem::DatabaseSet::close() {
  if(!is_open()) return;
  user_db.close();
  user_index_db.close();
  user_ticket_status_db.close();
  pending_ticket_status_ds.close();
  train_db.close();
  train_index_db.close();
  station_db.close();
}

BackendSystem::DatabaseSet::~DatabaseSet() {
  close();
}

void BackendSystem::UserManager::startup(DatabaseSet &db_set) {
  _db_set = &db_set;
}

void BackendSystem::TrainManager::startup(DatabaseSet &db_set) {
  _db_set = &db_set;
}

void BackendSystem::UserManager::shutdown() {
  _db_set = nullptr;
}

void BackendSystem::TrainManager::shutdown() {
  _db_set = nullptr;
}

bool BackendSystem::startup(const std::string &data_dir) {
  bool open_success = _db_set.open(data_dir);
  if(!open_success) return false;
  _user_mgr.startup(_db_set);
  _train_mgr.startup(_db_set);
  return true;
}

void BackendSystem::shutdown() {
  _db_set.close();
  _user_mgr.shutdown();
  _train_mgr.shutdown();
}

bool BackendSystem::is_open() const {
  return _db_set.is_open();
}






// BackendSystem main part



BackendSystem::Command BackendSystem::read_command(const std::string &command) {
  ism::vector<std::string> argv;
  std::string arg;
  for(auto &ch: command) {
    if(ch == ' ' || ch == '\n' || ch == '\r') {
      if(!arg.empty()) argv.push_back(arg);
      arg.clear();
    } else arg += ch;
  }
  if(!arg.empty()) argv.push_back(arg);
  Command cmd;
  cmd.timestamp = ism::stoi(argv[0]);
  cmd.cmd_name = argv[1];
  for(int i = 2; i < arg.length(); i += 2)
    cmd.arglist.push_back(CommandPair{argv[i][1], argv[i + 1]});
  return cmd;
}

void BackendSystem::sort_arglist(const arglist_t &arglist, arglist_t &sorted_arglist) {
  for(const auto &arg: arglist) {
    for(auto &sorted_arg: sorted_arglist)
      if(arg.key == sorted_arg.key) {
        sorted_arg.arg = arg.arg;
        break;
      }
  }
}

BackendSystem::Signal BackendSystem::run_command(const Command &command) const {
  ReturnInfo result;
  for(int i = 0; i < k_opr_cnt; ++i)
    if(command.cmd_name == func_name_list[i]) {
      arglist_t sorted_arglist = func_default_arglist[i];
      sort_arglist(command.arglist, sorted_arglist);
      result = func_list[i](sorted_arglist);
      break;
    }
  std::cout << result.info << std::endl;
  return result.signal;
  /*
  if(command.cmd_name == "add_user")
    result = _user_mgr.add_user(command.arg_list);
  else if(command.cmd_name == "login")
    result = _user_mgr.login(command.arg_list);
  else if(command.cmd_name == "logout")
    result = _user_mgr.logout(command.arg_list);
  else if(command.cmd_name == "query_profile")
    result = _user_mgr.query_profile(command.arg_list);
  else if(command.cmd_name == "modify_profile")
    result = _user_mgr.modify_profile(command.arg_list);
  ...
  */
}

BackendSystem::Signal BackendSystem::run_command(const std::string &command) const {
  return run_command(read_command(command));
}

// the operations.

BackendSystem::ReturnInfo BackendSystem::UserManager::add_user(const arglist_t &args) {


}

BackendSystem::ReturnInfo BackendSystem::UserManager::login(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::UserManager::logout(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::UserManager::query_profile(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::UserManager::modify_profile(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::TrainManager::add_train(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::TrainManager::delete_train(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::TrainManager::release_train(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::TrainManager::query_train(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::TrainManager::query_ticket(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::TrainManager::query_transfer(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::UserManager::buy_ticket(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::UserManager::query_order(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::UserManager::refund_ticket(const arglist_t &args) {

}

BackendSystem::ReturnInfo BackendSystem::DatabaseSet::clean(const arglist_t &args) {
  // args not used
  user_index_db.renew();
  user_db.renew();
  train_index_db.renew();
  train_db.renew();
  station_db.renew();
  return {"0", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::exit(const arglist_t &args) {
  return {"bye", Signal::sig_terminal};
}




} // namespace TicketSystem