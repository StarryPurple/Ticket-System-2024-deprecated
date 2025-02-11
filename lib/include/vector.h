#ifndef INSOMNIA_VECTOR_H
#define INSOMNIA_VECTOR_H

#include "lib_exceptions.h"

#include <vector>

namespace Insomnia {

template<class Tp>
using vector = std::vector<Tp>;

/*
template<class Tp, class Alloc = std::allocator<Tp>>
class vector {

private:
	Tp *_data;
	size_t _size, _capacity;
	Alloc _allocator;
	const bool _is_move_constructable = std::is_move_constructible_v<Tp>;

public:
	class const_iterator;
	class iterator {
		friend vector<Tp>;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = Tp;
		using pointer = Tp*;
		using reference = Tp&;
		using iterator_category = std::output_iterator_tag;

	private:
		const vector *container_ptr_;
		size_t index_;

	public:
		iterator(const vector &container, const size_t &index);
		iterator(const vector *container_ptr, const size_t &index);
		iterator(const iterator &);
		// iterator(iterator &&);
		iterator& operator=(const iterator &);
		// iterator& operator=(iterator &&);
		iterator operator+(const difference_type &n) const;
		iterator operator-(const difference_type &n) const;
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		difference_type operator-(const iterator &rhs) const;
		iterator& operator+=(const difference_type &n);
		iterator& operator-=(const difference_type &n);
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
	};

	class const_iterator {
		friend vector<Tp>;

	public:
		using difference_type = std::ptrdiff_t;
		using value_type = Tp;
		using pointer = Tp*;
		using reference = Tp&;
		using iterator_category = std::output_iterator_tag;

	private:
		const vector *container_ptr_;
		size_t index_;

	public:
		const_iterator(const vector &container, const size_t &index);
		const_iterator(const vector *container_ptr, const size_t &index);
		const_iterator(const const_iterator &);
		const_iterator& operator=(const const_iterator &);
		// const_iterator& operator=(const_iterator &&);
		const_iterator operator+(const difference_type &n) const;
		const_iterator operator-(const difference_type &n) const;
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		difference_type operator-(const const_iterator &rhs) const;
		const_iterator& operator+=(const difference_type &n);
		const_iterator& operator-=(const difference_type &n);
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
	};

	vector();
	vector(const vector &other);
	vector(vector &&other);
	vector(const std::initializer_list<Tp> &list);
	~vector();
	vector& operator=(const vector &other);
	vector& operator=(vector &&other);
	Tp& at(const size_t &pos);
	const Tp& at(const size_t &pos) const;
	Tp& operator[](const size_t &pos);
	const Tp& operator[](const size_t &pos) const;
	const Tp& front() const;
	const Tp& back() const;
	iterator begin() const;
	const_iterator cbegin() const;
	iterator end() const;
	const_iterator cend() const;
	bool empty() const;
	size_t size() const;
	void clear();
	void reserve(size_t capacity);
	iterator insert(iterator pos, const Tp &value);
	iterator insert(const size_t &ind, const Tp &value);
	iterator erase(iterator pos);
	iterator erase(const size_t &ind);
	void push_back(const Tp &value);
	void push_back(Tp &&value);
	void pop_back();
};
*/
} // namespace Insomnia

#include "vector.tcc"

#endif // INSOMNIA_VECTOR_H