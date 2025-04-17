#ifndef INSOMNIA_VECTOR_H
#define INSOMNIA_VECTOR_H

#include "lib_exceptions.h"

#include <vector>

namespace insomnia {
template <class Tp>
using vector = std::vector<Tp>;
/*
template <class Tp>
class vector {
public:
  class const_iterator;
  class iterator {
    friend vector<Tp>;
    friend const_iterator;

  public:
    using differnce_type = std::ptrdiff_t;
    using value_type = Tp;
    using pointer = Tp*;
    using reference = Tp&;
    using iterator_category = std::output_iterator_tag;

    iterator();
    iterator(const iterator &);
    iterator& operator=(const iterator &);
    iterator operator+(const differnce_type &) const;
    iterator operator-(const differnce_type &) const;
    // throw invalid_iterator if two containers are not same
    differnce_type operator-(const iterator &) const;
    iterator& operator+=(const differnce_type &);
    iterator& operator-=(const differnce_type &);
    iterator operator++(int);
    iterator& operator++();
    iterator operator--(int);
    iterator& operator--();
    Tp& operator*() const;
    Tp* operator->() const;
    bool operator==(const iterator &) const;
    bool operator==(const const_iterator &) const;
    bool operator!=(const iterator &) const;
    bool operator!=(const const_iterator &) const;

  private:
    const vector<Tp> *_container;
    size_t _index;
    iterator(const vector<Tp> *container, const size_t &index);
  };
  class const_iterator {
    friend vector<Tp>;
    friend iterator;

  public:
    using differnce_type = std::ptrdiff_t;
    using value_type = Tp;
    using pointer = Tp*;
    using reference = Tp&;
    using iterator_category = std::output_iterator_tag;

    const_iterator();
    const_iterator(const const_iterator &);
    const_iterator(const iterator &);
    const_iterator& operator=(const const_iterator &);
    const_iterator operator+(const differnce_type &) const;
    const_iterator operator-(const differnce_type &) const;
    // throw invalid_iterator if two containers are not same
    differnce_type operator-(const const_iterator &) const;
    const_iterator& operator+=(const differnce_type &);
    const_iterator& operator-=(const differnce_type &);
    const_iterator operator++(int);
    const_iterator& operator++();
    const_iterator operator--(int);
    const_iterator& operator--();
    const Tp& operator*() const;
    const Tp* operator->() const;
    bool operator==(const const_iterator &) const;
    bool operator==(const iterator &) const;
    bool operator!=(const const_iterator &) const;
    bool operator!=(const iterator &) const;

  private:
    const vector<Tp> *_container;
    size_t _index;
    const_iterator(const vector<Tp> *container, const size_t &index);
  };

  vector();
  vector(const vector &);
  vector(vector &&) noexcept;
  ~vector();
  vector &operator=(const vector &);
  vector &operator=(vector &&);
  // throw index_out_of_bound if pos is not in [0, size)
  Tp& at(const size_t &pos);
  // throw index_out_of_bound if pos is not in [0, size)
  const Tp& at(const size_t &pos) const;
  Tp& operator[](const size_t &pos);
  const Tp& operator[](const size_t &pos) const;
  // throw container_is_empty if size is 0
  const Tp& front() const;
  // throw container_is_empty if size is 0
  const Tp& back() const;
  iterator begin() const;
  iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;
  bool empty() const;
  size_t size() const;
  void clear();
  void reserve(const size_t &capacity);
  // throw invalid_iterator if iter is not valid
  // throw index_out_of_bound if iter has an index > size
  iterator insert(const iterator &iter, const Tp &value);
  // throw invalid_iterator if iter is not valid
  // throw index_out_of_bound if iter has an index > size
  iterator insert(const size_t &index, const Tp &value);
  // throw invalid_iterator if iter is not valid
  // throw index_out_of_bound if iter has an index >= size
  iterator erase(const iterator &iter);
  // throw invalid_iterator if iter is not valid
  // throw index_out_of_bound if iter has an index >= size
  iterator erase(const size_t &index);
  void push_back(const Tp &);
  void push_back(Tp &&);
  // throw container_is_empty if size == 0
  void pop_back();

private:
  Tp *_data;
  size_t _left, _right;
  size_t _capacity;
};
*/
}

#include "vector.tcc"

#endif // INSOMNIA_VECTOR_H