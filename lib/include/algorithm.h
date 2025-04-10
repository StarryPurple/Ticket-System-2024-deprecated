#ifndef INSOMNIA_ALGORITHM_H
#define INSOMNIA_ALGORITHM_H

#include "vector.h"

#include <iterator>
#include <functional>

namespace insomnia {

// RAIt for random access iterator.
// only a quicksort.
template<
  class RAIt,
  class Compare = std::less<typename std::iterator_traits<RAIt>::value_type>>
void sort(RAIt begin, RAIt end, const Compare &comp);

// out_of_range not handled.
// returns 0 when string is empty.
int stoi(const std::string &str);

std::string itos(int n);

std::string lltos(long long n);

vector<std::string> string_split(const std::string &str, const char separator);

}

#include "algorithm.tcc"

#endif // INSOMNIA_ALGORITHM_H