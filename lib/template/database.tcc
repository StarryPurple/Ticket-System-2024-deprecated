#ifndef INSOMNIA_DATABASE_TCC
#define INSOMNIA_DATABASE_TCC

#include <cassert>
#include <pstl/execution_defs.h>

#include "database.h"

namespace Insomnia {

template <class Key, class Value, class KeyCompare, class ValueCompare>
database<Key, Value, KeyCompare, ValueCompare>::kv_type::kv_type(
  const Key &the_key, const Value &the_value)
    : key(the_key), value(the_value) {}


template <class Key, class Value, class KeyCompare, class ValueCompare>
bool database<Key, Value, KeyCompare, ValueCompare>::kv_comparer(
  const kv_type &lhs, const kv_type &rhs) const {
  if(_key_comparer(lhs.key, rhs.key)) return true;
  if(_key_comparer(rhs.key, lhs.key)) return false;
  return _value_comparer(lhs.value, rhs.value);
}

template <class Key, class Value, class KeyCompare, class ValueCompare>
typename database<Key, Value, KeyCompare, ValueCompare>::kv_type&
  database<Key, Value, KeyCompare, ValueCompare>::Node::highkv() {
  return kv[size - 1];
}


template <class Key, class Value, class KeyCompare, class ValueCompare>
database<Key, Value, KeyCompare, ValueCompare>::NodeSelf::NodeSelf(
  const Nodeptr &the_ptr, const Node &the_node)
    : ptr(the_ptr), node(the_node) {}

template <class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::renew(const std::string &filename) {
  _fs.renew(filename);
}


template<class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::open(const std::string &filename) {
  if(is_open())
    throw FileSystemException("database is open");
  _fs.open(filename);
  if(!_fs.is_open()) return;
  _fs.read_info(_root);
}

template<class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::close() {
  if(!is_open()) return;
  _fs.write_info(_root);
  _fs.close();
}

template<class Key, class Value, class KeyCompare, class ValueCompare>
bool database<Key, Value, KeyCompare, ValueCompare>::is_open() const {
  return _fs.is_open();
}

template<class Key, class Value, class KeyCompare, class ValueCompare>
database<Key, Value, KeyCompare, ValueCompare>::~database() {
  close();
}


template <class Key, class Value, class KeyCompare, class ValueCompare>
size_t database<Key, Value, KeyCompare, ValueCompare>::occupancy_number() const {
  return _fs.occupancy_number();
}


template<class Key, class Value, class KeyCompare, class ValueCompare>
double database<Key, Value, KeyCompare, ValueCompare>::occupancy_rate() const {
  return _fs.occupancy_rate();
}

template <class Key, class Value, class KeyCompare, class ValueCompare>
bool database<Key, Value, KeyCompare, ValueCompare>::is_key_equal(
  const Key &lhs, const Key &rhs) const {
  // return !(_key_comparer(lhs, rhs) || _key_comparer(lhs, rhs));
  return lhs == rhs;
}


template <class Key, class Value, class KeyCompare, class ValueCompare>
bool database<Key, Value, KeyCompare, ValueCompare>::is_kv_equal(
  const kv_type &lhs, const kv_type &rhs) const {
  // return !(_kv_comparer(lhs, rhs) || _kv_comparer(rhs, lhs));
  return lhs.key == rhs.key && lhs.value == rhs.value;
}


template <class Key, class Value, class KeyCompare, class ValueCompare>
typename database<Key, Value, KeyCompare, ValueCompare>::NodeSelf
  database<Key, Value, KeyCompare, ValueCompare>::navigate_to_leaf(const kv_type &kv) {
  // remember to maintain parent ptr.
  Nodeptr cur_ptr = _root;
  Node cur_node;
  _fs.read(cur_ptr, cur_node);
  while(true) {
    if(kv_comparer(cur_node.highkv(), kv)) {
      if(cur_node.right == nullptr) {
        if(cur_node.is_leaf) return NodeSelf{cur_ptr, cur_node};
        Nodeptr next_ptr = cur_node.child[cur_node.size - 1];
        _fs.read(next_ptr, cur_node);
        if(cur_node.parent != cur_ptr) {
          cur_node.parent = cur_ptr;
          _fs.write(next_ptr, cur_node);
        }
        cur_ptr = next_ptr;
      } else {
        cur_ptr = cur_node.right;
        _fs.read(cur_ptr, cur_node);
      }
      continue;
    }
    if(cur_node.is_leaf) return NodeSelf{cur_ptr, cur_node};
    int l = 0, r = cur_node.size - 1;
    while(l < r) {
      int mid = (l + r) >> 1;
      if(kv_comparer(cur_node.kv[mid], kv)) l = mid + 1;
      else r = mid;
    }
    Nodeptr next_ptr = cur_node.child[l];
    _fs.read(next_ptr, cur_node);
    if(cur_node.parent != cur_ptr) {
      cur_node.parent = cur_ptr;
      _fs.write(next_ptr, cur_node);
    }
    cur_ptr = next_ptr;
  }
}

template <class Key, class Value, class KeyCompare, class ValueCompare>
typename database<Key, Value, KeyCompare, ValueCompare>::NodeSelf
database<Key, Value, KeyCompare, ValueCompare>::navigate_to_leaf(const Key &key) {
  // remember to maintain parent ptr.
  // assert(_root != nullptr);
  Nodeptr cur_ptr = _root;
  Node cur_node;
  _fs.read(cur_ptr, cur_node);
  while(true) {
    if(_key_comparer(cur_node.highkv().key, key)) {
      if(cur_node.right == nullptr) {
        if(cur_node.is_leaf) return NodeSelf{cur_ptr, cur_node};
        Nodeptr next_ptr = cur_node.child[cur_node.size - 1];
        _fs.read(next_ptr, cur_node);
        if(cur_node.parent != cur_ptr) {
          cur_node.parent = cur_ptr;
          _fs.write(next_ptr, cur_node);
        }
        cur_ptr = next_ptr;
      } else {
        cur_ptr = cur_node.right;
        _fs.read(cur_ptr, cur_node);
      }
      continue;
    }
    if(cur_node.is_leaf) return NodeSelf{cur_ptr, cur_node};
    int l = 0, r = cur_node.size - 1;
    while(l < r) {
      int mid = (l + r) >> 1;
      if(_key_comparer(cur_node.kv[mid].key, key)) l = mid + 1;
      else r = mid;
    }
    Nodeptr next_ptr = cur_node.child[l];
    _fs.read(next_ptr, cur_node);
    if(cur_node.parent != cur_ptr) {
      cur_node.parent = cur_ptr;
      _fs.write(next_ptr, cur_node);
    }
    cur_ptr = next_ptr;
  }
}

template <class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::average_from_left(
  NodeSelf &parent_self, NodeSelf &left_self, NodeSelf &right_self, int left_pos) {
  int left_size = (left_self.node.size + right_self.node.size) / 2;
  int right_size = (left_self.node.size + right_self.node.size) - left_size;
  int diff = left_self.node.size - left_size; // = right_size - right_self.node.size
  for(int i = right_self.node.size - 1; i >= 0; --i) {
    right_self.node.child[diff + i] = right_self.node.child[i];
    right_self.node.kv[diff + i] = right_self.node.kv[i];
  }
  for(int i = 0; i < diff; ++i) {
    right_self.node.child[i] = left_self.node.child[left_size + i];
    right_self.node.kv[i] = left_self.node.kv[left_size + i];
    left_self.node.child[left_size + i] = nullptr;
    left_self.node.kv[left_size + i] = kv_type{};
  }
  left_self.node.size = left_size;
  right_self.node.size = right_size;
  parent_self.node.kv[left_pos] = left_self.node.highkv();
  _fs.write(left_self.ptr, left_self.node);
  _fs.write(right_self.ptr, right_self.node);
  _fs.write(parent_self.ptr, parent_self.node);
}

template <class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::average_from_right(
NodeSelf &parent_self, NodeSelf &left_self, NodeSelf &right_self, int left_pos) {
  int left_size = (left_self.node.size + right_self.node.size) / 2;
  int right_size = (left_self.node.size + right_self.node.size) - left_size;
  int diff = left_size - left_self.node.size; // = right_self.node.size - right_size
  for(int i = 0; i < diff; ++i) {
    left_self.node.child[left_self.node.size + i] = right_self.node.child[i];
    left_self.node.kv[left_self.node.size + i] = right_self.node.kv[i];
  }
  for(int i = 0; i < right_size; ++i) {
    right_self.node.child[i] = right_self.node.child[diff + i];
    right_self.node.kv[i] = right_self.node.kv[diff + i];
    right_self.node.child[diff + i] = nullptr;
    right_self.node.kv[diff + i] = kv_type{};
  }
  left_self.node.size = left_size;
  right_self.node.size = right_size;
  parent_self.node.kv[left_pos] = left_self.node.highkv();
  _fs.write(left_self.ptr, left_self.node);
  _fs.write(right_self.ptr, right_self.node);
  _fs.write(parent_self.ptr, parent_self.node);
}


template <class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::split(
  NodeSelf &parent_self, NodeSelf &node_self, int pos) {
  NodeSelf right_self;
  int left_size = node_self.node.size / 2, right_size = node_self.node.size - left_size;
  for(int i = 0; i < right_size; ++i) {
    right_self.node.child[i] = node_self.node.child[left_size + i];
    right_self.node.kv[i] = node_self.node.kv[left_size + i];
    node_self.node.child[left_size + i] = nullptr;
    node_self.node.kv[left_size + i] = kv_type{};
  }
  right_self.node.size = right_size;
  node_self.node.size = left_size;
  right_self.node.is_leaf = node_self.node.is_leaf;
  right_self.node.parent = parent_self.ptr;
  right_self.node.left = node_self.ptr;
  right_self.node.right = node_self.node.right;
  right_self.ptr = _fs.alloc(right_self.node);
  node_self.node.right = right_self.ptr;
  if(right_self.node.right != nullptr) {
    Node rr_node;
    _fs.read(right_self.node.right, rr_node);
    rr_node.left = right_self.ptr;
    _fs.write(right_self.node.right, rr_node);
  }
  ++parent_self.node.size;
  for(int i = parent_self.node.size - 1; i > pos + 1; --i) {
    parent_self.node.child[i] = parent_self.node.child[i - 1];
    parent_self.node.kv[i] = parent_self.node.kv[i - 1];
  }
  parent_self.node.child[pos + 1] = right_self.ptr;
  parent_self.node.kv[pos + 1] = right_self.node.highkv();
  parent_self.node.kv[pos] = node_self.node.highkv();
  _fs.write(node_self.ptr, node_self.node);
  _fs.write(parent_self.ptr, parent_self.node);
}

template <class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::insertion_maintain(NodeSelf &node_self) {
  // remember to maintain the highkv.
  if(node_self.ptr == _root) {
    if(node_self.node.size <= k_node_size_max) return;
    Node root;
    root.kv[0] = node_self.node.highkv();
    root.child[0] = _root;
    root.size = 1; root.is_leaf = false;
    _root = _fs.alloc(root);
    NodeSelf parent_self{_root, root};
    node_self.node.parent = _root;
    _fs.write(node_self.ptr, node_self.node);
    split(parent_self, node_self, 0);
    return;
  }
  NodeSelf parent_self;
  parent_self.ptr = node_self.node.parent;
  _fs.read(parent_self.ptr, parent_self.node);
  bool is_highkv_modified = false;
  bool is_node_modified = false;
  // This judgement can be optimized out.
  int l = 0, r = parent_self.node.size - 1;
  while(l < r) {
    int mid = (l + r) >> 1;
    if(kv_comparer(parent_self.node.kv[mid], node_self.node.highkv())) l = mid + 1;
    else r = mid;
  }
  assert(parent_self.node.child[l] == node_self.ptr);
  if(!is_kv_equal(parent_self.node.kv[l], node_self.node.highkv())) {
    parent_self.node.kv[l] = node_self.node.highkv();
    _fs.write(parent_self.ptr, parent_self.node);
    is_highkv_modified = true;
  }
  if(node_self.node.size > k_node_size_max) {
    split(parent_self, node_self, l);
    is_node_modified = true;
  }
  if(is_highkv_modified || is_node_modified)
    insertion_maintain(parent_self);
}

template <class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::merge(
  NodeSelf &parent_self, NodeSelf &left_self, NodeSelf &right_self, int left_pos) {
  for(int i = 0; i < right_self.node.size; ++i) {
    left_self.node.child[left_self.node.size + i] = right_self.node.child[i];
    left_self.node.kv[left_self.node.size + i] = right_self.node.kv[i];
  }
  left_self.node.size += right_self.node.size;
  left_self.node.right = right_self.node.right;
  if(left_self.node.right != nullptr) {
    Node rr_node;
    _fs.read(left_self.node.right, rr_node);
    rr_node.left = left_self.ptr;
    _fs.write(left_self.node.right, rr_node);
  }
  for(int i = left_pos + 1; i < parent_self.node.size; ++i) {
    parent_self.node.kv[i] = parent_self.node.kv[i + 1];
    parent_self.node.child[i] = parent_self.node.child[i + 1];
  }
  --parent_self.node.size;
  parent_self.node.kv[left_pos] = left_self.node.highkv();
  // parent_self.node.child[left_pos] = left_self.ptr;
  _fs.write(left_self.ptr, left_self.node);
  _fs.dealloc(right_self.ptr);
  _fs.write(parent_self.ptr, parent_self.node);
}


template <class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::erasure_maintain(NodeSelf &node_self) {
  // remember to maintain highkv.
  if(node_self.ptr == _root) {
    if(node_self.node.size == 1 && node_self.node.child[0] != nullptr) {
      // don't discard the last element left.
      _root = node_self.node.child[0];
      _fs.dealloc(node_self.ptr);
      Node root;
      _fs.read(_root, root);
      root.parent = nullptr;
      _fs.write(_root, root);
    } else if(node_self.node.size == 0) {
      // clear the tree.
      _fs.dealloc(_root);
      _root = nullptr;
    }
    return;
  }
  NodeSelf parent_self;
  parent_self.ptr = node_self.node.parent;
  _fs.read(parent_self.ptr, parent_self.node);
  bool is_highkv_modified = false;
  bool is_node_modified = false;
  int l = 0, r = parent_self.node.size - 1;
  while(l < r) {
    int mid = (l + r) >> 1;
    if(kv_comparer(parent_self.node.kv[mid], node_self.node.highkv())) l = mid + 1;
    else r = mid;
  }
  assert(parent_self.node.child[l] == node_self.ptr);
  if(!is_kv_equal(parent_self.node.kv[l], node_self.node.highkv())) {
    parent_self.node.kv[l] = node_self.node.highkv();
    _fs.write(parent_self.ptr, parent_self.node);
    is_highkv_modified = true;
  }
  if(k_node_size_min > node_self.node.size) {
    NodeSelf left_self, right_self;
    bool has_left = (l > 0), has_right = (l < parent_self.node.size - 1);
    // maybe you can optimize it.
    if(has_left) {
      left_self.ptr = node_self.node.left;
      _fs.read(left_self.ptr, left_self.node);
    }
    if(has_right) {
      right_self.ptr = node_self.node.right;
      _fs.read(right_self.ptr, right_self.node);
    }
    if(has_left && k_node_size_fit > left_self.node.size)
      merge(parent_self, left_self, node_self, l - 1);
    else if(has_right && k_node_size_fit > right_self.node.size)
      merge(parent_self, node_self, right_self, l);
    else if(has_left && has_right) {
      if((left_self.node.size) < right_self.node.size)
        average_from_right(parent_self, node_self, right_self, l);
      else average_from_left(parent_self, left_self, node_self, l - 1);
    } else if(has_left)
      average_from_left(parent_self, left_self, node_self, l - 1);
    else if(has_right)
      average_from_right(parent_self, node_self, right_self, l);
    else assert(false); // Code shouldn't reach here.
    is_node_modified = true;
  }
  if(is_highkv_modified || is_node_modified)
    erasure_maintain(parent_self);
}

template<class Key, class Value, class KeyCompare, class ValueCompare>
bool database<Key, Value, KeyCompare, ValueCompare>::insert(const Key &key, const Value &value) {
  kv_type kv{key, value};
  if(occupancy_number() == 0) {
    Node root;
    root.kv[0] = kv;
    root.size = 1; root.is_leaf = true;
    _root = _fs.alloc(root);
    return true;
  }
  NodeSelf node_self = navigate_to_leaf(kv);
  if(kv_comparer(node_self.node.highkv(), kv)) {
    // larger than all existing kv.
    node_self.node.kv[node_self.node.size] = kv;
    ++node_self.node.size;
    // highkey maintenance is completed during insertion_maintain.
  } else {
    int l = 0, r = node_self.node.size - 1;
    while(l < r) {
      int mid = (l + r) >> 1;
      if(kv_comparer(node_self.node.kv[mid], kv)) l = mid + 1;
      else r = mid;
    }
    if(is_kv_equal(node_self.node.kv[l], kv)) return false;
    for(int i = node_self.node.size; i > l; --i)
      node_self.node.kv[i] = node_self.node.kv[i - 1];
    node_self.node.kv[l] = kv;
    ++node_self.node.size;
  }
  _fs.write(node_self.ptr, node_self.node);
  insertion_maintain(node_self);
  return true;
}

template <class Key, class Value, class KeyCompare, class ValueCompare>
void database<Key, Value, KeyCompare, ValueCompare>::erase(const Key &key, const Value &value) {
  if(occupancy_number() == 0) return;
  kv_type kv{key, value};
  NodeSelf node_self = navigate_to_leaf(kv);
  if(kv_comparer(node_self.node.highkv(), kv)) return;
  int l = 0, r = node_self.node.size - 1;
  while(l < r) {
    int mid = (l + r) >> 1;
    if(kv_comparer(node_self.node.kv[mid], kv)) l = mid + 1;
    else r = mid;
  }
  if(kv_comparer(kv, node_self.node.kv[l])) return;
  for(int i = l; i < node_self.node.size - 1; ++i)
    node_self.node.kv[i] = node_self.node.kv[i + 1];
  --node_self.node.size;
  // highkey maintenance is completed during erasure_maintain.
  _fs.write(node_self.ptr, node_self.node);
  erasure_maintain(node_self);
}

template <class Key, class Value, class KeyCompare, class ValueCompare>
vector<Value> database<Key, Value, KeyCompare, ValueCompare>::list(const Key &key) {
  vector<Value> res;
  if(occupancy_number() == 0) return res;
  NodeSelf node_self = navigate_to_leaf(key);
  if(_key_comparer(node_self.node.highkv().key, key)) return res;
  int l = 0, r = node_self.node.size - 1;
  while(l < r) {
    int mid = (l + r) >> 1;
    if(_key_comparer(node_self.node.kv[mid].key, key)) l = mid + 1;
    else r = mid;
  }
  int pos = l;
  while(true) {
    if(!is_key_equal(key, node_self.node.kv[pos].key)) return res;
    res.push_back(node_self.node.kv[pos].value);
    ++pos;
    if(pos == node_self.node.size) {
      if(node_self.node.right == nullptr) return res;
      node_self.ptr = node_self.node.right;
      _fs.read(node_self.ptr, node_self.node);
      pos = 0;
    }
  }
}

} // namespace Insomnia


#endif // INSOMNIA_DATABASE_TCC