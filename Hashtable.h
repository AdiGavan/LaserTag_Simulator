#ifndef _HOME_STUDENT___RESOURCES___HASHTABLE_H_
#define _HOME_STUDENT___RESOURCES___HASHTABLE_H_

// "Copyright [2017] <Gavan Adrian-George, 314CA>"

#include "List.h"
#define SMAX 100

/* Structura valorii din fiecare element din lista din Hashtable */
template<typename Tkey, typename Tvalue> struct elem_info {
  Tkey Nume;  // Numele jucatorului
  Tvalue index_matrice;  // Asocierea cu indicele unei matrici
  int echipa;  // Echipa jucatorului
  int top_explorer;  // Distanta parcursa a jucatorului
  int senzor_anterior;  // Ultimul senzor prin care a trecut
  int best_shooter;  // Scorul best_shooter al jucatorului
  int alive;  // Numarul de vieti al jucatorului (2, 1 - viu; 0 - mort)
  int senzori[SMAX];  // Senzorii unici
};

/* Clasa Hashtable */
template<typename Tkey, typename Tvalue> class Hashtable {
 private:
  LinkedList<struct elem_info<Tkey, Tvalue> > *H;
  int HMAX;
  int (*hash)(Tkey);

 public:
  /* Constructor */
  Hashtable(int hmax, int (*h)(Tkey)) {
    HMAX = hmax;
    hash = h;
    H = new LinkedList<struct elem_info<Tkey, Tvalue> > [HMAX];
  }

  /* Destructor */
  ~Hashtable() {
    for (int i = 0; i < HMAX; i++) {
      while (!H[i].isEmpty())
        H[i].removeFirst();
    }
    delete[] H;
  }

  /* Metoda pentru adaugarea unui jucator in hashtable */
  void put(Tkey key, Tvalue value, int echipa) {
    struct elem_info<Tkey, Tvalue> info;
    int hkey = hash(key);
    info.Nume = key;
    info.index_matrice = value;
    info.echipa = echipa;
    info.top_explorer = 0;
    info.best_shooter = 0;
    for (int j = 0; j < 100; j++) {
      info.senzori[j] = 0;
    }
    H[hkey].addLast(info);
  }

  /* Metoda folosita la inceputul unui nou joc */
  void initializare_joc(int numar_senzori) {
    struct Node<struct elem_info<Tkey, Tvalue> > *p;
    int numar_senzori_unici = 0;
    for (int i = 0; i < HMAX; i++) {
      p = H[i].front();
      while (p != NULL) {
        numar_senzori_unici = 0;
        p->value.senzor_anterior = numar_senzori;
        p->value.alive = 2;
        for (int j = 0; j < 100; j++) {
          if (p->value.senzori[j] == 1) {
            numar_senzori_unici++;  // Se calculeaza numarul de senzori unici
          }
          p->value.senzori[j] = 0;  // Se reseteaza vectorul de senzori unici
        }

        /* Se actualizeaza scorul top_explorer */
        p->value.top_explorer = p->value.top_explorer + 3 * numar_senzori_unici;
        p = p->next;
      }
    }
    p = NULL;
  }

  /* Metoda ce returneaza structura elem_info specifica unui jucator */
  elem_info<Tkey, Tvalue> get(Tkey key) {
    struct Node<struct elem_info<Tkey, Tvalue> > *p;
    int hkey = hash(key);
    p = H[hkey].front();
    while (p != NULL) {
      if (p->value.Nume == key) break;
      p = p->next;
    }
    if (p != NULL) {
      return p->value;
    } else {
      std::cerr << "Error 101 - The key does not exist" << std::endl;
      elem_info<Tkey, Tvalue> x;
      return x;
    }
  }

  /* Metoda ce adauga un senzor la lista de senzori unici,
     retine ultimul senzor si actualizeaza top_explorer */
  void set_senzor(int senzor, int distanta, Tkey key) {
    struct Node<struct elem_info<Tkey, Tvalue> > *p;
    int hkey = hash(key);
    p = H[hkey].front();
    while (p != NULL) {
      if (p->value.Nume == key) break;
      p = p->next;
    }

    // Se actualizeaza top_explorer */
    p->value.top_explorer = p->value.top_explorer + distanta;
    p->value.senzor_anterior = senzor;  // Se retine ultimul senzor
    p->value.senzori[senzor] = 1;  // Se actualizeaza lista de senzori unici
  }

  /* Metoda apelata la primirea comenzii END_CHAMPIONSHIP */
  void end_championship() {
    struct Node<struct elem_info<Tkey, Tvalue> > *p;
    int numar_senzori_unici = 0;
    for (int i = 0; i < HMAX; i++) {
      p = H[i].front();
      while (p != NULL) {
        numar_senzori_unici = 0;
        for (int j = 0; j < 100; j++) {
          // Se calculeaza numarul senzorilor unici
          if (p->value.senzori[j] == 1) {
            numar_senzori_unici++;
          }
          p->value.senzori[j] = 0;
        }

        // Se actualizeaza top_explorer
        p->value.top_explorer = p->value.top_explorer + 3 * numar_senzori_unici;
        p = p->next;
      }
    }
    p = NULL;
  }

  /* Metoda ce actualizeaza best_shooter al tragatorului si viata
     jucatorului impuscat */
  void best_shooter(Tkey key1, Tkey key2) {
    struct Node<struct elem_info<Tkey, Tvalue> > *p1;
    struct Node<struct elem_info<Tkey, Tvalue> > *p2;
    int hkey1 = hash(key1);
    p1 = H[hkey1].front();
    int hkey2 = hash(key2);
    p2 = H[hkey2].front();
    while (p1 != NULL) {
      if (p1->value.Nume == key1) break;
      p1 = p1->next;
    }
    while (p2 != NULL) {
      if (p2->value.Nume == key2) break;
      p2 = p2->next;
    }

    // Cazul cand jucatorii sunt din echipe diferite
    if (p1->value.echipa != p2->value.echipa) {
      p1->value.best_shooter = p1->value.best_shooter + 2;

    // Cazul cand jucatorii sunt din aceeasi echipa
    } else {
      p1->value.best_shooter = p1->value.best_shooter - 5;
    }

    // Se actualizeaza vietile jucatorului impuscat
    p2->value.alive--;
  }
};

#endif  // _HOME_STUDENT___RESOURCES___HASHTABLE_H_
