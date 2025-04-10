#ifndef INSOMNIA_MAP_H
#define INSOMNIA_MAP_H

#include "rb_tree.h"

#include <map>

namespace insomnia {

template<class Key, class Value, class KeyCompare = std::less<Key>>
using map = std::map<Key, Value, KeyCompare>;

}




#include "map.tcc"

#endif // INSOMNIA_MAP_H