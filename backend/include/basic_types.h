#ifndef TICKET_SYSTEM_BASIC_TYPES_H
#define TICKET_SYSTEM_BASIC_TYPES_H

#include "config.h"
#include "ticket_system_exceptions.h"

namespace TicketSystem {

// hour and minute.
struct Time_hm {
  using time_dur_t = int; // minutes. maybe better called time_diff_t.

  int hour = -1, minute = -1;

  Time_hm() = default;
  Time_hm(int _hour, int _minute);
  Time_hm(const std::string &str);
  struct time_result_t;
  time_result_t add_minutes(const time_dur_t &) const;
  time_result_t subtract_minutes(const time_dur_t &) const;
  time_dur_t operator-(const Time_hm &) const;
  bool operator==(const Time_hm &) const;
  bool operator!=(const Time_hm &) const;
  bool operator<(const Time_hm &) const;
  bool operator>(const Time_hm &) const;
  bool operator<=(const Time_hm &) const;
  bool operator>=(const Time_hm &) const;
  std::string str() const;
};

struct Time_hm::time_result_t {
  Time_hm time;
  int days_diff;
};

// month and date.
// no leap year considered. February always has 28 days.
// upd: 06-01 to 08-31 only.
struct Date_md {
  using date_dur_t = int; // days
  static constexpr int k_month_days[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  static constexpr int k_month_days_accum[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  int month = -1, day = -1;

  Date_md() = default;
  Date_md(int _month, int _day);
  Date_md(const std::string &str);
  Date_md add_days(const date_dur_t &) const;
  Date_md subtract_days(const date_dur_t &) const;
  date_dur_t operator-(const Date_md &) const;
  bool operator==(const Date_md &) const;
  bool operator!=(const Date_md &) const;
  bool operator<(const Date_md &) const;
  bool operator>(const Date_md &) const;
  bool operator<=(const Date_md &) const;
  bool operator>=(const Date_md &) const;
  std::string str() const;
  date_dur_t days_since_epoch() const;
};

struct Date {
  using time_dur_t = Time_hm::time_dur_t;
  using date_dur_t = Date_md::date_dur_t;

  Time_hm time_hm;
  Date_md date_md;

  Date() = default;
  Date(const Date_md &_date_md, const Time_hm &_time_hm);
  Date add_minutes(const time_dur_t &) const;
  Date subtract_minutes(const time_dur_t &) const;
  Date add_days(const date_dur_t &) const;
  Date subtract_days(const date_dur_t &) const;
  time_dur_t get_diff_minutes(const Date &) const;
  // ism::pair<date_dur_t, time_dur_t> operator-(const Date &); // result members are both positive or both negative.
  bool operator==(const Date &) const;
  bool operator!=(const Date &) const;
  bool operator<(const Date &) const;
  bool operator>(const Date &) const;
  bool operator<=(const Date &) const;
  bool operator>=(const Date &) const;
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
  mail_addr_t mail_addr;
  privilege_t privilege = -1;

  User() = default;
  User(
    const username_t &_username, const password_t &_password, const real_name_t &_real_name,
    const mail_addr_t &_mail_addr, const privilege_t &_privilege);
  std::string str() const;
  bool operator==(const User &) const;
  bool operator!=(const User &) const;
  bool operator<(const User &) const;
  bool operator>(const User &) const;
  bool operator<=(const User &) const;
  bool operator>=(const User &) const;

};

struct Train {
public:
  using train_id_t = ism::ascii_string<20>;
  using station_num_t = int;
  using station_name_t = ism::utf8_string<10>;
  using seat_num_t = int;
  using price_t = int;
  // using total_price_t = long long;
  using time_hm_t = Time_hm;
  using time_dur_t = time_hm_t::time_dur_t;
  using date_md_t = Date_md;
  using date_dur_t = date_md_t::date_dur_t;
  using train_type_t = char;
  using station_order_t = int;
  constexpr static int k_max_station_num = 100;
  constexpr static seat_num_t k_max_seat_num = 100000;
  constexpr static time_dur_t k_max_total_travel_time = 43200;
  constexpr static price_t k_max_price = 10000;
  constexpr static int k_max_days = 92;
  constexpr static time_dur_t k_time_inf = 2 * k_max_total_travel_time + 1;
  constexpr static price_t k_cost_inf = (k_max_station_num - 1) * k_max_price + 1;
  train_id_t train_id;
  station_num_t station_num {};
  station_name_t station_names[k_max_station_num];
  seat_num_t passenger_capacity {};
  // from station i to station i + 1. size = st_num - 1.
  seat_num_t unsold_seat_nums[k_max_days][k_max_station_num] {};
  // from station 0 to station i. a_p[0] = 0.
  price_t accumulated_prices[k_max_station_num] {};
  time_hm_t start_time;
  // leaving station i at start_time + l_t_d[i]. size = st_num - 1.
  time_dur_t leaving_time_diff[k_max_station_num] {};
  // reaching station i at start_time + a_t_d[i]. a_t_d[0] shouldn't be used.
  time_dur_t arriving_time_diff[k_max_station_num] {};
  date_md_t sale_date_first, sale_date_last;
  train_type_t train_type {};
  bool has_released; // initially false.

  Train() = default;
  Train(
    const train_id_t &_train_id, const station_num_t &_station_num, const seat_num_t &_passenger_capacity,
    const ism::vector<station_name_t> &_station_names, const ism::vector<price_t> &_prices,
    const time_hm_t &_start_time, const ism::vector<time_dur_t> &_travel_times,
    const ism::vector<time_dur_t> &_stopover_times,
    const date_md_t &_sale_date_first, const date_md_t &_sale_date_last,
    const train_type_t &_train_type);
  bool operator==(const Train &) const;
  bool operator!=(const Train &) const;
  bool operator<(const Train &) const;
  bool operator>(const Train &) const;
  bool operator<=(const Train &) const;
  bool operator>=(const Train &) const;
  std::string station_profile(const Date_md &start_date, int route_index) const;
};

struct ticket_status_t {
  using ticket_request_index_t = int;
  enum class status_t {success, pending, refunded};
  status_t status;
  ticket_request_index_t request_id;
  Train::train_id_t train_id;
  Date_md start_date_md;
  Train::station_order_t order_s, order_t;
  Train::station_name_t start_station, terminal_station;
  Date date_s, date_t;
  Train::price_t price;
  Train::seat_num_t amount;
  std::string str() const;
  bool operator==(const ticket_status_t &) const;
  bool operator!=(const ticket_status_t &) const;
  bool operator<(const ticket_status_t &) const;
  bool operator>(const ticket_status_t &) const;
  bool operator<=(const ticket_status_t &) const;
  bool operator>=(const ticket_status_t &) const;
};
}



#endif // TICKET_SYSTEM_BASIC_TYPES_H