#ifndef INSOMNIA_PAIR_H
#define INSOMNIA_PAIR_H

#include <iostream>

namespace Insomnia {

/** @brief A pair container which behaves like std::pair.
 *  Nothing special.
 */
template<class T1, class T2>
using pair = std::pair<T1, T2>;

}

#include "pair.tcc"


#endif // INSOMNIA_PAIR_H