#ifndef _HOME_STUDENT___RESOURCES___LIST_H_
#define _HOME_STUDENT___RESOURCES___LIST_H_

// "Copyright [2017] <Gavan Adrian-George, 314CA>"

template <typename T>
struct Node {
  T value;
  Node<T> *next, *prev;
  Node() {
    next = prev = NULL;
  }
};

template <typename T>
class LinkedList {
 private:
  Node<T> *pFirst, *pLast;

 public:
  //  Constructor
  LinkedList() {
    pFirst = pLast = NULL;
  }

  //  Destructor
  ~LinkedList() {
    while (pFirst != NULL) {
      Node<T> *aux = pFirst;
      pFirst = pFirst->next;
      delete aux;
      pFirst = NULL;
      pLast = NULL;
    }
  }

  /* Returneaza primul nod din lista */
  Node<T> *front() {
    return pFirst;
  }

  /* Adauga un nod cu valoarea == value la sfarsitul listei. */
  void addLast(T value) {
    Node<T> *newElem = new Node<T>;
    newElem->value = value;
    if (pFirst == NULL) {
      newElem->next = NULL;
      newElem->prev = NULL;
      pFirst = pLast = newElem;
    } else {
      newElem->next = NULL;
      newElem->prev = pLast;
      pLast->next = newElem;
      pLast = newElem;
    }
  }

  /* Elimina elementul de la inceputul listei. */
  void removeFirst() {
    struct Node<T> *paux;
    if (pFirst == NULL) {
      std::cout << "Lista goala" << std::endl;
    }
    paux = pFirst->next;
    if (pFirst == pLast) {
      pLast = NULL;
    }
    delete pFirst;
    pFirst = paux;
    if (pFirst != NULL) {
      pFirst->prev = NULL;
    }
  }

  /* Intoarce true daca lista este vida, false altfel. */
  bool isEmpty() {
    if (pFirst == NULL) {
      return true;
    } else {
      return false;
    }
  }
};

#endif  // _HOME_STUDENT___RESOURCES___LIST_H_
