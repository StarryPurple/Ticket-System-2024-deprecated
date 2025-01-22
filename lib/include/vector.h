#ifndef INSOMNIA_VECTOR_H
#define INSOMNIA_VECTOR_H

#include "lib_exceptions.h"

namespace Insomnia {

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
	~vector();
	vector& operator=(const vector &other);
	vector& operator=(vector &&other);
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	Tp& at(const size_t &pos);
	const Tp& at(const size_t &pos) const;
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	Tp& operator[](const size_t &pos);
	const Tp& operator[](const size_t &pos) const;
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const Tp& front() const;
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const Tp& back() const;
	iterator begin();
	const_iterator cbegin() const;
	iterator end();
	const_iterator cend() const;
	bool empty() const;
	size_t size() const;
	void clear();
	void reserve(size_t capacity);
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const Tp &value);
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const Tp &value);
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos);
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind);
	/**
	 * adds an element to the end.
	 */
	void push_back(const Tp &value);
	void push_back(Tp &&value);
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back();

};

} // namespace Insomnia

#include "vector.tcc"

#endif // INSOMNIA_VECTOR_H