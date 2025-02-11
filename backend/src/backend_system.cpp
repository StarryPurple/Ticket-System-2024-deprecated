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

// startups / shutdowns / basics

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
    [this](const arglist_t &args){return clean(args);},
    [this](const arglist_t &args){return exit(args);},
  } {}

BackendSystem::ReturnInfo::ReturnInfo(const std::string &_info, const Signal &_signal)
  : info(_info), signal(_signal) {}

BackendSystem::CommandPair::CommandPair(const key_t &_key, const arg_t &_arg)
  : key(_key), arg(_arg) {}

void BackendSystem::DatabaseSet::renew(const std::string &working_dir) {
  user_db.renew(working_dir + "/user_db.dat");
  user_index_db.renew(working_dir + "/user_index_db.dat");
  user_ticket_status_index_db.renew(working_dir + "/user_ticket_status_index_db.dat");
  ticket_status_db.renew(working_dir + "/ticket_status_db.dat");
  pending_list_db.renew(working_dir + "/pending_ticket_status_ds.dat");
  train_db.renew(working_dir + "/train_db.dat");
  train_index_db.renew(working_dir + "/train_index_db.dat");
  station_db.renew(working_dir + "/station_db.dat");
  info_rec.renew(working_dir + "/info_rec.dat");
  info.user_index_tot = 0;
  info.train_index_tot = 0;
  info.ticket_request_tot = 0;
}

void BackendSystem::DatabaseSet::renew() {
  user_index_db.renew();
  user_db.renew();
  user_ticket_status_index_db.renew();
  ticket_status_db.renew();
  pending_list_db.renew();
  train_index_db.renew();
  train_db.renew();
  station_db.renew();
  info_rec.renew();
  info.user_index_tot = 0;
  info.train_index_tot = 0;
  info.ticket_request_tot = 0;
}


bool BackendSystem::DatabaseSet::open(const std::string &working_dir) {
  user_db.open(working_dir + "/user_db.dat");
  user_index_db.open(working_dir + "/user_index_db.dat");
  user_ticket_status_index_db.open(working_dir + "/user_ticket_status_index_db.dat");
  ticket_status_db.open(working_dir + "/ticket_status_db.dat");
  pending_list_db.open(working_dir + "/pending_ticket_status_ds.dat");
  train_db.open(working_dir + "/train_db.dat");
  train_index_db.open(working_dir + "/train_index_db.dat");
  station_db.open(working_dir + "/station_db.dat");
  info_rec.open(working_dir + "/info_rec.dat");
  if(user_db.is_open() && user_index_db.is_open()
    && user_ticket_status_index_db.is_open() && pending_list_db.is_open()
    && train_db.is_open() && train_index_db.is_open()
    && station_db.is_open() && info_rec.is_open()) {
    info_rec.read_info(info);
    return true;
  }
  user_db.close();
  user_index_db.close();
  user_ticket_status_index_db.close();
  ticket_status_db.close();
  pending_list_db.close();
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
  user_ticket_status_index_db.close();
  ticket_status_db.close();
  pending_list_db.close();
  train_db.close();
  train_index_db.close();
  station_db.close();
  info_rec.close();
}

BackendSystem::DatabaseSet::~DatabaseSet() {
  close();
}

BackendSystem::DatabaseSet::station_info_t::station_info_t(
  const Train::train_id_t &_train_id, const station_order_t &_order)
  : train_id(_train_id), order(_order) {}

bool BackendSystem::DatabaseSet::station_info_t::operator==(const station_info_t &other) const {
  return train_id == other.train_id && order == other.order;
}

bool BackendSystem::DatabaseSet::station_info_t::operator!=(const station_info_t &other) const {
  return train_id != other.train_id || order != other.order;
}

bool BackendSystem::DatabaseSet::station_info_t::operator<(const station_info_t &other) const {
  if(train_id != other.train_id) return train_id < other.train_id;
  return order < other.order;
}

bool BackendSystem::DatabaseSet::station_info_t::operator>(const station_info_t &other) const {
  if(train_id != other.train_id) return train_id > other.train_id;
  return order > other.order;
}

bool BackendSystem::DatabaseSet::station_info_t::operator<=(const station_info_t &other) const {
  if(train_id != other.train_id) return train_id < other.train_id;
  return order <= other.order;
}

bool BackendSystem::DatabaseSet::station_info_t::operator>=(const station_info_t &other) const {
  if(train_id != other.train_id) return train_id > other.train_id;
  return order >= other.order;
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
    open_success = _db_set.open(data_dir);
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
  if(argv.empty()) return cmd;
  argv[0].pop_back();
  argv[0].erase(argv[0].begin());
  cmd.timestamp = ism::stoi(argv[0]);
  if(argv.size() == 1) return cmd;
  cmd.cmd_name = argv[1];
  for(int i = 2; i < argv.size(); i += 2)
    cmd.arglist.push_back(CommandPair{argv[i][1], argv[i + 1]});
  return cmd;
}

void BackendSystem::sort_arglist(const raw_arglist_t &arglist, arglist_t &sorted_arglist) {
  for(const auto &arg: arglist)
    sorted_arglist[arg.key] = arg.arg;
}

BackendSystem::Signal BackendSystem::run_command(const Command &command) const {
  ReturnInfo result;
  bool is_valid = false;
  for(int i = 0; i < k_opr_cnt; ++i)
    if(command.cmd_name == func_name_list[i]) {
      arglist_t sorted_arglist = func_default_arglist[i];
      sort_arglist(command.arglist, sorted_arglist);
      result = func_list[i](sorted_arglist);
      is_valid = true;
      break;
    }
  if(!is_valid) {
    if(command.timestamp == Command::n_timestamp && !command.cmd_name.empty())
      std::cout << '[' << command.timestamp << ']' << " Command \'" + command.cmd_name << "\' not found." << std::endl;
    return Signal::sig_normal;
  }
  std::cout << '[' << command.timestamp << ']' << ' ' << result.info << std::endl;
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

  if(active_users.contains(username)) return {"-1", Signal::sig_normal};
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
  User::username_t cur_username = args.at('c');
  User::username_t username = args.at('u');

  if(!active_users.contains(cur_username)) return {"-1", Signal::sig_normal};
  auto index_list = db_set->user_index_db.list(username);
  if(index_list.empty()) return {"-1", Signal::sig_normal};
  User cur_user = db_set->user_db.list(db_set->user_index_db.list(cur_username)[0])[0];
  User user = db_set->user_db.list(index_list[0])[0];
  if(cur_user.privilege < user.privilege) return {"-1", Signal::sig_normal};
  return {user.str(), Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::UserManager::modify_profile(const arglist_t &args) {
  // {{'c', ""}, {'u', ""}, {'p', ""}, {'n', ""}, {'m', ""}, {'g', ""}}
  User::username_t cur_username = args.at('c');
  User::username_t username = args.at('u');
  User::password_t password = args.at('p');
  User::real_name_t real_name = args.at('n');
  User::mail_addr_t mail_addr = args.at('m');
  User::privilege_t privilege = ism::stoi(args.at('g'));

  if(!active_users.contains(cur_username)) return {"-1", Signal::sig_normal};
  auto index_list = db_set->user_index_db.list(username);
  if(index_list.empty()) return {"-1", Signal::sig_normal};
  User cur_user = db_set->user_db.list(db_set->user_index_db.list(cur_username)[0])[0];
  auto user_index = index_list[0];
  User user = db_set->user_db.list(user_index)[0];
  if(cur_user.privilege < user.privilege) return {"-1", Signal::sig_normal};
  if(cur_user.privilege < user.privilege || cur_user.privilege <= privilege) return {"-1", Signal::sig_normal};

  db_set->user_db.erase(user_index, user);
  if(!args.at('p').empty()) user.password = password;
  if(!args.at('n').empty()) user.real_name = real_name;
  if(!args.at('m').empty()) user.mail_addr = mail_addr;
  if(!args.at('g').empty()) user.privilege = privilege;
  db_set->user_db.insert(user_index, user);
  return {user.str(), Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::add_train(const arglist_t &args) {
  // {'i', ""}, {'n', ""}, {'m', ""}, {'s', ""}, {'p', ""}, {'x', ""},
  // {'t', ""}, {'o', ""}, {'d', ""}, {'y', ""}
  Train::train_id_t train_id = args.at('i');
  if(!db_set->train_index_db.list(train_id).empty()) return {"-1", Signal::sig_normal};
  Train::station_num_t station_num = ism::stoi(args.at('n'));
  Train::seat_num_t passenger_capacity = ism::stoi(args.at('m'));
  ism::vector<std::string> station_names_str = ism::string_split(args.at('s'), '|');
  ism::vector<Train::station_name_t> station_names;
  for(const auto &st: station_names_str) station_names.push_back(st);
  ism::vector<std::string> prices_str = ism::string_split(args.at('p'), '|');
  ism::vector<Train::price_t> prices;
  for(const auto &pr: prices_str) prices.push_back(ism::stoi(pr));
  Train::time_hm_t start_time = args.at('x');
  ism::vector<std::string> travel_times_str = ism::string_split(args.at('t'), '|');
  ism::vector<Train::time_dur_t> travel_times;
  for(const auto &tr: travel_times_str) travel_times.push_back(ism::stoi(tr));
  ism::vector<std::string> stopover_times_str = ism::string_split(args.at('o'), '|');
  ism::vector<Train::time_dur_t> stopover_times;
  if(station_num > 2)
    for(const auto &st: stopover_times_str) stopover_times.push_back(ism::stoi(st));
  ism::vector<std::string> sale_dates_str = ism::string_split(args.at('d'), '|');
  ism::vector<Train::date_md_t> sale_dates;
  for(const auto &sa: sale_dates_str) sale_dates.push_back(sa);
  Train::train_type_t train_type = args.at('y')[0];

  Train train{train_id, station_num, passenger_capacity, station_names, prices, start_time,
              travel_times, stopover_times, sale_dates[0], sale_dates[1], train_type};
  auto train_index = ++db_set->info.train_index_tot;
  db_set->train_index_db.insert(train_id, train_index);
  db_set->train_db.insert(train_index, train);
  for(int i = 0; i < station_num; ++i)
    db_set->station_db.insert(station_names[i], {train_id, i});
  return {"0", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::delete_train(const arglist_t &args) {
  // {{'i', ""}}
  Train::train_id_t train_id = args.at('i');

  auto train_index_list = db_set->train_index_db.list(train_id);
  if(train_index_list.empty()) return {"-1", Signal::sig_normal};
  auto train_index = train_index_list[0];
  Train train = db_set->train_db.list(train_index)[0];
  if(train.has_released) return {"-1", Signal::sig_normal};
  db_set->train_db.erase(train_index, train);
  db_set->train_index_db.erase(train_id, train_index);
  for(int i = 0; i < train.station_num; ++i)
    db_set->station_db.erase(train.station_names[i], {train.train_id, i});
  return {"0", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::release_train(const arglist_t &args) {
  // {{'i', ""}}
  Train::train_id_t train_id = args.at('i');

  auto train_index_list = db_set->train_index_db.list(train_id);
  if(train_index_list.empty()) return {"-1", Signal::sig_normal};
  auto train_index = train_index_list[0];
  Train train = db_set->train_db.list(train_index)[0];
  if(train.has_released) return {"-1", Signal::sig_normal};

  db_set->train_db.erase(train_index, train);
  train.has_released = true;
  db_set->train_db.insert(train_index, train);
  return {"0", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::query_train(const arglist_t &args) {
  // {{'i', ""}, {'d', ""}}
  Train::train_id_t train_id = args.at('i');
  Date_md date_md = args.at('d');

  auto train_index_list = db_set->train_index_db.list(train_id);
  if(train_index_list.empty()) return {"-1", Signal::sig_normal};
  auto train_index = train_index_list[0];
  Train train = db_set->train_db.list(train_index)[0];
  if(date_md < train.sale_date_first || date_md > train.sale_date_last) return {"-1", Signal::sig_normal};
  std::string result = train.train_id.str() + ' ' + train.train_type;
  for(int i = 0; i < train.station_num; ++i)
    result += '\n' + train.station_profile(date_md, i);
  return {result, Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::query_ticket(const arglist_t &args) {
  // {{'s', ""}, {'t', ""}, {'d', ""}, {'p', "time"}}
  Train::station_name_t start_station = args.at('s');
  Train::station_name_t terminal_station = args.at('t');
  Date_md date_md_mid = args.at('d');
  std::string sort_type = args.at('p');

  struct Temp {
    std::string info;
    Train::train_id_t train_id;
    Train::time_dur_t time;
    Train::price_t cost;
  };

  ism::vector<Temp> res_list;

  const auto train_list_s = db_set->station_db.list(start_station);
  const auto train_list_t = db_set->station_db.list(terminal_station);
  for(int i = 0, j = 0; i < train_list_s.size() && j < train_list_t.size(); ) {
    if(train_list_s[i].train_id < train_list_t[j].train_id) {++i; continue;}
    else if(train_list_s[i].train_id > train_list_t[j].train_id) {++j; continue;}
    if(train_list_s[i].order >= train_list_t[j].order) {++i; ++j; continue;}
    const Train &train = db_set->train_db.list(db_set->train_index_db.list(train_list_t[j].train_id)[0])[0];
    if(!train.has_released) {++i; ++j; continue;}
    const int index_s = train_list_s[i].order;
    const int index_t = train_list_t[j].order;
    Date last_date_mid = Date{train.sale_date_last, train.start_time}.add_minutes(train.leaving_time_diff[index_s]);
    auto days_diff = last_date_mid.date_md - date_md_mid;
    Date_md date_md_s = train.sale_date_last.subtract_days(days_diff);
    if(date_md_s < train.sale_date_first || date_md_s > train.sale_date_last) {++i; ++j; continue;}
    Date date_s = {date_md_s, train.start_time};
    int date_s_exact = date_md_s.exact_number();
    Temp temp;
    temp.info = train.train_id.str() + ' ' + start_station.str() + ' ' +
                date_s.add_minutes(train.leaving_time_diff[index_s]).str() + " -> " +
                terminal_station.str() + ' ' +
                date_s.add_minutes(train.arriving_time_diff[index_t]).str();
    Train::price_t price = train.accumulated_prices[index_t] - train.accumulated_prices[index_s];
    Train::seat_num_t seat_num = Train::k_max_seat_num + 1;
    for(int k = index_s; k < index_t; ++k)
      if(seat_num > train.unsold_seat_nums[date_s_exact][k]) seat_num = train.unsold_seat_nums[date_s_exact][k];
    temp.info += ' ' + ism::itos(price) + ' ' + ism::itos(seat_num);
    temp.time = train.arriving_time_diff[index_t] - train.leaving_time_diff[index_s];
    temp.cost = price;
    temp.train_id = train.train_id;
    res_list.push_back(temp);
    ++i; ++j;
  }
  if(sort_type == "time")
    ism::sort(res_list.begin(), res_list.end(), [](const Temp &lhs, const Temp &rhs) {
      if(lhs.time == rhs.time) return lhs.train_id < rhs.train_id;
      return lhs.time < rhs.time;
    });
  else if(sort_type == "cost")
    ism::sort(res_list.begin(), res_list.end(), [](const Temp &lhs, const Temp &rhs) {
      if(lhs.cost == rhs.cost) return lhs.train_id < rhs.train_id;
      return lhs.cost < rhs.cost;
    });
  std::string result = ism::itos(res_list.size());
  for(auto &temp: res_list) result += '\n' + temp.info;
  return {result, Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::TrainManager::query_transfer(const arglist_t &args) {
  // {{'s', ""}, {'t', ""}, {'d', ""}, {'p', "time"}}
  Train::station_name_t start_station = args.at('s');
  Train::station_name_t terminal_station = args.at('t');
  Date_md date_md_mid1 = args.at('d');
  std::string sort_type = args.at('p');

  struct Info {
    Train train1, train2;
    Train::station_order_t order1s, order1t, order2s, order2t;
    Date date1s, date1t, date2s, date2t;
    int date_exact1, date_exact2;
    Train::time_dur_t time = Train::k_time_inf;
    Train::price_t cost = Train::k_cost_inf;
    std::string str() const {
      std::string res;
      res += train1.train_id.str();
      res += ' ' + train1.station_names[order1s].str();
      res += ' ' + date1s.str();
      res += " -> " + train1.station_names[order1t].str();
      res += ' ' + date1t.str();
      res += ' ' + ism::itos(train1.accumulated_prices[order1t] - train1.accumulated_prices[order1s]);
      Train::seat_num_t seat_num1 = Train::k_max_seat_num + 1;
      for(int i = order1s; i < order1t; ++i)
        if(seat_num1 > train1.unsold_seat_nums[date_exact1][i]) seat_num1 = train1.unsold_seat_nums[date_exact1][i];
      res += ' ' + ism::itos(seat_num1);
      res += '\n';
      res += train2.train_id.str();
      res += ' ' + train2.station_names[order2s].str();
      res += ' ' + date2s.str();
      res += " -> " + train2.station_names[order2t].str();
      res += ' ' + date2t.str();
      res += ' ' + ism::itos(train2.accumulated_prices[order2t] - train2.accumulated_prices[order2s]);
      Train::seat_num_t seat_num2 = Train::k_max_seat_num + 1;
      for(int i = order2s; i < order2t; ++i)
        if(seat_num2 > train2.unsold_seat_nums[date_exact2][i]) seat_num2 = train2.unsold_seat_nums[date_exact2][i];
      res += ' ' + ism::itos(seat_num2);
      return res;
    }
  };

  auto less_comp = [sort_type](const Info &lhs, const Info &rhs) -> bool {
    if(sort_type == "time") {
      if(lhs.time != rhs.time) return lhs.time < rhs.time;
      if(lhs.cost != rhs.cost) return lhs.cost < rhs.cost;
      if(lhs.train1 != rhs.train1) return lhs.train1 < rhs.train1;
      return lhs.train2 < rhs.train2;
    } else if(sort_type == "price") {
      if(lhs.cost != rhs.cost) return lhs.cost < rhs.cost;
      if(lhs.time != rhs.time) return lhs.time < rhs.time;
      if(lhs.train1 != rhs.train1) return lhs.train1 < rhs.train1;
      return lhs.train2 < rhs.train2;
    }
    // shouldn't reach here.
    if(lhs.train1 != rhs.train1) return lhs.train1 < rhs.train1;
    return lhs.train2 < rhs.train2;
  };

  Info info;


  const auto train_list_s = db_set->station_db.list(start_station);
  const auto train_list_t = db_set->station_db.list(terminal_station);

  struct station_t {
    Train::station_name_t station_name;
    Train train;
    Train::station_order_t order_s, order_t;
  };

  ism::vector<station_t> station_list_s, station_list_t;
  for(const auto &[train_id, order]: train_list_s) {
    Train train = db_set->train_db.list(db_set->train_index_db.list(train_id)[0])[0];
    for(int i = order + 1; i < train.station_num; ++i)
      station_list_s.push_back(station_t{train.station_names[i], train, order, i});
  }
  ism::sort(station_list_s.begin(), station_list_s.end(), [](const station_t &lhs, const station_t &rhs) {
    if(lhs.station_name == rhs.station_name) return lhs.train < rhs.train;
    return lhs.station_name < rhs.station_name;
  });
  for(const auto &[train_id, order]: train_list_t) {
    Train train = db_set->train_db.list(db_set->train_index_db.list(train_id)[0])[0];
    for(int i = 0; i < order; ++i)
      station_list_t.push_back(station_t{train.station_names[i], train, i, order});
  }
  ism::sort(station_list_t.begin(), station_list_t.end(), [](const station_t &lhs, const station_t &rhs) {
    if(lhs.station_name == rhs.station_name) return lhs.train < rhs.train;
    return lhs.station_name < rhs.station_name;
  });

  for(int i_start = 0, j_start = 0; i_start < station_list_s.size() && j_start < station_list_t.size(); ) {
    if(station_list_s[i_start].station_name < station_list_t[j_start].station_name) {++i_start; continue;}
    if(station_list_s[i_start].station_name > station_list_t[j_start].station_name) {++j_start; continue;}
    const auto &station_name = station_list_t[j_start].station_name;
    int i_end = i_start, j_end = j_start;
    while(i_end + 1 < station_list_s.size() && station_list_s[i_end + 1].station_name == station_name)
      ++i_end;
    while(j_end + 1 < station_list_t.size() && station_list_t[j_end + 1].station_name == station_name)
      ++j_end;
    for(int i = i_start; i <= i_end; ++i) {
      if(!station_list_s[i].train.has_released) continue;
      for(int j = j_start; j <= j_end; ++j) {
        if(!station_list_t[j].train.has_released) continue;
        if(station_list_s[i].train == station_list_t[j].train) continue;
        const Train &tr1 = station_list_s[i].train, &tr2 = station_list_t[j].train;
        const Train::station_order_t
          order1s = station_list_s[i].order_s, order1t = station_list_s[i].order_t,
          order2s = station_list_t[j].order_s, order2t = station_list_t[j].order_t;
        Date end_date1s = Date{tr1.sale_date_last, tr1.start_time}.add_minutes(tr1.leaving_time_diff[order1s]);
        auto day_diff1 = end_date1s.date_md - date_md_mid1;
        Date_md start_date_md1 = tr1.sale_date_last.subtract_days(day_diff1);
        if(start_date_md1 < tr1.sale_date_first || start_date_md1 > tr1.sale_date_last) continue;
        Date start_date1 = {start_date_md1, tr1.start_time};
        Date date1s = start_date1.add_minutes(tr1.leaving_time_diff[order1s]);
        Date date1t = start_date1.add_minutes(tr1.arriving_time_diff[order1t]);
        Date_md date_md_mid2 = date1t.date_md;
        if(date1t.time_hm > tr2.start_time) date_md_mid2 = date_md_mid2.add_days(1);
        Date end_date2s = Date{tr2.sale_date_last, tr2.start_time}.add_minutes(tr2.leaving_time_diff[order2s]);
        auto day_diff2 = end_date2s.date_md - date_md_mid2;
        Date_md start_date_md2 = tr2.sale_date_last.subtract_days(day_diff2);
        if(start_date_md2 < tr2.sale_date_first || start_date_md2 > tr2.sale_date_last) continue;
        Date start_date2 = {start_date_md2, tr2.start_time};
        Date date2s = start_date2.add_minutes(tr2.leaving_time_diff[order2s]);
        Date date2t = start_date2.add_minutes(tr2.arriving_time_diff[order2t]);

        Train::time_dur_t time = date2t.get_diff_minutes(date1s);
        Train::price_t price =
          (tr1.accumulated_prices[order1t] - tr1.accumulated_prices[order1s]) +
          (tr2.accumulated_prices[order2t] - tr2.accumulated_prices[order2s]);
        Info current{tr1, tr2, order1s, order1t, order2s, order2t,
                     date1s, date1t, date2s, date2t,
                     start_date_md1.exact_number(), start_date_md2.exact_number(), time, price};
        if(less_comp(current, info)) info = current;
      }
    }
    i_start = i_end + 1;
    j_start = j_start + 1;
  }

  if(info.time == Train::k_time_inf && info.cost == Train::k_cost_inf) return {"0", Signal::sig_normal};
  return {info.str(), Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::UserManager::buy_ticket(const arglist_t &args) {
  // {'u', ""}, {'i', ""}, {'d', ""}, {'n', ""}, {'f', ""}, {'t', ""},
  // {'q', "false"}
  User::username_t username = args.at('u');
  Train::train_id_t train_id = args.at('i');
  Date_md date_md_mid = args.at('d');
  Train::seat_num_t seat_num = ism::stoi(args.at('n'));
  Train::station_name_t start_station = args.at('f');
  Train::station_name_t terminal_station = args.at('t');
  bool accept_queue = (args.at('q') == "true");

  if(!active_users.contains(username)) return {"-1", Signal::sig_normal};
  // can we afford this?
  int order_s = -1, order_t = -1;
  auto train_index_list = db_set->train_index_db.list(train_id);
  if(train_index_list.empty()) return {"-1", Signal::sig_normal};
  auto train_index = train_index_list[0];
  Train train = db_set->train_db.list(train_index)[0];
  if(!train.has_released) return {"-1", Signal::sig_normal};
  for(int i = 0; i < train.station_num; ++i) {
    if(train.station_names[i] == start_station) order_s = i;
    if(train.station_names[i] == terminal_station) order_t = i;
  }
  if(order_s == -1 || order_t == -1) return {"-1", Signal::sig_normal};
  auto start_date_md = train.sale_date_last.subtract_days(
    Date{train.sale_date_last, train.start_time}.add_minutes(train.leaving_time_diff[order_s]).date_md
    - date_md_mid);
  if(start_date_md < train.sale_date_first || start_date_md > train.sale_date_last)
    return {"-1", Signal::sig_normal};
  int day_exact = start_date_md.exact_number();
  Date date_s = Date{start_date_md, train.start_time}.add_minutes(train.leaving_time_diff[order_s]);
  Date date_t = Date{start_date_md, train.start_time}.add_minutes(train.arriving_time_diff[order_t]);
  Train::seat_num_t seat_num_left = Train::k_max_seat_num - 1;
  for(int i = order_s; i < order_t; ++i)
    if(seat_num_left > train.unsold_seat_nums[day_exact][i])
      seat_num_left = train.unsold_seat_nums[day_exact][i];
  Train::price_t price =
    train.accumulated_prices[order_t] - train.accumulated_prices[order_s];
  if(seat_num_left >= seat_num) {
    db_set->train_db.erase(train_index, train);
    for(int i = order_s; i < order_t; ++i)
      train.unsold_seat_nums[day_exact][i] -= seat_num;
    db_set->train_db.insert(train_index, train);
    auto ticket_request_index = ++db_set->info.ticket_request_tot;
    ticket_status_t ticket_status{ticket_status_t::status_t::success,
      ticket_request_index, train_id, start_date_md, order_s, order_t,
      start_station, terminal_station, date_s, date_t, price, seat_num};
    auto user_index = db_set->user_index_db.list(username)[0];
    db_set->user_ticket_status_index_db.insert(user_index, ticket_request_index);
    db_set->ticket_status_db.insert(ticket_request_index, ticket_status);
    return {ism::itos(price), Signal::sig_normal};
  } else {
    if(!accept_queue) return {"-1", Signal::sig_normal};
    auto ticket_request_index = ++db_set->info.ticket_request_tot;
    ticket_status_t ticket_status{ticket_status_t::status_t::pending,
      ticket_request_index, train_id, start_date_md, order_s, order_t,
      start_station, terminal_station, date_s, date_t, price, seat_num};
    auto user_index = db_set->user_index_db.list(username)[0];
    db_set->user_ticket_status_index_db.insert(user_index, ticket_request_index);
    db_set->ticket_status_db.insert(ticket_request_index, ticket_status);
    db_set->pending_list_db.insert({train_id, start_date_md.exact_number()}, ticket_request_index);
    return {"queue", Signal::sig_normal};
  }
}

BackendSystem::ReturnInfo BackendSystem::UserManager::query_order(const arglist_t &args) {
  // {{'u', ""}}
  User::username_t username = args.at('u');

  if(!active_users.contains(username)) return {"-1", Signal::sig_normal};
  auto user_index = db_set->user_index_db.list(username)[0];
  auto ticket_status_list = db_set->user_ticket_status_index_db.list(user_index);
  std::string result = ism::itos(ticket_status_list.size());
  for(int i = ticket_status_list.size() - 1; i >= 0; --i)
    result += '\n' + db_set->ticket_status_db.list(ticket_status_list[i])[0].str();
  return {result, Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::UserManager::refund_ticket(const arglist_t &args) {
  // {{'u', ""}, {'n', "1"}}
  User::username_t username = args.at('u');
  int number = ism::stoi(args.at('n'));

  if(!active_users.contains(username)) return {"-1", Signal::sig_normal};
  auto user_index = db_set->user_index_db.list(username)[0];
  auto ticket_status_list = db_set->user_ticket_status_index_db.list(user_index);
  if(ticket_status_list.size() < number) return {"-1", Signal::sig_normal};
  auto refund_ticket_index = ticket_status_list[ticket_status_list.size() - number];
  auto refund_ticket_status = db_set->ticket_status_db.list(refund_ticket_index)[0];
  if(refund_ticket_status.status == ticket_status_t::status_t::success) {
    db_set->ticket_status_db.erase(refund_ticket_index, refund_ticket_status);
    refund_ticket_status.status = ticket_status_t::status_t::refunded;
    db_set->ticket_status_db.insert(refund_ticket_index, refund_ticket_status);
    int date_exact = refund_ticket_status.start_date_md.exact_number();
    Train::train_id_t train_id = refund_ticket_status.train_id;
    auto train_index = db_set->train_index_db.list(train_id)[0];
    Train train = db_set->train_db.list(train_index)[0];
    db_set->train_db.erase(train_index, train);
    for(int i = refund_ticket_status.order_s; i < refund_ticket_status.order_t; ++i)
      train.unsold_seat_nums[date_exact][i] += refund_ticket_status.amount;
    // refund ended. Try to sell pending tickets.
    auto pending_list = db_set->pending_list_db.list({train_id, date_exact});
    for(int pending_index : pending_list) {
      auto pending_status = db_set->ticket_status_db.list(pending_index)[0];
      int max_seat_num = Train::k_max_seat_num + 1;
      for(int i = pending_status.order_s; i < pending_status.order_t; ++i)
        if(max_seat_num > train.unsold_seat_nums[date_exact][i])
          max_seat_num = train.unsold_seat_nums[date_exact][i];
      if(max_seat_num < pending_status.amount) break;

      db_set->pending_list_db.erase({train_id, date_exact}, pending_index);
      db_set->ticket_status_db.erase(pending_index, pending_status);
      for(int i = pending_status.order_s; i < pending_status.order_t; ++i)
        train.unsold_seat_nums[date_exact][i] -= pending_status.amount;
      pending_status.status = ticket_status_t::status_t::success;
      db_set->ticket_status_db.insert(pending_index, pending_status);
    }
    db_set->train_db.insert(train_index, train);
    return {"0", Signal::sig_normal};
  } else return {"-1", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::clean(const arglist_t &args) {
  // {}
  _db_set.renew();
  _user_mgr.active_users.clear();
  return {"0", Signal::sig_normal};
}

BackendSystem::ReturnInfo BackendSystem::exit(const arglist_t &args) {
  // {}
  return {"bye", Signal::sig_exit};
}




} // namespace TicketSystem