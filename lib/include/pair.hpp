#ifndef INSOMNIA_PAIR_HPP
#define INSOMNIA_PAIR_HPP

namespace Insomnia {

/** @brief A pair container which behaves like std::pair.
 *  Nothing special.
 */
template<class T1, class T2>
struct pair {
  T1 first;
  T2 second;

  constexpr pair() = default;
  constexpr pair(const pair &other) = default;
  constexpr pair(pair &&other) = default;

  template<class U1 = T1, class U2 = T2>
  constexpr pair(U1 &&x, U2 &&y)
      : first(std::forward<U1>(x)), second(std::forward<U2>(y)) {}

  template<class U1, class U2>
  constexpr pair(const pair<U1, U2> &other)
      : first(other.first), second(other.second) {}

  template<class U1, class U2>
  constexpr pair(pair<U1, U2> &&other)
      : first(std::move(other.first))
      ,second(std::move(other.second)) {}
};



}

#endif // INSOMNIA_PAIR_H