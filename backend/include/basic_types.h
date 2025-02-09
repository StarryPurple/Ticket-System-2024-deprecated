#ifndef TICKET_SYSTEM_BASIC_TYPES_H
#define TICKET_SYSTEM_BASIC_TYPES_H

#include "config.h"
#include "ticket_system_exceptions.h"

namespace TicketSystem {

// hour and minute.
struct  Time_hm {
  using time_dur_t = int; // minutes. maybe better called time_diff_t.

  int hour = -1, minute = -1;

  Time_hm() = default;
  Time_hm(int hour, int minute);
  ism::pair<Time_hm, int> add_minutes(const time_dur_t &) const;
  int self_add_minutes(const time_dur_t &);
  ism::pair<Time_hm, int> subtract_minutes(const time_dur_t &) const;
  int self_subtract_minutes(const time_dur_t &);
  std::strong_ordering operator<=>(const Time_hm &) const;
  std::string str() const;
};

// month and date.
// no leap year considered. February always has 28 days.
struct Date_md {
  using date_dur_t = int; // days
  static constexpr int k_month_days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int month = -1, day = -1;

  Date_md() = default;
  Date_md(int month, int day);
  ism::pair<Date_md, int> add_days(const date_dur_t &) const;
  int self_add_days(const date_dur_t &);
  ism::pair<Date_md, int> subtract_minutes(const date_dur_t &) const;
  int self_subtract_minutes(const date_dur_t &) const;
  std::strong_ordering operator<=>(const Date_md &) const;
  std::string str() const;
};

struct Date {
  using time_dur_t = Time_hm::time_dur_t;
  using date_dur_t = Date_md::date_dur_t;

  Time_hm time_hm;
  Date_md date_md;

  Date() = default;
  Date(const Time_hm &time_hm, const Date_md &date_md);
  ism::pair<Date, int> add_minutes(const time_dur_t &) const;
  int self_add_minutes(const time_dur_t &);
  ism::pair<Date, int> subtract_minutes(const time_dur_t &) const;
  int self_subtract_minutes(const time_dur_t &);
  ism::pair<Date, int> add_days(const date_dur_t &) const;
  int self_add_days(const date_dur_t &);
  ism::pair<Date, int> subtract_days(const date_dur_t &) const;
  int self_subtract_days(const date_dur_t &);
  // ism::pair<date_dur_t, time_dur_t> operator-(const Date &); // result members are both positive or both negative.
  std::strong_ordering operator<=>(const Date &) const;
  std::string str() const;
};

struct User {
public:
  using username_t = ism::ascii_string<20>;
  using password_t = ism::ascii_string<30>;
  using real_name_t = ism::utf8_string<5>;
  using mail_addr_t = ism::ascii_string<30>;
  using privilege_t = int;

  username_t username;
  password_t password;
  real_name_t real_name;
  mail_addr_t mail_address;
  privilege_t privilege = -1;

  User() = default;
  User(
    const username_t &_username, const password_t &_password, const real_name_t &_real_name,
    const mail_addr_t &_mail_address, const privilege_t &_privilege);
};

struct Train {
public:
  using train_id_t = ism::ascii_string<20>;
  using station_num_t = int;
  using station_name_t = ism::utf8_string<10>;
  using seat_num_t = int;
  using price_t = int;
  using time_hm_t = Time_hm;
  using time_dur_t = time_hm_t::time_dur_t;
  using date_md_t = Date_md;
  using date_dur_t = date_md_t::date_dur_t;
  using train_type_t = char;
  constexpr static int k_max_station_num = 100;

  train_id_t train_id;
  station_num_t station_num {};
  station_name_t station_names[k_max_station_num];
  seat_num_t passenger_capacity {};
  seat_num_t unsold_seat_nums[k_max_station_num - 1] {};
  price_t prices[k_max_station_num - 1] {};
  time_hm_t start_time;
  time_dur_t travel_times[k_max_station_num - 1] {};
  time_dur_t stopover_times[k_max_station_num - 1] {};
  date_md_t sale_date_first, sale_date_last;
  train_type_t train_type {};

  Train() = default;
  Train(
    const train_id_t &_train_id, const station_num_t &_station_num, const seat_num_t &_passenger_capacity,
    const ism::vector<station_name_t> &_stations, const ism::vector<price_t> &_prices,
    const time_hm_t &_start_time, const ism::vector<time_dur_t> &_travel_times,
    const ism::vector<time_dur_t> &_stopover_times,
    const date_md_t &_sale_date_first, const date_md_t &_sale_date_last,
    const train_type_t &_train_type);
};

struct ticket_status_t {
  using ticket_request_t = int;
  enum class status {failure, success, pending};
  bool is_pending_accepted;
  ticket_request_t request_id;
  std::strong_ordering operator<=>(const ticket_status_t &) const;
};
}



#endif // TICKET_SYSTEM_BASIC_TYPES_H