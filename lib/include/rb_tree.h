#ifndef INSOMNIA_RB_TREE_H
#define INSOMNIA_RB_TREE_H

#include "pair.h"

namespace insomnia {

template<class Key, class Value, class KeyofValue, class KeyCompare = std::less<Key>>
class rb_tree {
public:
  using value_type = pair<const Key, Value>;
  using size_type = size_t;

private:
  enum class node_color {Red, Black};
  struct node_base {
    node_base *left, *right, *parent;
  };
  struct tree_node: node_base {
    value_type kv;
    node_color color;

    tree_node* get_prev() const;
    tree_node* get_next() const;
    tree_node* left_rotate();
    tree_node* right_rotate();
  };
  class iterator_base {
    const rb_tree<Key, Value, KeyofValue, KeyCompare> *_container;
    node_base *_node;
    void increase();
    void decrease();
  };

  node_base *_header;
  size_type _size;
  KeyofValue _get_key_of_value;
  KeyCompare _key_comparer;

  void insertion_maintain();
  void erasure_maintain();

public:
  class const_iterator;
  class iterator: iterator_base {
    iterator();
    iterator(const rb_tree<Key, Value, KeyofValue, KeyCompare> *container, node_base *node);
    iterator& operator++();
    iterator operator++(int);
    iterator& operator--();
    iterator operator--(int);
    Value& operator*() const;
    Value* operator->() const;
    bool operator==(const iterator &) const;
    bool operator!=(const iterator &) const;
    bool operator==(const const_iterator &) const;
    bool operator!=(const const_iterator &) const;
  };
  class const_iterator: iterator_base {
    const_iterator();
    const_iterator(const rb_tree<Key, Value, KeyofValue, KeyCompare> *container, node_base *node);
    const_iterator(const iterator &);
    const_iterator(iterator &&);
    const_iterator& operator++();
    const_iterator operator++(int);
    const_iterator& operator--();
    const_iterator operator--(int);
    const Value& operator*() const;
    const Value* operator->() const;
    bool operator==(const const_iterator &) const;
    bool operator!=(const const_iterator &) const;
    bool operator==(const iterator &) const;
    bool operator!=(const iterator &) const;
  };

  rb_tree();
  rb_tree(const rb_tree &);
  rb_tree(rb_tree &&);
  ~rb_tree();

  rb_tree& operator=(const rb_tree &);
  rb_tree& operator=(rb_tree &&);

  iterator begin();
  iterator end();
  const_iterator cbegin() const;
  const_iterator cend() const;

  pair<iterator, bool> insert_unique(const value_type &kv); // terminate if key already exists.
  pair<iterator, bool> insert_equal(const value_type &kv); //
  iterator lower_bound(const Key &key);
  const_iterator lower_bound(const Key &key) const;
  iterator upper_bound(const Key &key);
  const_iterator upper_bound(const Key &key) const;
  void erase(const iterator &it);

  size_t count() const;
  size_t size() const;
  bool empty() const;

  void clear();
};

}

#include "rb_tree.tcc"

#endif // INSOMNIA_RB_TREE_H