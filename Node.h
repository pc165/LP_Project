#include <stdlib.h>
template <class T>
class Node {
public:

	Node() {
		m_data = 0;
		m_pNext = nullptr;
		m_pPrevious = nullptr;
	}
	Node(const int &a, Node *next = nullptr, Node *prev = nullptr) {
		m_data = a;
		m_pNext = next;
		m_pPrevious = prev;
	}
	void setPrev(Node *pt) { m_pPrevious = pt; }
	void setNext(Node *pt) { m_pNext = pt; }
	void setData(int  pt) { m_data = pt; }
	Node *getPrev() const { return m_pPrevious; }
	Node *getNext() const { return m_pNext; }
	T getValor() const { return m_data; }

private:
	T m_data;
	Node *m_pNext;
	Node *m_pPrevious;
};
