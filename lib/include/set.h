#ifndef INSOMNIA_SET_H
#define INSOMNIA_SET_H

#include "rb_tree.h"

namespace Insomnia {

template<class Tp, class Compare = std::less<Tp>>
class set {

};

} // namespace Insomnia


#include "set.tcc"

#endif // INSOMNIA_SET_H