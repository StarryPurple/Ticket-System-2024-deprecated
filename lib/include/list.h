#ifndef INSOMNIA_LIST_H
#define INSOMNIA_LIST_H

#include <list>

namespace Insomnia {

using list = std::list;

/*
template<class Tp>
class list {
  struct node_base {
    node_base *prev = nullptr, *next = nullptr;
    node_base() = default;
  };
  struct node: node_base {
    Tp v;
    node(const Tp &);
  };

  node_base _end_node, _rend_node;
  size_t _size;

public:
  class iterator {
    node_base *node = nullptr;
    iterator() = default;
    iterator(const iterator &) = default;
    iterator(iterator &&) = default;
  };

  list();
  list(const list &);
  list(list &&) = default;
  list& operator=(const list &);
  list& operator=(list &&) = default;
  iterator begin();
  iterator end();
  iterator find(const Tp &);
  void insert(iterator);
  void erase(iterator);
  void clear();
  size_t size() const;
  bool empty() const;
};
*/
}

#include "list.tcc"

#endif // INSOMNIA_LIST_H