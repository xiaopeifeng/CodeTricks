#pragma once

template <typename T>
class Queue
{
private:
	struct Node
	{
		Node()
		{
			ptr = nullptr;
			next = nullptr;
		}

		T* ptr;
		Node* next;
	};

public:
	Queue()
	{
		m_head = new Node;
		m_tail = m_head;
		m_size = 0;
	}

public:
	void Push(T* obj)
	{
		Node* p = new Node;
		p->ptr = obj;

		Node* old_tail;
		do
		{
			old_tail = m_tail;
		} while (InterlockedCompareExchange(m_tail->next, NULL, p) != old_tail);

		m_tail->next = p;
		m_tail = p;
		++m_size;
	}

	bool Pop(T*& obj)
	{
		if (m_size > 0)
		{
			obj = m_head->next->ptr;
			Node* temp = m_head->next->next;
			delete m_head->next;
			m_head->next = temp;
			--m_size;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void print()
	{
		Node* p = m_head->next;
		while (p)
		{
			std::cout << *(p->ptr) << std::endl;
			p = p->next;
		}
	}

	size_t Size()
	{
		return m_size;
	}

private:
	Node* m_head;
	Node* m_tail;
	size_t m_size;
};