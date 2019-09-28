#include "Node.h"

template <class T>
class List {
public:
	List();
	~List();

	bool empty() const;
	Node<T> *begin() const;
	Node<T> *rbegin() const;
	Node<T> *insert(const int &valor, Node<T> *posicio);
	Node<T> *erase(Node<T> *posicio);
	void unique();
	void merge(List &llista);
	List &operator=(const List &llista);
private:
	//Node<T> *m_last;
	Node<T> *m_llista;
};