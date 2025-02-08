#ifndef TICKET_SYSTEM_BASIC_TYPES_H
#define TICKET_SYSTEM_BASIC_TYPES_H

#include "config.h"
#include "ticket_system_exceptions.h"

namespace TicketSystem {

// hour and minute.
class Time_hm {
public:
  using time_dur_t = int; // minutes. maybe better called time_diff_t.
private:
  int _hour, _minute;
public:
  Time_hm(int hour, int minute);
  time_dur_t operator-(const Time_hm &); // difference in [-3599, 3599]
  std::strong_ordering operator<=>(const Time_hm &) const;
  std::string str() const;
};

// month and date.
// no leap year considered. February always has 28 days.
class Date_md {
public:
  using date_dur_t = int; // days
private:
  static constexpr int k_month_days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int _month, _day;
public:
  Date_md(int month, int day);
  date_dur_t operator-(const Date_md &); // difference in [-364, 364]
  std::strong_ordering operator<=>(const Date_md &) const;
  std::string str() const;
};

class Date {
public:
  using time_dur_t = Time_hm::time_dur_t;
  using date_dur_t = Date_md::date_dur_t;
private:
  Time_hm _time_hm;
  Date_md _date_md;
public:
  Date(const Time_hm &time_hm, const Date_md &date_md);
  Date add_minutes(const time_dur_t &) const;
  void self_add_minutes(const time_dur_t &);
  Date subtract_minutes(const time_dur_t &) const;
  void self_subtract_minutes(const time_dur_t &);
  Date add_days(const date_dur_t &) const;
  void self_add_days(const date_dur_t &);
  Date subtract_days(const date_dur_t &) const;
  void self_subtract_days(const date_dur_t &);
  ism::pair<date_dur_t, time_dur_t> operator-(const Date &); // result members are both positive or both negative.
  std::strong_ordering operator<=>(const Date &) const;
  std::string str() const;
};

class User {
public:
  using username_t = ism::ascii_string<20>;
  using password_t = ism::ascii_string<30>;
  using real_name_t = ism::utf8_string<5>;
  using mail_address_t = ism::ascii_string<30>;
  using privilege_t = int;
private:
  username_t _username;
  password_t _password;
  real_name_t _real_name;
  mail_address_t _mail_address;
  privilege_t _privilege;
public:
  User(
    const username_t &username, const password_t &password, const real_name_t &real_name,
    const mail_address_t &mail_address, const privilege_t &privilege);
};

class Train {
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
private:
  train_id_t _train_id;
  station_num_t _station_num;
  station_name_t _station_names[k_max_station_num];
  seat_num_t _seat_num;
  price_t _prices[k_max_station_num - 1];
  time_hm_t _start_time;
  time_dur_t _travel_times[k_max_station_num - 1];
  time_dur_t _stopover_times[k_max_station_num - 1];
  date_md_t _sale_date_first, _sale_date_last;
  train_type_t _train_type;
public:
  Train(
    const train_id_t &train_id, const station_num_t &station_num, const seat_num_t &seat_num,
    const ism::vector<station_name_t> &stations, const ism::vector<price_t> &prices,
    const time_hm_t &start_time, const ism::vector<time_dur_t> &travel_times,
    const ism::vector<time_dur_t> &stopover_times,
    const date_md_t &sale_date_first, const date_md_t &sale_date_last,
    const train_type_t &train_type);
};

}



#endif // TICKET_SYSTEM_BASIC_TYPES_H