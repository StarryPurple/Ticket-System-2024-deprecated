#ifndef INSOMNIA_DATABASE_H
#define INSOMNIA_DATABASE_H

#include "filestream.h"
#include "pair.h"

namespace insomnia {

/** @brief A B+Tree based in-file database manager.
 *  Supports in-file operations like in-memory std::multimap.
 */
template<class Key, class Value, class KeyCompare = std::less<Key>, class ValueCompare = std::less<Value>>
class bplustree {
  static_assert(std::is_default_constructible_v<Key> && std::is_default_constructible_v<Value>,
    "KeyType and ValueType is required to be default constructible in Insomnia::database");
  using Nodeptr = fspointer;
private:
  struct kv_type {
    Key key;
    Value value;
    kv_type() = default;
    kv_type(const Key &the_key, const Value &the_value);
  };
  static constexpr int
    k_node_size_max = (sizeof(kv_type) * 16 / 4096 + 1) * 4096 / sizeof(kv_type),
    k_node_size_min = k_node_size_max / 2 - 3,
    k_node_size_fit = k_node_size_max / 2 + 1;
  // static_assert(k_node_size_max >= 16,
  //   "You shall choose smaller Key and Value types");
  struct Node {
    kv_type kv[k_node_size_max + 1]{};
    Nodeptr child[k_node_size_max + 1]{};
    Nodeptr parent, left, right;
    int size = 0;
    bool is_leaf = false;
    Node() = default;
    Node(const Node &) = default;
    kv_type& highkv();
  };
  struct NodeSelf {
    Nodeptr ptr;
    Node node;
    NodeSelf() = default;
    NodeSelf(const Nodeptr &the_ptr, const Node &the_node);
  };

  filestream<Node, Nodeptr> _fs;
  Nodeptr _root;
  KeyCompare _key_comparer;
  ValueCompare _value_comparer;

  bool kv_comparer(const kv_type &, const kv_type &) const;
  bool is_key_equal(const Key &, const Key &) const;
  bool is_kv_equal(const kv_type &, const kv_type &) const;
  NodeSelf navigate_to_leaf(const kv_type &kv);
  NodeSelf navigate_to_leaf(const Key &key);
  void average_from_left(NodeSelf &parent_self, NodeSelf &left_self, NodeSelf &right_self, int left_pos);
  void average_from_right(NodeSelf &parent_self, NodeSelf &left_self, NodeSelf &right_self, int left_pos);
  void split(NodeSelf &parent_self, NodeSelf &node_self, int pos);
  void insertion_maintain(NodeSelf &node_self);
  void merge(NodeSelf &parent_self, NodeSelf &left_self, NodeSelf &right_self, int left_pos);
  void erasure_maintain(NodeSelf &node_self);

public:
  bplustree() = default;
  ~bplustree();

  bplustree(const bplustree &) = delete;
  bplustree(bplustree &&) = delete;
  bplustree& operator=(const bplustree &) = delete;
  bplustree& operator=(bplustree &&) = delete;

  // is_open() == false required.
  void renew(const std::string &filename);
  // renew current file. is_open() == true required.
  void renew();
  // open a file.
  // can't open a file not existing.
  // Remember to use renew() function to create one.
  void open(const std::string &filename);
  void close();
  // returns whether insertion succeeds.
  // possible failure: <key, value> pair already exists.
  bool insert(const Key &key, const Value &value);
  bool erase(const Key &key, const Value &value);
  // returns all values with index "key" in the order of ValueCompare.
  vector<Value> list(const Key &key);

  bool is_open() const;
  size_t occupancy_number() const;
  double occupancy_rate() const;
  bool empty() const;
};
}


#include "database.tcc"

#endif // INSOMNIA_DATABASE_H