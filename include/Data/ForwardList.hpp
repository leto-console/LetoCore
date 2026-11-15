/**
 * @file ForwardList.hpp
 * @date Nov 15, 2026
 * @author Rakhimov T.
 */

#pragma once

#include <type_traits>

#include <LetoAPI_V1/LetoAPI_V1.h>

#include "Iterators.hpp"

template <typename T>
class ForwardList
{
protected:
	struct Node
	{
		T value;
		Node* next = nullptr;

		Node(const T& value) : value{ value }
		{ }
	};

public:
	class Iterator : public ForwardIterator<T, Iterator>
	{
		friend ForwardList;
	public:
		Iterator(Node* current)
			: current{ current }
		{ }

		T& 			get_value() { return current->value; }
		T const& 	get_value() const { return current->value; }
		void 		next() const { current = current->next; }
		bool 		equal(const Iterator& iterator) const
		{
			return current == iterator.current;
		}

	protected:
		mutable Node* current = nullptr;
	};

	class ConstIterator : public ConstForwardIterator<T, ConstIterator>
	{
		friend ForwardList;
	public:
		ConstIterator(Node* current) 
			: current{ current }
		{ }
		ConstIterator(const Iterator& iterator) 
			: current{ iterator.current }
		{ }

		T const& 	get_value() const { return current->value; }
		void 		next() const { current = current->next; }
		bool 		equal(const ConstIterator& iterator) const
		{
			return current == iterator.current;
		}

	protected:
		mutable Node* current = nullptr;
	};

public:

	ForwardList(const LetoAllocator_V1* allocator = nullptr)
		: allocator{ allocator }
	{ }

	~ForwardList()
	{
		while (!empty()) pop_front();
	}

	bool empty() const { return head == nullptr; }
	size_t size() const { return m_size; }

	void push_front(const T& value)
	{
		Node* node = create_node(value);
		if (!node) return;

		node->next = head;
		head = node;
		if (!tail) tail = head;

		m_size++;
	}

	void push_back(const T& value)
	{
		Node* node = create_node(value);
		if (!node) return;

		if (tail) tail->next = node;
		tail = node;
		if (!head) head = tail;

		m_size++;
	}

	void pop_front()
	{
		if (!head) return;
		Node* next = head->next;
		delete_node(head);
		head = next;
		if (!head) tail = nullptr;

		m_size--;
	}

	void pop_back()
	{
		if (!head) return;
		if (head == tail)
		{
			delete_node(head);
			head = tail = nullptr;
			m_size = 0;
			return;
		}

		Node* node = get_node(-2);

		delete_node(node->next);
		node->next = nullptr;
		tail = node;

		m_size--;
	}

	/**
	* @brief Скопировать значение по индексу
	*/
	bool get(int32_t idx, T& value) const
	{
		Node* node = get_node(idx);
		if (!node) return false;
		value = node->value;
		return true;
	}

	/**
	* @brief Получить указатель на значение по индексу
	*/
	bool get(int32_t idx, const T*& value) const
	{
		Node* node = get_node(idx);
		if (!node) return false;
		value = &node->value;
		return true;
	}

	/**
	* @brief Получить указатель на значение по индексу
	*/
	bool get(int32_t idx, T*& value)
	{
		Node* node = get_node(idx);
		if (!node) return false;
		value = &node->value;
		return true;
	}

	void insert_after(ConstIterator iter, const T& value)
	{
		if (!iter.current) return;

		Node* node = create_node(value);
		if (!node) return;

		if (empty())
		{
			head = tail = node;
			m_size = 1;
			return;
		}

		node->next = iter.current->next;
		iter.current->next = node;

		m_size++;
	}

	void erase_after(ConstIterator iter)
	{
		if (!iter.current) return;

		Node* next = nullptr;
		if (iter.current->next)
		{
			if (iter.current->next == tail)
			{
				pop_back();
				return;
			}

			next = iter.current->next->next;
			iter.current->next = next;
			m_size--;
		}
	}

	Iterator begin() { return Iterator{ head }; }
	Iterator end() { return Iterator{ nullptr }; }

	ConstIterator begin() const { return ConstIterator{ head }; }
	ConstIterator end() const { return ConstIterator{ nullptr }; }

	ConstIterator cbegin() const { return begin(); }
	ConstIterator cend() const { return end(); }

protected:
	const LetoAllocator_V1* allocator = nullptr;

	Node* head = nullptr;
	Node* tail = nullptr;
	uint32_t m_size = 0;

	/**
	* @brief Получить ноду по индексу
	*/
	Node* get_node(int32_t idx) const
	{
		int32_t m_size = this->m_size;
		if (idx > m_size || idx < -m_size) return nullptr;
		if (idx < 0) idx += m_size;

		Node* node = head;
		for (; idx > 0; --idx) node = node->next;

		return node;
	}

	Node* create_node(const T& value)
	{
		if (!allocator) return nullptr;
		return new(allocator) Node{ value };
	}

	void delete_node(Node* node)
	{
		if (!allocator) return;
		allocator->Free(node);
	}
};
