#ifndef INSOMNIA_MULTIMAP_DB_H
#define INSOMNIA_MULTIMAP_DB_H

#include "filestream.h"
#include "pair.hpp"

#include <mutex>
#include <atomic>

namespace Insomnia {

/** @brief A B+Tree based in-file database manager.
 *  Supports in-file operations like in-memory std::multimap.
 */
template<class Key, class Value, class KeyCompare = std::less<Key>, class ValueCompare = std::less<Value>>
class multimap_db {
  static_assert(std::is_default_constructible_v<Key> && std::is_default_constructible_v<Value>,
    "KeyType and ValueType is required to be default constructible in Insomnia::multimap_db");
  using kv_pair = pair<Key, Value>;
  static constexpr int k_node_max_size = 4096 / sizeof(kv_pair), k_node_min_size = k_node_max_size / 2 - 2;
private:
  struct Info {
    fspointer root;
    int size = 0;
  };
  struct Node {
    kv_pair kv[k_node_max_size + 1];
    fspointer child[k_node_max_size];
    fspointer parent, left, right;
    int size = 0;
    bool is_leaf = false;
    std::mutex lock;
  };
  struct this_loc {
    fspointer parent_ptr;
    Node parent_node;
    int pos;
  };

  filestream<Node, Info> _fs;
  Info _info;
  vector<this_loc> _route;
  KeyCompare _key_comparer;
  ValueCompare _value_comparer;

  void average(this_loc &left_loc);
  void split(this_loc &node_loc);
  void merge(this_loc &left_loc);
  void maintain();

public:
  multimap_db(const std::filesystem::path &file);
  multimap_db(const multimap_db &) = delete;
  multimap_db(multimap_db &&) = delete;
  ~multimap_db();
  multimap_db& operator=(const multimap_db &) = delete;
  multimap_db& operator=(multimap_db &&) = delete;

  void open();
  void close();
  void insert(const Key &key, const Value &value);
  // returns {nullptr, 0} for failure.
  pair<fspointer, int> find(const Key &key, const Value &value);
  void erase(const fspointer &ptr);
  // returns all values with index "key" in the order of ValueCompare.
  vector<Value> operator[](const Key &key);

  bool is_open() const;
};
}


#include "multimap_db.tcc"

#endif // INSOMNIA_MULTIMAP_DB_H