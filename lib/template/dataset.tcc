#ifndef INSOMNIA_DATASET_TCC
#define INSOMNIA_DATASET_TCC

namespace Insomnia {

template <class Tp, class Compare>
dataset<Tp, Compare>::~dataset() {
  close();
}

template <class Tp, class Compare>
void dataset<Tp, Compare>::renew(const std::string &filename) {
  _db.renew(filename);
}

template <class Tp, class Compare>
void dataset<Tp, Compare>::renew() {
  _db.renew();
}

template <class Tp, class Compare>
void dataset<Tp, Compare>::open(const std::string &filename) {
  _db.open(filename);
}

template <class Tp, class Compare>
void dataset<Tp, Compare>::close() {
  _db.close();
}

template <class Tp, class Compare>
bool dataset<Tp, Compare>::insert(const Tp &value) {
  return _db.insert(placeholder, value);
}

template <class Tp, class Compare>
bool dataset<Tp, Compare>::erase(const Tp &value) {
  return _db.erase(placeholder, value);
}

template <class Tp, class Compare>
vector<Tp> dataset<Tp, Compare>::list() {
  return _db.list(placeholder);
}

template <class Tp, class Compare>
bool dataset<Tp, Compare>::is_open() const {
  return _db.is_open();
}

template <class Tp, class Compare>
size_t dataset<Tp, Compare>::occupancy_number() const {
  return _db.occupancy_number();
}

template <class Tp, class Compare>
double dataset<Tp, Compare>::occupancy_rate() const {
  return _db.occupancy_rate();
}

template <class Tp, class Compare>
bool dataset<Tp, Compare>::empty() const {
  return _db.empty();
}

} // namespace Insomnia

#endif // INSOMNIA_DATASET_TCC