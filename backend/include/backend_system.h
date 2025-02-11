#ifndef TICKET_SYSTEM_BACKEND_SYSTEM_H
#define TICKET_SYSTEM_BACKEND_SYSTEM_H

#include "basic_types.h"

#include <functional>

namespace TicketSystem {


class BackendSystem {
public:
  enum class Signal {sig_normal, sig_exit};
private:
  struct ReturnInfo {
    std::string info;
    Signal signal = Signal::sig_normal;
    ReturnInfo() = default;
    ReturnInfo(const std::string &_info, const Signal &_signal);
  };
  struct CommandPair {
    using key_t = char;
    using arg_t = std::string;
    key_t key;
    arg_t arg;
    CommandPair(const key_t &_key, const arg_t &_arg);
  };
  using raw_arglist_t = ism::vector<CommandPair>;
  using arglist_t = ism::map<CommandPair::key_t, CommandPair::arg_t>;
  struct Command {
    using timestamp_t = int;
    using cmd_name_t = std::string;
    timestamp_t timestamp;
    cmd_name_t cmd_name;
    raw_arglist_t arglist;
  };

  struct DatabaseSet {
  private:
    bool is_all_open = false;
  public:
    using user_index_t = int;
    ism::database<user_index_t, User> user_db;
    ism::database<User::username_t, user_index_t> user_index_db;
    ism::database<user_index_t, ticket_status_t::ticket_request_index_t> user_ticket_status_index_db;
    using ticket_request_index_t = ticket_status_t::ticket_request_index_t;
    ism::database<ticket_status_t::ticket_request_index_t, ticket_status_t> ticket_status_db;
    ism::database<ism::pair<Train::train_id_t, Train::date_dur_t>, ticket_status_t::ticket_request_index_t> pending_list_db;
    using train_index_t = int;
    ism::database<train_index_t, Train> train_db;
    ism::database<Train::train_id_t, train_index_t> train_index_db;
    struct station_info_t {
      using station_order_t = Train::station_order_t; // the order number that this station is on the route of the train
      Train::train_id_t train_id;
      station_order_t order;
      station_info_t() = default;
      station_info_t(const Train::train_id_t &_train_id, const station_order_t &_order);
      bool operator==(const station_info_t &) const;
      bool operator!=(const station_info_t &) const;
      bool operator<(const station_info_t &) const;
      bool operator>(const station_info_t &) const;
      bool operator<=(const station_info_t &) const;
      bool operator>=(const station_info_t &) const;
    };
    ism::database<Train::station_name_t, station_info_t> station_db;
    struct info_t {
      user_index_t user_index_tot;
      train_index_t train_index_tot;
      ticket_status_t::ticket_request_index_t ticket_request_tot;
    };
    info_t info;
    ism::info_recorder<info_t> info_rec;

    ~DatabaseSet();
    void renew(const std::string &working_dir);
    void renew();
    bool open(const std::string &working_dir);
    bool is_open() const;
    void close();
    ReturnInfo clean(const arglist_t &args);
  };

  // for operations which needs the active-user list.
  struct UserManager {
    DatabaseSet *db_set = nullptr;
    ism::set<User::username_t> active_users;

    ReturnInfo add_user(const arglist_t &args);
    ReturnInfo login(const arglist_t &args);
    ReturnInfo logout(const arglist_t &args);
    ReturnInfo query_profile(const arglist_t &args);
    ReturnInfo modify_profile(const arglist_t &args);

    ReturnInfo buy_ticket(const arglist_t &args);
    ReturnInfo query_order(const arglist_t &args);
    ReturnInfo refund_ticket(const arglist_t &args);

    void startup(DatabaseSet &_db_set);
    void shutdown();
  };

  // no user privilege required... and associated with train profiles.
  struct TrainManager {
    DatabaseSet *db_set = nullptr;

    ReturnInfo add_train(const arglist_t &args);
    ReturnInfo delete_train(const arglist_t &args);
    ReturnInfo release_train(const arglist_t &args);
    ReturnInfo query_train(const arglist_t &args);
    ReturnInfo query_ticket(const arglist_t &args);
    ReturnInfo query_transfer(const arglist_t &args);

    void startup(DatabaseSet &_db_set);
    void shutdown();
  };

  DatabaseSet _db_set {};
  UserManager _user_mgr {};
  TrainManager _train_mgr {};

  static ReturnInfo exit(const arglist_t &args);

  static constexpr int k_opr_cnt = 16;
  std::function<ReturnInfo(const arglist_t &)> func_list[k_opr_cnt];
  static const std::string func_name_list[k_opr_cnt];
  // the default arg is "", for no param passed.
  static const arglist_t func_default_arglist[k_opr_cnt];
  static Command read_command(const std::string &command);
  // args: {'z', "par1"}, {'y', "par2"}, (no 'x'), ...
  // (initial) sorted_args: {'x', "default0"}, {'y', ""}, {'z', "default_z"}, ...
  // target: sorted_args: {'x', "default0"}, {'y', "par2"}, {'z', "par1"}, ...
  static void sort_arglist(const raw_arglist_t &arglist, arglist_t &sorted_arglist);

  Signal run_command(const Command &command) const;
public:
  BackendSystem();
  bool startup(const std::string &data_dir);
  void shutdown();
  bool is_open() const;
  Signal run_command(const std::string &command) const;
};

} // namespace TicketSystem

#endif // TICKET_SYSTEM_BACKEND_SYSTEM_H