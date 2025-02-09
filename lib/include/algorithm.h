#ifndef INSOMNIA_ALGORITHM_H
#define INSOMNIA_ALGORITHM_H

#include "iterator"
#include "functional"

namespace Insomnia {

// RAIt for random access iterator.
// only a quicksort.
template<
  class RAIt,
  class Compare = std::less<typename std::iterator_traits<RAIt>::value_type>>
void sort(RAIt begin, RAIt end, const Compare &comp);

// out_of_range not handled.
int stoi(const std::string &str);

}

#include "algorithm.tcc"

#endif // INSOMNIA_ALGORITHM_H