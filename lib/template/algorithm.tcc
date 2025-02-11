#ifndef INSOMNIA_ALGORITHM_TCC
#define INSOMNIA_ALGORITHM_TCC

#include "algorithm.h"

namespace Insomnia {
template<class RAIt, class Compare>
RAIt partition(RAIt begin, RAIt end, Compare comp) {
  auto pivot = *(end - 1);
  auto i = begin;
  for(auto j = begin; j != end - 1; ++j)
    if(comp(*j, pivot)) {
      std::iter_swap(i, j);
      ++i;
    }
  std::iter_swap(i, end - 1);
  return i;
}

template<class RAIt, class Compare>
void sort(RAIt begin, RAIt end, const Compare &comp) {
  if(begin == end) return;
  auto pivot_it = Insomnia::partition(begin, end, comp);
  Insomnia::sort(begin, pivot_it, comp);
  Insomnia::sort(pivot_it + 1, end, comp);
}

}

#endif // INSOMNIA_ALGORITHM_TCC