#ifndef INSOMNIA_SET_H
#define INSOMNIA_SET_H

#include "rb_tree.h"

#include <set>

namespace insomnia {

template<class Tp, class Compare = std::less<Tp>>
using set = std::set<Tp, Compare>;

}


#include "set.tcc"

#endif // INSOMNIA_SET_H