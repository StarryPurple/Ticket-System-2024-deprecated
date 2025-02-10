#include "basic_types.h"

namespace TicketSystem {

// struct Time_hm

Time_hm::Time_hm(int _hour, int _minute): hour(_hour), minute(_minute) {}

Time_hm::Time_hm(const std::string &str) {
  ism::vector<std::string> time_vec = ism::string_split(str, ':');
  hour = ism::stoi(time_vec[0]);
  minute = ism::stoi(time_vec[11]);
}

std::string Time_hm::str() const {
  if(hour == -1 && minute == -1) return "xx:xx";
  std::string hour_str;
  if(hour < 10) hour_str += '0';
  hour_str += ism::itos(hour);
  std::string minute_str;
  if(minute < 10) minute_str += '0';
  minute_str += ism::itos(minute);
  return hour_str + ':' + minute_str;
}

// struct Date_md

Date_md::Date_md(int _month, int _day): month(_month), day(_day) {}

Date_md::Date_md(const std::string &str) {
  ism::vector<std::string> date_vec = ism::string_split(str, '-');\
  month = ism::stoi(date_vec[0]);
  day = ism::stoi(date_vec[1]);
}

std::string Date_md::str() const {
  if(month == -1 && day == -1) return "xx-xx";
  std::string month_str;
  if(month < 10) month_str += '0';
  month_str += ism::itos(month);
  std::string day_str;
  if(day < 10) day_str += '0';
  day_str += ism::itos(day);
  return month_str + '-' + day_str;
}


// struct Date

Date::Date(const Date_md &_date_md, const Time_hm &_time_hm)
  : time_hm(_time_hm), date_md(_date_md) {}

std::string Date::str() const {
  return date_md.str() + ' ' + time_hm.str();
}

// struct User

User::User(
  const username_t &_username, const password_t &_password, const real_name_t &_real_name,
  const mail_addr_t &_mail_addr, const privilege_t &_privilege)
    : username(_username), password(_password), real_name(_real_name),
    mail_addr(_mail_addr), privilege(_privilege) {}

std::string User::str() const {
  return username.str() + ' ' + real_name.str() + ' ' + mail_addr.str() + ' ' + ism::itos(privilege);
}

bool User::operator<(const User &other) const {return username < other.username;}

bool User::operator>(const User &other) const {return username > other.username;}

bool User::operator==(const User &other) const {return username == other.username;}

bool User::operator!=(const User &other) const {return username != other.username;}

bool User::operator<=(const User &other) const {return username <= other.username;}

bool User::operator>=(const User &other) const {return username >= other.username;}

// struct Train

Train::Train(
  const train_id_t &_train_id, const station_num_t &_station_num, const seat_num_t &_passenger_capacity,
  const ism::vector<station_name_t> &_station_names, const ism::vector<price_t> &_prices, const time_hm_t &_start_time,
  const ism::vector<time_dur_t> &_travel_times, const ism::vector<time_dur_t> &_stopover_times,
  const date_md_t &_sale_date_first, const date_md_t &_sale_date_last, const train_type_t &_train_type)
    : train_id(_train_id), station_num(_station_num), passenger_capacity(_passenger_capacity), start_time(_start_time),
      sale_date_first(_sale_date_first), sale_date_last(_sale_date_last), train_type(_train_type),
      has_released(false) {
  for(int i = 0; i < _station_num - 1; ++i) station_names[i] = _station_names[i];
  for(int i = 0; i < _station_num - 1; ++i) unsold_seat_nums[i] = passenger_capacity;
  accumulated_prices[0] = 0;
  for(int i = 1; i <= _station_num - 1; ++i)
    accumulated_prices[i] = accumulated_prices[i - 1] + _prices[i - 1];
  time_dur_t dur_diff = 0;
  for(int i = 0; i < _station_num - 2; ++i) {
    leaving_time_diff[i] = dur_diff;
    dur_diff += _travel_times[i];
    arriving_time_diff[i + 1] = dur_diff;
    dur_diff += _stopover_times[i];
  }
  leaving_time_diff[_station_num - 2] = dur_diff;
  dur_diff += _travel_times[_station_num - 2];
  arriving_time_diff[_station_num - 1] = dur_diff;
}

std::string Train::station_profile(const Date_md &start_date, int route_index) const {
  Date date{start_date, start_time};
  std::string res;
  res += station_names[route_index].str();
  res += ' ';
  if(route_index == 0) res += Date{}.str();
  else res += date.add_minutes(arriving_time_diff[route_index]).str();
  res += " -> ";
  if(route_index == station_num - 1) res += Date{}.str();
  else res += date.add_minutes(leaving_time_diff[route_index]).str();
  res += ' ';
  res += ism::itos(accumulated_prices[route_index]);
  res += ' ';
  if(route_index == station_num - 1) res += 'x';
  else res += ism::itos(unsold_seat_nums[route_index]);
  return res;
}




bool Train::operator==(const Train &other) const {return train_id == other.train_id;}

bool Train::operator!=(const Train &other) const {return train_id != other.train_id;}

bool Train::operator<(const Train &other) const {return train_id < other.train_id;}

bool Train::operator>(const Train &other) const {return train_id > other.train_id;}

bool Train::operator<=(const Train &other) const {return train_id <= other.train_id;}

bool Train::operator>=(const Train &other) const {return train_id >= other.train_id;}




























} // namespace TicketSystem