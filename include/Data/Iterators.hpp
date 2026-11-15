/**
 * @file Iterators.hpp
 * @date Nov 15, 2026
 * @author Rakhimov T.
 */

#pragma once

// ================================================== Base ==================================================

/**
 * @brief Base constant iterator
 * @tparam T Value type
 * @tparam Derived Iterator implementation
 * 
 * @note `Derived` must have:
 * 
 * - `bool Derived::equal(const Derived& iterator) const`
 * 
 * - `const T& Derived::get_value() const`
 */
template <typename T, typename Derived>
class ConstBaseIterator
{
public:
	bool operator==(const Derived& iterator) const { return self()->equal(iterator); }
	bool operator!=(const Derived& iterator) const { return !self()->equal(iterator); }

	T const& operator* ()	const { return self()->get_value(); }
	T const* operator-> ()	const { return &self()->get_value(); }

protected:
	Derived const*	self() const										{ return to_derived(this); }
	Derived const*	to_derived(const ConstBaseIterator* base) const     { return static_cast<Derived const*>(base); }
};

/**
 * @brief Base iterator
 * @tparam T Value type
 * @tparam Derived Iterator implementation
 * 
 * @note `Derived` must have:
 * 
 * - `bool Derived::equal(const Derived& iterator) const`
 * 
 * - `const T& Derived::get_value() const`
 * 
 * - `T& Derived::get_value()`
 */
template <typename T, typename Derived>
class BaseIterator : public ConstBaseIterator<T, Derived>
{
public:
	T& 	operator* ()	{ return self()->get_value(); }
	T* 	operator-> ()	{ return &self()->get_value(); }

protected:
	Derived* self()								{ return to_derived(this); }
	Derived* to_derived(BaseIterator* base)	    { return static_cast<Derived*>(base); }
};


// ================================================ Forward =================================================

/**
 * @brief Const forward iterator
 * @tparam T Value type
 * @tparam Derived Iterator implementation
 * 
 * @note `Derived` must have:
 * 
 * - `bool Derived::equal(const Derived& iterator) const`
 * 
 * - `const T& Derived::get_value() const`
 * 
 * - `void Derived::next() const`
 */
template <typename T, typename Derived>
class ConstForwardIterator : public ConstBaseIterator<T, Derived>
{
public:
	using ConstBaseIterator<T, Derived>::self;

	// ++iter
	const Derived& operator++() const
	{
		self()->next();
		return *self();
	}

	// iter++
	const Derived& operator++(int) const
	{
		const Derived& temp = *self();
		++(*this);
		return temp;
	}

	// iter + value
	const Derived& operator+(uint32_t step) const
	{
		while(step--) self()->next();
		return *self();
	}
};

/**
 * @brief Forward iterator
 * @tparam T Value type
 * @tparam Derived Iterator implementation
 * 
 * @note `Derived` must have:
 * 
 * - `bool Derived::equal(const Derived& iterator) const`
 * 
 * - `const T& Derived::get_value() const`
 * 
 * - `T& Derived::get_value()`
 * 
 * - `void Derived::next() const`
 */
template <typename T, typename Derived>
class ForwardIterator : public BaseIterator<T, Derived>
{
public:
	using BaseIterator<T, Derived>::self;

	// ++iter
	Derived& operator++()
	{
		self()->next();
		return *self();
	}

	// iter++
	Derived& operator++(int)
	{
		const Derived& temp = *self();
		++(*this);
		return temp;
	}

	// iter + value
	Derived& operator+(uint32_t step)
	{
		while(step--) self()->next();
		return *self();
	}
};

// ============================================= Bidirectional ==============================================

/**
 * @brief Const bidirectional iterator
 * @tparam T Value type
 * @tparam Derived Iterator implementation
 * 
 * @note `Derived` must have:
 * 
 * - `bool Derived::equal(const Derived& iterator) const`
 * 
 * - `const T& Derived::get_value() const`
 * 
 * - `void Derived::next() const`
 * 
 * - `void Derived::prev() const`
 */
template <typename T, typename Derived>
class ConstBidirectionalIterator : public ConstForwardIterator<T, Derived>
{
public:
	using ConstBaseIterator<T, Derived>::self;

	// --iter
	const Derived& operator--() const
	{
		self()->prev();
		return *self();
	}

	// iter--
	const Derived& operator--(int) const
	{
		const Derived& temp = *self();
		--(*this);
		return temp;
	}

	// iter - value
	const Derived& operator-(uint32_t step) const
	{
		while(step--) self()->prev();
		return *self();
	}
};

/**
 * @brief Forward bidirectional
 * @tparam T Value type
 * @tparam Derived Iterator implementation
 * 
 * @note `Derived` must have:
 * 
 * - `bool Derived::equal(const Derived& iterator) const`
 * 
 * - `const T& Derived::get_value() const`
 * 
 * - `T& Derived::get_value()`
 * 
 * - `void Derived::next() const`
 * 
 * - `void Derived::prev() const`
 */
template <typename T, typename Derived>
class BidirectionalIterator : public ForwardIterator<T, Derived>
{
public:
	using BaseIterator<T, Derived>::self;

	// --iter
	Derived& operator--()
	{
		self()->prev();
		return *self();
	}

	// iter--
	Derived& operator--(int)
	{
		const Derived& temp = *self();
		--(*this);
		return temp;
	}

	// iter - value
	Derived& operator-(uint32_t step)
	{
		while(step--) self()->prev();
		return *self();
	}
};

