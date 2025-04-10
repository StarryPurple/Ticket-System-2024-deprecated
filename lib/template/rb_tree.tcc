#ifndef INSOMNIA_RB_TREE_TCC
#define INSOMNIA_RB_TREE_TCC

#include "rb_tree.h"

#include <functional>

namespace insomnia {

template <class Key, class Value, class KeyofValue, class KeyCompare>
typename rb_tree<Key, Value, KeyofValue, KeyCompare>::tree_node*
  rb_tree<Key, Value, KeyofValue, KeyCompare>::tree_node::get_prev() const {
}


}

#endif // INSOMNIA_RB_TREE_TCC