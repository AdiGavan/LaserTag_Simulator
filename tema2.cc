// "Copyright [2017] <Gavan Adrian-George, 314CA>"
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "Hashtable.h"

#define VMAX 30

/* Structura folosita pentru top best_shooter si top_explorer */
struct Structura_Nume_Punctaj {
  std::string Nume;
  int punctaj;
};

/* Structura folosita pentru top_fire_exchange */
struct Structura_Duel {
  int punctaj;
  std::string Nume1;
  std::string Nume2;
};

/* Functia de hash */
int hash(std::string key) {
  int hkey = 0;
  hkey = (int)(key[0] - 'A');
  return hkey;
}

int main() {
  /* info_jucator - se vor retine informatiile despre jucator aduse din
                    Hashtable
     jucatori - Hashtable
     numar_senzori - numar total de senzori
     nr_jucatori_echipa1,2 - numarul jucatorilor din echipa 1, 2
     indice_matrice - cu ajutorul ei se va face legatura dintre numele
                      jucatorilor si indicii matricii
     scor_echipe - vector in care se retine scorul echipelor
     nume - se va retine numele unui jucator de la input
     actiune - se va retine o informatie de la input
     nume_tragator - se va retine numele tragatorului
     nume_impuscat - se va retine numele celui impuscat
     matrice_distanta - se retin distantele dintre senzori
  */

  elem_info<std::string, int> info_jucator;
  Hashtable<std::string, int> jucatori = Hashtable<std::string, int>
                                        (VMAX, hash);
  int numar_senzori, i, j;
  int nr_jucatori_echipa1;
  int nr_jucatori_echipa2;
  int indice_matrice = 0;
  int senzor;
  int scor_echipe[2] = {};
  std::string nume;
  std::string actiune;
  std::string nume_tragator, nume_impuscat;
  std::cin >> numar_senzori;
  int matrice_distanta[numar_senzori + 1][numar_senzori + 1] = {};

  // Initializarea matricea de distante
  for (i = 0; i < numar_senzori; i++) {
    for (j = 0; j < numar_senzori; j++) {
      if (i == j) {
        matrice_distanta[i][j] = 0;
        continue;
      } else {
        std::cin >> matrice_distanta[i][j];
      }
    }
  }

  // Se citeste numarul membrilor din echipe
  std::cin >> nr_jucatori_echipa1;
  std::cin >> nr_jucatori_echipa2;

  // kTotalJucatori retine numarul total de jucatori
  int kTotalJucatori = nr_jucatori_echipa1 + nr_jucatori_echipa2;

  // Matrice folosita la calculul pentru dueluri
  // Se retine de cate ori un jucator a impuscat un anume jucator
  int matrice_duel[kTotalJucatori][kTotalJucatori] = {};

  // Vectorul in care se stocheaza numele jucatorilor
  std::string nume_jucatori[kTotalJucatori];

  // Se adauga jucatorul in Hashtable si in nume_jucatori
  // Pentru jucatorii din echipa 1
  for (i = 0; i < nr_jucatori_echipa1; i++) {
    std::cin >> nume;
    jucatori.put(nume, indice_matrice, 1);
    nume_jucatori[indice_matrice] = nume;
    indice_matrice++;
  }

  // Pentru jucatorii din echipa 2
  for (j = 0; j < nr_jucatori_echipa2; j++) {
    std::cin >> nume;
    jucatori.put(nume, indice_matrice, 2);
    nume_jucatori[indice_matrice] = nume;
    indice_matrice++;
  }

  // Se citesc comenzi de la input pana la intalnirea "END_CHAMPIONSHIP"
  std::cin >> actiune;
  while (actiune != "END_CHAMPIONSHIP") {
    // Linie cu joc
    if (actiune.substr(0, 4) == "JOC_") {
      // Se actualizeaza scorul echipelor
      if (nume_tragator != "") {
        scor_echipe[jucatori.get(nume_tragator).echipa - 1]++;
      }

      // Se initializeaza un nou joc
      jucatori.initializare_joc(numar_senzori);


      // Linie cu senzori
    } else if ('0' <= actiune[0] && actiune[0] <= '9') {
      // Se transforma numarul senzorului din caracter in intreg
      senzor = atoi(actiune.c_str());

      // Se citeste numele de dupa senzor
      std::cin >> nume;
      jucatori.set_senzor(senzor, matrice_distanta[senzor]
                          [jucatori.get(nume).senzor_anterior], nume);

      // Linie cu trageri
    } else if ('A' <= actiune[0] && actiune[0] <= 'Z' &&
               'a' <= actiune[1] && actiune[1] <= 'z') {
      nume_tragator = actiune;
      std::cin >> actiune;  // Sa treaca peste "->" ;
      std::cin >> nume_impuscat;  // Se citeste numele celui impuscat

      // Se actualizeaza in matricea duelurilor
      matrice_duel[jucatori.get(nume_tragator).index_matrice][jucatori.get(
            nume_impuscat).index_matrice]++;

      // Se actualizeaza datele jucatorilor
      jucatori.best_shooter(nume_tragator, nume_impuscat);
    }
    std::cin >> actiune;
  }

  // Dupa ce se primeste comanda END_CHAMPIONSHIP se apeleaza Metoda
  // end_championship din hashtable
  jucatori.end_championship();

  // Se initializeaza matricea in care se retin datele folositoare
  // pentru calcularea winning chance
  // In 0 e alive, in 1 e best shooter si in 2 e top exlorer pt. fiecare jucator
  int sansa[kTotalJucatori][3] = {};

  // Vectorii in care se vor insera elemente in ordine descrescatoare
  // pentru best shooter si explorers
  std::vector<Structura_Nume_Punctaj> top_best_shooter;
  std::vector<Structura_Nume_Punctaj> top_best_explorer;

  // Contor pentru best_shooter
  unsigned int contor_shooter = 0;

  // Nume_Punctaj retine numele jucatorului si best_shooter-ul lui
  Structura_Nume_Punctaj Nume_Punctaj;

  // Nume_Punctaj_Explorer retine numele jucatorului si top_explorer-ului lui
  Structura_Nume_Punctaj Nume_Punctaj_Explorer;

  // Pentru o sortare eficienta, se vor compara punctajele structurilor ce
  // trebuie inserate in vector cu o anumita referinta, dupa caz.
  // Referinta va fi cea mai mica valoare din top 5.
  int punctaj_referinta;

  // Se insereaza primul jucator
  // Se iau datele din hashtable
  info_jucator = jucatori.get(nume_jucatori[0]);

  // Se actualizeaza matricea pentru winning chance
  if (info_jucator.alive != 0) {
    sansa[0][0] = 1;
  }
  sansa[0][1] = info_jucator.best_shooter;
  sansa[0][2] = info_jucator.top_explorer;
  Nume_Punctaj.Nume = info_jucator.Nume;
  Nume_Punctaj.punctaj = info_jucator.best_shooter;

  // Se insereaza in vectorul pentru best shooter
  top_best_shooter.insert(top_best_shooter.begin(), Nume_Punctaj);
  Nume_Punctaj_Explorer.Nume = info_jucator.Nume;
  Nume_Punctaj_Explorer.punctaj = info_jucator.top_explorer;

  // Se insereaza in vectorul pentru top explorer
  top_best_explorer.insert(top_best_explorer.begin(), Nume_Punctaj_Explorer);

  for (i = 1; i < kTotalJucatori; i++) {
    contor_shooter = 0;

    // Se aduc datele jucatorului
    info_jucator = jucatori.get(nume_jucatori[i]);
    if (info_jucator.alive != 0) {
      sansa[i][0] = 1;
    }
    sansa[i][1] = info_jucator.best_shooter;
    sansa[i][2] = info_jucator.top_explorer;
    Nume_Punctaj.Nume = info_jucator.Nume;
    Nume_Punctaj.punctaj = info_jucator.best_shooter;

    // Se calculeaza punctajul de referinta (daca nu exista un top 5 sau
    // exista deja 5 valori in top)
    if (top_best_shooter.size() <= 5) {
      punctaj_referinta = top_best_shooter[top_best_shooter.size() - 1].punctaj;
    } else {
      punctaj_referinta = top_best_shooter[4].punctaj;
    }

    // Daca sunt 5 jucatori in top si punctajul este mai mic decat referinta,
    // nu se mai insereaza in vector
    if (Nume_Punctaj.punctaj >= punctaj_referinta ||
        top_best_shooter.size() <= 5) {
      while (contor_shooter < top_best_shooter.size()) {
        // Cazul cand jucatorul ce se adauga are punctaj mai mare decat
        // jucatorul cu care se compara
        if (Nume_Punctaj.punctaj > top_best_shooter[contor_shooter].punctaj) {
          top_best_shooter.insert(top_best_shooter.begin() + contor_shooter,
                                  Nume_Punctaj);
          break;

          // Cazul cand au punctajele egale
        } else if (Nume_Punctaj.punctaj ==
                   top_best_shooter[contor_shooter].punctaj) {
          // Se compara numele celor 2 jucatori si daca numele jucatorului nou
          // este mai mic "alfabetic" decat jucatorul cu care este comparat,
          // se insereaza in vector
          if (Nume_Punctaj.Nume < top_best_shooter[contor_shooter].Nume) {
            top_best_shooter.insert(top_best_shooter.begin() + contor_shooter,
                                    Nume_Punctaj);
            break;

            // Cazul cand sunt mai putin de 5 elemente si punctajul este
            // egal cu al ultimului si numele nu este mai mic "alfabetic",
            // se insereaza la final
            // De asemenea, trateaza si cazul cand punctajul jucatorului
            // este egal cu referinta dar nu are numele mai mic
            // alfabetic si trebuie adaugat la final
          } else if (contor_shooter == top_best_shooter.size() - 1) {
            top_best_shooter.push_back(Nume_Punctaj);
            break;

            // Se incrementeaza si se trece la urmatorul element cu care
            // trebuie verificat
          } else {
            contor_shooter++;
          }

          // Cazul cand sunt mai putin de 5 elemente si nu este mai mare
          // decat ultimul dintre acestea, se insereaza la final
        } else if (contor_shooter == top_best_shooter.size() - 1) {
          top_best_shooter.push_back(Nume_Punctaj);
          break;

          // Se incrementeaza si se trece la urmatorul element cu care
          // trebuie verificat
        } else {
          contor_shooter++;
        }
      }
    }

    // Contor pentru best_shooter
    unsigned int contor_explorer  = 0;
    Nume_Punctaj_Explorer.Nume = info_jucator.Nume;
    Nume_Punctaj_Explorer.punctaj = info_jucator.top_explorer;

    // Se calculeaza punctajul de referinta (daca nu exista un top 5 sau
    // exista deja 5 valori in top)
    if (top_best_explorer.size() <= 5) {
      punctaj_referinta =
                        top_best_explorer[top_best_explorer.size() - 1].punctaj;
    } else {
      punctaj_referinta = top_best_explorer[4].punctaj;
    }

    // Daca sunt 5 jucatori in top si punctajul este mai mic decat referinta,
    // nu se mai insereaza in vector
    if (Nume_Punctaj_Explorer.punctaj >= punctaj_referinta
        || top_best_explorer.size() <= 5) {
      while (contor_explorer < top_best_explorer.size()) {
        // Cazul cand jucatorul ce se adauga are punctaj mai mare decat
        // jucatorul cu care se compara
        if (Nume_Punctaj_Explorer.punctaj >
            top_best_explorer[contor_explorer].punctaj) {
          top_best_explorer.insert(top_best_explorer.begin() + contor_explorer,
                                   Nume_Punctaj_Explorer);
          break;

          // Cazul cand au punctajele egale
        } else if (Nume_Punctaj_Explorer.punctaj ==
                   top_best_explorer[contor_explorer].punctaj) {
          // Se compara numele celor 2 jucatori si daca numele jucatorului nou
          // este mai mic "alfabetic" decat jucatorul cu care este comparat,
          // se insereaza in vector
          if (Nume_Punctaj_Explorer.Nume <
              top_best_explorer[contor_explorer].Nume) {
            top_best_explorer.insert(top_best_explorer.begin() +
                                     contor_explorer, Nume_Punctaj_Explorer);
            break;

            // Cazul cand sunt mai putin de 5 elemente si punctajul este
            // egal cu al ultimului si numele nu este mai mic "alfabetic",
            // se insereaza la final
            // De asemenea, trateaza si cazul cand punctajul jucatorului
            // este egal cu referinta dar nu are numele mai mic
            // alfabetic si trebuie adaugat la final
          } else if (contor_explorer == top_best_explorer.size() - 1) {
            top_best_explorer.push_back(Nume_Punctaj_Explorer);
            break;

            // Se incrementeaza si se trece la urmatorul element cu care
            // trebuie verificat
          } else {
            contor_explorer++;
          }

          // Cazul cand sunt mai putin de 5 elemente si nu este mai mare
          // decat ultimul dintre acestea, se insereaza la final
        } else if (contor_explorer == top_best_explorer.size() - 1) {
          top_best_explorer.push_back(Nume_Punctaj_Explorer);
          break;

          // Se incrementeaza si se trece la urmatorul element cu care
          // trebuie verificat
        } else {
          contor_explorer++;
        }
      }
    }
  }

  // Vectorul in care se insereaza elemente in ordine descrescatoare
  // pentru top fire exchange
  std::vector<Structura_Duel> top_dueluri;
  Structura_Duel duel;

  // Contor pentru vectorul duelurilor
  unsigned int contor_duel = 0;
  int duel_referinta = 0;

  // Se iau datele primului duel (din matrice_duel si din vectorul cu numele
  // jucatorilor) si numele se retin alfabetic, apoi se insereaza in vectorul
  // pentru topul duelurilor
  duel.punctaj = matrice_duel[1][0] + matrice_duel[0][1];
  if (nume_jucatori[1] < nume_jucatori[0]) {
    duel.Nume1 = nume_jucatori[1];
    duel.Nume2 = nume_jucatori[0];
  } else {
    duel.Nume1 = nume_jucatori[0];
    duel.Nume2 = nume_jucatori[1];
  }
  top_dueluri.insert(top_dueluri.begin(), duel);
  for (i = 2; i < kTotalJucatori; i++) {
    for (j = 0; j < i; j++) {
      contor_duel = 0;

      // Se iau datele noului duel
      duel.punctaj = matrice_duel[i][j] + matrice_duel[j][i];
      if (nume_jucatori[i] < nume_jucatori[j]) {
        duel.Nume1 = nume_jucatori[i];
        duel.Nume2 = nume_jucatori[j];
      } else {
        duel.Nume1 = nume_jucatori[j];
        duel.Nume2 = nume_jucatori[i];
      }
      // Se calculeaza punctajul de referinta (daca nu exista un top 5 sau
      // exista deja 5 valori in top)
      if (top_dueluri.size() <= 5) {
        duel_referinta = top_dueluri[top_dueluri.size() - 1].punctaj;
      } else {
        duel_referinta = top_dueluri[4].punctaj;
      }

      // Daca sunt 5 dueluri in top si punctajul este mai mic decat referinta,
      // nu se mai insereaza in vector
      if (duel.punctaj >= duel_referinta || top_dueluri.size() <= 5) {
        while (contor_duel < top_dueluri.size()) {
          // Cazul cand duelul ce se adauga are punctaj mai mare decat
          // duelul cu care se compara
          if (duel.punctaj > top_dueluri[contor_duel].punctaj) {
            top_dueluri.insert(top_dueluri.begin() + contor_duel, duel);
            break;

            // Cazul cand au punctajele egale
          } else if (duel.punctaj == top_dueluri[contor_duel].punctaj) {
            // Se compara primul nume al fiecarui jucator dintre cele 2 dueluri
            // si daca numele jucatorului din duelul nou este mai mic
            // "alfabetic" decat jucatorul cu care este comparat, se insereaza
            // in vector
            if (duel.Nume1 < top_dueluri[contor_duel].Nume1) {
              top_dueluri.insert(top_dueluri.begin() + contor_duel, duel);
              break;

              // Cazul cand si primele nume sunt aceleasi
            } else if (duel.Nume1 == top_dueluri[contor_duel].Nume1) {
              // Se compara al doilea nume al fiecarui jucator dintre cele 2
              // dueluri si daca numele jucatorului din duelul nou este mai mic
              // "alfabetic" decat jucatorul cu care este comparat, se insereaza
              // in vector
              if (duel.Nume2 < top_dueluri[contor_duel].Nume2) {
                top_dueluri.insert(top_dueluri.begin() + contor_duel, duel);
                break;
              } else if (contor_duel == top_dueluri.size() - 1) {
                top_dueluri.push_back(duel);
                break;
              } else {
                contor_duel++;
              }
            } else if (contor_duel == top_dueluri.size() - 1) {
              top_dueluri.push_back(duel);
              break;
            } else {
              contor_duel++;
            }
          } else if (contor_duel == top_dueluri.size() - 1) {
            top_dueluri.push_back(duel);
            break;
          } else {
            contor_duel++;
          }
        }
      }
    }
  }

  // Se afiseaza top shooters
  if (top_best_shooter.size() <= 5) {
    punctaj_referinta = top_best_shooter[top_best_shooter.size() - 1].punctaj;
  } else {
    punctaj_referinta = top_best_shooter[4].punctaj;
  }
  unsigned int contor_afisare = 0;
  std::cout << "I. Top shooters" << std::endl;
  while (top_best_shooter[contor_afisare].punctaj >= punctaj_referinta) {
    std::cout << contor_afisare + 1 << ". "
              << top_best_shooter[contor_afisare].Nume << " "
              << top_best_shooter[contor_afisare].punctaj << "p" << std::endl;
    contor_afisare++;
    if (contor_afisare >= top_best_shooter.size()) {
      break;
    }
  }
  std::cout << std::endl;

  // Se afiseaza top explorers
  if (top_best_explorer.size() <= 5) {
    punctaj_referinta = top_best_explorer[top_best_explorer.size() - 1].punctaj;
  } else {
    punctaj_referinta = top_best_explorer[4].punctaj;
  }
  contor_afisare = 0;
  std::cout << "II. Top explorers" << std::endl;
  while (top_best_explorer[contor_afisare].punctaj >= punctaj_referinta) {
    std::cout << contor_afisare + 1 << ". "
              << top_best_explorer[contor_afisare].Nume << " "
              << top_best_explorer[contor_afisare].punctaj << "p" << std::endl;
    contor_afisare++;
    if (contor_afisare >= top_best_explorer.size()) {
      break;
    }
  }
  std::cout << std::endl;

  // Se afiseaza top fire exchange
  contor_afisare = 0;
  if (top_dueluri.size() <= 5) {
    duel_referinta = top_dueluri[top_dueluri.size() - 1].punctaj;
  } else {
    duel_referinta = top_dueluri[4].punctaj;
  }
  std::cout << "III. Top fire exchange" << std::endl;
  while (top_dueluri[contor_afisare].punctaj >= duel_referinta) {
    std::cout << contor_afisare + 1 << ". "
              << top_dueluri[contor_afisare].Nume1 << " - "
              << top_dueluri[contor_afisare].Nume2 << " "
              << top_dueluri[contor_afisare].punctaj << std::endl;
    contor_afisare++;
    if (contor_afisare >= top_dueluri.size()) {
      break;
    }
  }

  // Se calculeaza winning chance al fiecarei echipe
  float winning_echipa1 = 0, winning_echipa2 = 0;
  if (numar_senzori == 0) {
    for (i = 0; i < nr_jucatori_echipa1; i++) {
      winning_echipa1 = winning_echipa1 + sansa[i][0] * ((float)sansa[i][1] /
                        top_best_shooter[0].punctaj);
    }
    for (i = nr_jucatori_echipa1; i < kTotalJucatori;
         i++) {
      winning_echipa2 = winning_echipa2 + sansa[i][0] * ((float)sansa[i][1] /
                        top_best_shooter[0].punctaj);
    }
  } else {
    for (i = 0; i < nr_jucatori_echipa1; i++) {
      winning_echipa1 = winning_echipa1 + sansa[i][0] * ((float)sansa[i][1] /
                        top_best_shooter[0].punctaj + ((float)sansa[i][2] /
                            top_best_explorer[0].punctaj));
    }
    for (i = nr_jucatori_echipa1; i < kTotalJucatori;
         i++) {
      winning_echipa2 = winning_echipa2 + sansa[i][0] * ((float)sansa[i][1] /
                        top_best_shooter[0].punctaj + ((float)sansa[i][2] /
                            top_best_explorer[0].punctaj));
    }
  }

  // Se actualizeaza scorul
  if (winning_echipa1 > winning_echipa2) {
    scor_echipe[0]++;
  } else {
    scor_echipe[1]++;
  }

  // Se afiseaza scorul
  std::cout << std::endl;
  std::cout << "IV. Final score" << std::endl;
  std::cout << scor_echipe[0] << " - " << scor_echipe[1] << std::endl;
  return 0;
}
