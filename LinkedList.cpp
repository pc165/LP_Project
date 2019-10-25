#include "LinkedList.h"
template <class T>
List<T>::List() {
    m_llista = nullptr;
    //m_last = nullptr;
}
template <class T>
List<T>::~List() {
    Node<T> *prev = m_llista;
    while (prev != nullptr) {
        m_llista = m_llista->getNext();
        delete prev;
        prev = m_llista;
    }
}

template <class T>
bool List<T>::empty() const {
    return m_llista == nullptr;
}
template <class T>
Node<T> *List<T>::begin() const {
    return m_llista;
}
template <class T>
Node<T> *List<T>::rbegin() const {
    if (m_llista == nullptr) return nullptr;

    Node<T> *temp = m_llista;
    while (temp->getNext() != nullptr)
        temp = temp->getNext();
    return temp;
}
template <class T>
Node<T> *List<T>::insert(const int &valor, Node<T> *posicio) {

    Node<T> *ret = nullptr;
    if (m_llista == nullptr) {
        m_llista = new Node<T>(valor);
        ret = m_llista;
    } else if (posicio == NULL) {
        Node<T> *a = rbegin();
        a->setNext(new Node<T>(valor, nullptr, a));
        ret = a->getNext();
    } else {
        ret = new Node<T>(valor, posicio, posicio->getPrev());
        if (posicio == m_llista) // "push back"
            m_llista = ret;
        else
            posicio->getPrev()->setNext(ret);
        posicio->setPrev(ret);
    }
    return ret;
}
template<class T>
Node<T> *List<T>::erase(Node<T> *posicio) {
    if (m_llista == nullptr || posicio == nullptr) return nullptr;

    Node<T> *next = posicio->getNext();
    Node<T> *prev = posicio->getPrev();
    if (next != nullptr)
        next->setPrev(prev);
    if (prev != nullptr)
        prev->setNext(next);
    else
        m_llista = next;
    delete posicio;

    return next;

}
template<class T>
void List<T>::unique() {
    if (m_llista == nullptr) return;

    Node<T> *temp = m_llista;
    while (temp->getNext() != nullptr) {
        if (temp->getNext()->getVal() == temp->getVal()) {
            erase(temp->getNext());
        } else {
            temp = temp->getNext();
        }
    }
}
template<class T>
void List<T>::merge(List<T> &llista) {
    List<T> tempLlista;
    if (this->m_llista == nullptr) {
        this->m_llista = llista.m_llista;
    } else if (llista.m_llista != nullptr) {
        llista.m_llista->setPrev(this->rbegin());
        this->rbegin()->setNext(llista.m_llista);
    }
    llista.m_llista = nullptr;


    do {
        Node<T> *it = m_llista, *aux = m_llista, *max = m_llista;
        while (it != nullptr) {
            aux = it;
            if (aux->getVal() > max->getVal())
                max = aux;
            it = it->getNext();
        }
        if (max != nullptr)
            tempLlista.insert(max->getVal(), tempLlista.m_llista);
        this->erase(max);
    } while (!this->empty());
    this->m_llista = tempLlista.m_llista;
    tempLlista.m_llista = nullptr;

}

template<class T>
List<T> &List<T>::operator=(const List<T> &llista) {
    Node<T> *it = llista.rbegin();
    while (it != nullptr) {
        this->insert(it->getVal(), m_llista);
        it = it->getPrev();
    }
    return *this;
}