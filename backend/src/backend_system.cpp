#include "backend_system.h"

namespace TicketSystem {

// static constant

const std::string BackendSystem::func_name_list[k_opr_cnt] = {
  "add_user", "login", "logout", "query_profile",
  "modify_profile", "add_train", "delete_train", "release_train",
  "query_train", "query_ticket", "query_transfer", "buy_ticket",
  "query_order", "refund_ticket", "clean", "exit"
};

const BackendSystem::arglist_t BackendSystem::func_default_arglist[k_opr_cnt] = {
  {{'c', ""}, {'u', ""}, {'p', ""}, {'n', ""}, {'m', ""}, {'g', ""}}, // add_user
  {{'u', ""}, {'p', ""}},                                             // login
  {{'u', ""}},                                                        // logout
  {{'c', ""}, {'u', ""}},                                             // query_profile
  {{'c', ""}, {'u', ""}, {'p', ""}, {'n', ""}, {'m', ""}, {'g', ""}}, // modify_profile
  {{'i', ""}, {'n', ""}, {'m', ""}, {'s', ""}, {'p', ""}, {'x', ""},
   {'t', ""}, {'o', ""}, {'d', ""}, {'y', ""}},                       // add_train
  {{'i', ""}},                                                        // delete_train
  {{'i', ""}},                                                        // release_train
  {{'i', ""}, {'d', ""}},                                             // query_train
  {{'s', ""}, {'t', ""}, {'d', ""}, {'p', "time"}},                   // query_ticket
  {{'s', ""}, {'t', ""}, {'d', ""}, {'p', "time"}},                   // query_transfer
  {{'u', ""}, {'i', ""}, {'d', ""}, {'n', ""}, {'f', ""}, {'t', ""},
   {'q', "false"}},                                                   // query_ticket
  {{'u', ""}},                                                        // query_order
  {{'u', ""}, {'n', "1"}},                                            // refund_ticket
  {},                                                                 // clean
  {}                                                                  // exit
};

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

void BackendSystem::DatabaseSet::renew(const std::string &working_dir) {
  user_db.renew(working_dir + "/user_db.dat");
  user_index_db.renew(working_dir + "/user_index_db.dat");
  user_ticket_status_db.renew(working_dir + "/user_ticket_status_db.dat");
  pending_ticket_status_ds.renew(working_dir + "/pending_ticket_status_ds.dat");
  train_db.renew(working_dir + "/train_db.dat");
  train_index_db.renew(working_dir + "/train_index_db.dat");
  station_db.renew(working_dir + "/station_db.dat");
  info_rec.renew(working_dir + "/info_rec.dat");
  info.user_index_tot = 0;
  info.train_index_tot = 0;
}

void BackendSystem::DatabaseSet::renew() {
  user_index_db.renew();
  user_db.renew();
  user_ticket_status_db.renew();
  pending_ticket_status_ds.renew();
  train_index_db.renew();
  train_db.renew();
  station_db.renew();
  info_rec.renew();
  info.user_index_tot = 0;
  info.train_index_tot = 0;
}


bool BackendSystem::DatabaseSet::open(const std::string &working_dir) {
  user_db.open(working_dir + "/user_db.dat");
  user_index_db.open(working_dir + "/user_index_db.dat");
  user_ticket_status_db.open(working_dir + "/user_ticket_status_db.dat");
  pending_ticket_status_ds.open(working_dir + "/pending_ticket_status_ds.dat");
  train_db.open(working_dir + "/train_db.dat");
  train_index_db.open(working_dir + "/train_index_db.dat");
  station_db.open(working_dir + "/station_db.dat");
  info_rec.open(working_dir + "/info_rec.dat");
  if(user_db.is_open() && user_index_db.is_open()
    && user_ticket_status_db.is_open() && pending_ticket_status_ds.is_open()
    && train_db.is_open() && train_index_db.is_open()
    && station_db.is_open() && info_rec.is_open()) {
    info_rec.read_info(info);
    return true;
  }
  user_db.close();
  user_index_db.close();
  user_ticket_status_db.close();
  pending_ticket_status_ds.close();
  train_db.close();
  train_index_db.close();
  station_db.close();
  info_rec.close();
  return false;
}

bool BackendSystem::DatabaseSet::is_open() const {
  return user_db.is_open();
}

void BackendSystem::DatabaseSet::close() {
  if(!is_open()) return;
  info_rec.write_info(info);
  user_db.close();
  user_index_db.close();
  user_ticket_status_db.close();
  pending_ticket_status_ds.close();
  train_db.close();
  train_index_db.close();
  station_db.close();
  info_rec.close();
}

BackendSystem::DatabaseSet::~DatabaseSet() {
  close();
}

void BackendSystem::UserManager::startup(DatabaseSet &_db_set) {
  db_set = &_db_set;
  active_users.clear();
}

void BackendSystem::TrainManager::startup(DatabaseSet &_db_set) {
  db_set = &_db_set;
}

void BackendSystem::UserManager::shutdown() {
  db_set = nullptr;
}

void BackendSystem::TrainManager::shutdown() {
  db_set = nullptr;
}

bool BackendSystem::startup(const std::string &data_dir) {
  bool open_success = _db_set.open(data_dir);
  if(!open_success) {
    _db_set.renew(data_dir);
    _db_set.open(data_dir);
  }
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

void BackendSystem::sort_arglist(const raw_arglist_t &arglist, arglist_t &sorted_arglist) {
  for(const auto &arg: arglist)
    sorted_arglist[arg.key] = arg.arg;
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
  // {{'c', ""}, {'u', ""}, {'p', ""}, {'n', ""}, {'m', ""}, {'g', ""}}
  User::username_t cur_username = args.at('c');
  User::username_t username = args.at('u');
  User::password_t password = args.at('p');
  User::real_name_t real_name = args.at('n');
  User::mail_addr_t mail_addr = args.at('m');
  User::privilege_t privilege = ism::stoi(args.at('g'));

  if(db_set->user_index_db.empty()) {
    privilege = 10;
  } else {
    if(!active_users.contains(cur_username)) return {"-1", Signal::sig_normal};
    if(!db_set->user_index_db.list(username).empty()) return {"-1", Signal::sig_normal};
    auto cur_user_index = db_set->user_index_db.list(cur_username)[0];
    User cur_user = db_set->user_db.list(cur_user_index)[0];
    if(cur_user.privilege <= privilege) return {"-1", Signal::sig_normal};
  }
  User user{username, password, real_name, mail_addr, privilege};
  auto user_index = ++db_set->info.user_index_tot;
  db_set->user_index_db.insert(username, user_index);
  db_set->user_db.insert(user_index, user);
  return {"0", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::UserManager::login(const arglist_t &args) {
  // {{'u', ""}, {'p', ""}}
  User::username_t username = args.at('u');
  User::password_t password = args.at('p');
  auto user_index_list = db_set->user_index_db.list(username);
  if(user_index_list.empty()) return {"-1", Signal::sig_normal};
  auto user = db_set->user_db.list(user_index_list[0])[0];
  if(password != user.password) return {"-1", Signal::sig_normal};
  active_users.insert(username);
  return {"0", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::UserManager::logout(const arglist_t &args) {
  // {{'u', ""}}
  User::username_t username = args.at('u');
  auto iter = active_users.find(username);
  if(iter == active_users.end()) return {"-1", Signal::sig_normal};
  active_users.erase(iter);
  // or just use the return value of erase() to check.
  return {"0", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::UserManager::query_profile(const arglist_t &args) {
  // {{'c', ""}, {'u', ""}}

}

BackendSystem::ReturnInfo BackendSystem::UserManager::modify_profile(const arglist_t &args) {
  // {{'c', ""}, {'u', ""}, {'p', ""}, {'n', ""}, {'m', ""}, {'g', ""}}
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::add_train(const arglist_t &args) {
  // {'i', ""}, {'n', ""}, {'m', ""}, {'s', ""}, {'p', ""}, {'x', ""},
  // {'t', ""}, {'o', ""}, {'d', ""}, {'y', ""}
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::delete_train(const arglist_t &args) {
  // {{'i', ""}}
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::release_train(const arglist_t &args) {
  // {{'i', ""}}
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::query_train(const arglist_t &args) {
  // {{'i', ""}, {'d', ""}}
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::query_ticket(const arglist_t &args) {
  // {{'s', ""}, {'t', ""}, {'d', ""}, {'p', "time"}}
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::query_transfer(const arglist_t &args) {
  // {{'s', ""}, {'t', ""}, {'d', ""}, {'p', "time"}}
}

BackendSystem::ReturnInfo BackendSystem::UserManager::buy_ticket(const arglist_t &args) {
  // {'u', ""}, {'i', ""}, {'d', ""}, {'n', ""}, {'f', ""}, {'t', ""},
  // {'q', "false"}
}

BackendSystem::ReturnInfo BackendSystem::UserManager::query_order(const arglist_t &args) {
  // {{'u', ""}}
}

BackendSystem::ReturnInfo BackendSystem::UserManager::refund_ticket(const arglist_t &args) {
  // {{'u', ""}, {'n', "1"}}
}

BackendSystem::ReturnInfo BackendSystem::DatabaseSet::clean(const arglist_t &args) {
  // {}
  renew();
  return {"0", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::exit(const arglist_t &args) {
  // {}
  return {"bye", Signal::sig_exit};
}




} // namespace TicketSystem