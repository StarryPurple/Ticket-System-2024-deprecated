#ifndef INSOMNIA_DATASET_H
#define INSOMNIA_DATASET_H

#include "database.h"

namespace insomnia {

template<class Tp, class Compare = std::less<Tp>>
class dataset {
private:
  using placeholder_t = int;
  bplustree<placeholder_t, Tp, std::less<placeholder_t>, Compare> _db;
  static constexpr placeholder_t placeholder = 0;
public:
  dataset() = default;
  ~dataset();
  dataset(const dataset &) = delete;
  dataset(dataset &&) = delete;
  dataset& operator=(const dataset &) = delete;
  dataset& operator=(dataset &&) = delete;

  // is_open() == false required.
  void renew(const std::string &filename);
  // renew current file. is_open() == true required.
  void renew();
  // open a file.
  // can't open a file not existing.
  // Remember to use renew() function to create one.
  void open(const std::string &filename);
  void close();
  // returns whether insertion succeeds.
  // possible failure: value already exists.
  bool insert(const Tp &value);
  bool erase(const Tp &value);
  // returns all values in the order of ValueCompare.
  vector<Tp> list();

  bool is_open() const;
  size_t occupancy_number() const;
  double occupancy_rate() const;
  bool empty() const;
};

}

#include "dataset.tcc"

#endif // INSOMNIA_DATASET_H