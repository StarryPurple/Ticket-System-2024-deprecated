#ifndef INSOMNIA_ALGORITHM_TCC
#define INSOMNIA_ALGORITHM_TCC

#include "algorithm.h"

namespace insomnia {
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
  auto pivot_it = insomnia::partition(begin, end, comp);
  insomnia::sort(begin, pivot_it, comp);
  insomnia::sort(pivot_it + 1, end, comp);
}

}

#endif // INSOMNIA_ALGORITHM_TCC