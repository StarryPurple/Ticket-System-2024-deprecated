#ifndef INSOMNIA_VECTOR_TCC
#define INSOMNIA_VECTOR_TCC

#include "vector.h"

namespace Insomnia {
/*
// class vector::iterator

template<class Tp, class Alloc>
vector<Tp, Alloc>::iterator::iterator(const vector &container, const size_t &index)
	: container_ptr_(&container), index_(index) {}

template<class Tp, class Alloc>
vector<Tp, Alloc>::iterator::iterator(const vector *container_ptr, const size_t &index)
	: container_ptr_(container_ptr), index_(index) {}

template<class Tp, class Alloc>
vector<Tp, Alloc>::iterator::iterator(const iterator &other)
  : container_ptr_(other.container_ptr_), index_(other.index_) {}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator&
  vector<Tp, Alloc>::iterator::operator=(const iterator &other) {
	if(this == &other) return *this;
	container_ptr_ = other.container_ptr_;
	index_ = other.index_;
	return *this;
}


template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::iterator::operator+(const difference_type &n) const {
	if(n < 0) return *this - (-n);
	if(index_ + n > container_ptr_->_size) throw invalid_iterator();
	return {container_ptr_, index_ + n};
}
template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::iterator::operator-(const difference_type &n) const {
	if(n < 0) return *this + (-n);
	if(index_ < n) throw index_out_of_bound();
	return {container_ptr_, index_ - n};
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator::difference_type
  vector<Tp, Alloc>::iterator::operator-(const iterator &rhs) const {
	if(container_ptr_ != rhs.container_ptr_) throw invalid_iterator();
	return static_cast<difference_type>(index_)- static_cast<difference_type>(rhs.index_);
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator&
  vector<Tp, Alloc>::iterator::operator+=(const difference_type &n) {
	if(n < 0) return *this -= (-n);
	if(index_ + n > container_ptr_->_size) throw invalid_iterator();
	index_ += n;
	return *this;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator&
	vector<Tp, Alloc>::iterator::operator-=(const difference_type &n) {
	if(n < 0) return *this += (-n);
	if(index_ < n) throw invalid_iterator();
	index_ -= n;
	return *this;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::iterator::operator++(int) {
	iterator tmp = *this;
	*this += 1;
	return tmp;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator&
	vector<Tp, Alloc>::iterator::operator++() {
	return *this += 1;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::iterator::operator--(int) {
	iterator tmp = *this;
	*this -= 1;
	return tmp;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator&
	vector<Tp, Alloc>::iterator::operator--() {
	return *this -= 1;
}

template<class Tp, class Alloc>
Tp& vector<Tp, Alloc>::iterator::operator*() const {
	return container_ptr_->_data[index_];
}

template<class Tp, class Alloc>
Tp* vector<Tp, Alloc>::iterator::operator->() const {
	return &(**this);
}

template<class Tp, class Alloc>
bool vector<Tp, Alloc>::iterator::operator==(const iterator &rhs) const {
	return *this - rhs == 0;
}

template<class Tp, class Alloc>
bool vector<Tp, Alloc>::iterator::operator==(const const_iterator &rhs) const {
	return container_ptr_ == rhs.container_ptr_ && index_ == rhs.index_;
}

template<class Tp, class Alloc>
bool vector<Tp, Alloc>::iterator::operator!=(const iterator &rhs) const {
	return !(*this == rhs);
}

template<class Tp, class Alloc>
bool vector<Tp, Alloc>::iterator::operator!=(const const_iterator &rhs) const {
	return !(*this == rhs);
}

// class vector::const_iterator

template<class Tp, class Alloc>
vector<Tp, Alloc>::const_iterator::const_iterator(const vector &container, const size_t &index)
  : container_ptr_(&container), index_(index) {}

template<class Tp, class Alloc>
vector<Tp, Alloc>::const_iterator::const_iterator(const vector *container_ptr, const size_t &index)
	: container_ptr_(container_ptr), index_(index) {}

template<class Tp, class Alloc>
vector<Tp, Alloc>::const_iterator::const_iterator(const const_iterator &other)
  : container_ptr_(other.container_ptr_), index_(other.index_) {}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::const_iterator&
	vector<Tp, Alloc>::const_iterator::operator=(const const_iterator &other) {
	if(this == &other) return *this;
	container_ptr_ = other.container_ptr_;
	index_ = other.index_;
	return *this;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::const_iterator
  vector<Tp, Alloc>::const_iterator::operator+(const difference_type &n) const {
	if(n < 0) return *this - (-n);
	if(index_ + n > container_ptr_->_size) throw invalid_iterator();
	return {container_ptr_, index_ + n};
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::const_iterator
  vector<Tp, Alloc>::const_iterator::operator-(const difference_type &n) const {
	if(n < 0) return *this + (-n);
	if(index_ < n) throw invalid_iterator();
	return {container_ptr_, index_ - n};
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::difference_type
  vector<Tp, Alloc>::const_iterator::operator-(const const_iterator &rhs) const {
	if(container_ptr_ != rhs.container_ptr_) throw invalid_iterator();
	return static_cast<difference_type>(index_)- static_cast<difference_type>(rhs.index_);
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::const_iterator&
	vector<Tp, Alloc>::const_iterator::operator+=(const difference_type &n) {
	if(n < 0) return *this -= (-n);
	if(index_ + n > container_ptr_->_size) throw invalid_iterator();
	index_ += n;
	return *this;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::const_iterator&
	vector<Tp, Alloc>::const_iterator::operator-=(const difference_type &n) {
	if(n < 0) return *this += (-n);
	if(index_ < n) throw index_out_of_bound();
	index_ -= n;
	return *this;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::const_iterator
  vector<Tp, Alloc>::const_iterator::operator++(int) {
	iterator tmp = *this;
	*this += 1;
	return tmp;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::const_iterator&
	vector<Tp, Alloc>::const_iterator::operator++() {
	return *this += 1;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::const_iterator
  vector<Tp, Alloc>::const_iterator::operator--(int) {
	iterator tmp = *this;
	*this -= 1;
	return tmp;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator::const_iterator&
	vector<Tp, Alloc>::const_iterator::operator--() {
	return *this -= 1;
}

template<class Tp, class Alloc>
const Tp& vector<Tp, Alloc>::const_iterator::operator*() const {
	return container_ptr_->_data[index_];
}

template<class Tp, class Alloc>
const Tp* vector<Tp, Alloc>::const_iterator::operator->() const {
	return &(**this);
}

template<class Tp, class Alloc>
bool vector<Tp, Alloc>::const_iterator::operator==(const const_iterator &rhs) const {
	return *this - rhs == 0;
}

template<class Tp, class Alloc>
bool vector<Tp, Alloc>::const_iterator::operator==(const iterator &rhs) const {
	return container_ptr_ == rhs.container_ptr_ && index_ == rhs.index_;
}

template<class Tp, class Alloc>
bool vector<Tp, Alloc>::const_iterator::operator!=(const const_iterator &rhs) const {
	return !(*this == rhs);
}

template<class Tp, class Alloc>
bool vector<Tp, Alloc>::const_iterator::operator!=(const iterator &rhs) const {
	return !(*this == rhs);
}

// class vector


template<class Tp, class Alloc>
vector<Tp, Alloc>::vector(): _size(0), _capacity(16) {
	_data = _allocator.allocate(_capacity);
}

template<class Tp, class Alloc>
vector<Tp, Alloc>::vector(const vector &other) {
	_size = other._size;
	_capacity = other._capacity;
	_data = _allocator.allocate(_capacity);
	for(size_t i = 0; i < _size; ++i)
		new (_data + i) Tp(other._data[i]);
}

template<class Tp, class Alloc>
vector<Tp, Alloc>::vector(vector &&other) {
	_size = other._size;
	_capacity = other._capacity;
	_data = other._data;
	other._data = nullptr;
}

template <class Tp, class Alloc>
vector<Tp, Alloc>::vector(const std::initializer_list<Tp> &list): vector() {
	reserve(list.size());
	_size = list.size();
	// not exception-safe
	size_t i = 0;
	for(const auto &value: list)
		new (_data + (i++)) Tp(value);
}


template<class Tp, class Alloc>
vector<Tp, Alloc>::~vector() {
	clear();
	_allocator.deallocate(_data, _capacity);
	_capacity = 0;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::vector&
	vector<Tp, Alloc>::operator=(const vector &other) {
	if(this == &other) return *this;
	clear();
	reserve(other._capacity);
	_size = other._size;
	for(size_t i = 0; i < _size; ++i)
		new (_data + i) Tp(other._data[i]);
	return *this;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::vector&
	vector<Tp, Alloc>::operator=(vector &&other) {
	if(this == &other) return *this;
	_size = other._size;
	_capacity = other._capacity;
	_data = other._data;
	other._size = 0;
	other._capacity = 0;
	other._data = nullptr;
	return *this;
}

template<class Tp, class Alloc>
Tp& vector<Tp, Alloc>::at(const size_t &pos) {
	if(pos >= _size) throw index_out_of_bound();
	return _data[pos];
}

template<class Tp, class Alloc>
const Tp& vector<Tp, Alloc>::at(const size_t &pos) const {
	if(pos >= _size) throw index_out_of_bound();
	return _data[pos];
}

template<class Tp, class Alloc>
Tp& vector<Tp, Alloc>::operator[](const size_t &pos) {
	if(pos >= _size) throw index_out_of_bound();
	return _data[pos];
}

template<class Tp, class Alloc>
const Tp& vector<Tp, Alloc>::operator[](const size_t &pos) const {
	if(pos >= _size) throw index_out_of_bound();
	return _data[pos];
}

template<class Tp, class Alloc>
const Tp& vector<Tp, Alloc>::front() const {
	if(empty()) throw container_is_empty();
	return _data[0];
}

template<class Tp, class Alloc>
const Tp& vector<Tp, Alloc>::back() const {
	if(empty()) throw container_is_empty();
	return _data[_size - 1];
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::begin() const {
	return {*this, 0};
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator
  vector<Tp, Alloc>::cbegin() const {
	return {*this, 0};
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::end() const {
	return {*this, _size};
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::const_iterator
  vector<Tp, Alloc>::cend() const {
	return {*this, _size};
}

template<class Tp, class Alloc>
bool vector<Tp, Alloc>::empty() const {
	return _size == 0;
}

template<class Tp, class Alloc>
size_t vector<Tp, Alloc>::size() const {
	return _size;
}

template<class Tp, class Alloc>
void vector<Tp, Alloc>::clear() {
	for(size_t i = 0; i < _size; ++i)
		_data[i].~Tp();
	_size = 0;
}

template<class Tp, class Alloc>
void vector<Tp, Alloc>::reserve(size_t capacity) {
	if(_capacity >= capacity) return;
	Tp *new_data = _allocator.allocate(capacity);
	if(_is_move_constructable) {
		for(size_t i = 0; i < _size; ++i)
			new (new_data + i) Tp(std::move(_data[i]));
	} else {
		for(size_t i = 0; i < _size; ++i)
			new (new_data + i) Tp(_data[i]);
	}
	if constexpr(std::is_class_v<Tp>) {
		for(size_t i = 0; i < _size; ++i)
			_data[i].~Tp(); // remember to check while compiling
	}
	_allocator.deallocate(_data, _capacity);

	_data = new_data;
	_capacity = capacity;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::insert(iterator pos, const Tp &value) {
	size_t index = pos.index_;
	if(index > _size) throw index_out_of_bound();
	if(_size == _capacity) reserve(_capacity << 1); // new capacity may be larger than size_t_max.
	++_size;
	if(_is_move_constructable) {
		for(size_t i = _size - 1; i > index; --i) {
			new (_data + i) Tp(std::move(_data[i - 1]));
			_data[i - 1].~T();
		}
	} else {
		for(size_t i = _size - 1; i > index; --i) {
			new (_data + i) Tp(_data[i - 1]);
			_data[i - 1].~T();
		}
	}
	new (_data + index) Tp(value);
	return pos;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::insert(const size_t &ind, const Tp &value) {
	return insert(iterator(this, ind), value);
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::erase(iterator pos) {
	size_t index = pos.index_;
	if(index >= _size) throw index_out_of_bound();
	--_size;
	_data[index].~T();
	if(_is_move_constructable) {
		for(size_t i = index; i < _size; ++i) {
			new (_data + i) Tp(std::move(_data[i + 1]));
			_data[i + 1].~T();
		}
	} else {
		for(size_t i = index; i < _size; ++i) {
			new (_data + i) Tp(_data[i + 1]);
			_data[i + 1].~T();
		}
	}
	return pos;
}

template<class Tp, class Alloc>
typename vector<Tp, Alloc>::iterator
  vector<Tp, Alloc>::erase(const size_t &ind) {
	return erase(iterator(this, ind));
}

template<class Tp, class Alloc>
void vector<Tp, Alloc>::push_back(const Tp &value) {
	if(_size == _capacity) reserve(_capacity << 1); // new capacity may be larger than size_t_max
	new (_data + _size) Tp(value);
	++_size;
}

template<class Tp, class Alloc>
void vector<Tp, Alloc>::push_back(Tp &&value) {
	if(_size == _capacity) reserve(_capacity << 1);
	new (_data + _size) Tp(std::move(value));
	++_size;
}

template<class Tp, class Alloc>
void vector<Tp, Alloc>::pop_back() {
	if(empty()) throw container_is_empty();
	--_size;
	_data[_size].~T();
}
*/
} // namespace Insomnia

#endif // INSOMNIA_VECTOR_TCC