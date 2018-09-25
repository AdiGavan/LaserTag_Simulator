"Copyright [2017] <Gavan Adrian-George, 314CA>"

Nume, prenume: Gavan, Adrian-George

Grupa, seria: 314CA

Tema2: Laser Tag

Data inceperii temei: 22-04-2017

Data finalizarii temei: 28-04-2017

Probleme intampinate:
=====================

1. La sortari (pentru "Top fire exchange" mai ales):
- nu iesea din bucla;
- cazul cand nu era niciun jucator in vector;
- cazul cand erau mai putin de 5 jucatori;
2. La topul duelurilor, cand de exemplu ar fi fost Ana il impusca pe Mihai de 5
ori si Mihai o impusca pe Ana de 3 ori, iar numarul total de dueluri ar fi
reprezenta suma acestora, initial nu reuseam sa le adun corect.
3. La winning chance - cazul cu division by 0.
4. Initial din cauza listei mele se produceau memory leak-uri, care au fost
rezolvate ulterior.

Prezentarea implementarii:
==========================

A. Functii si medote folosite:
1. tema2.cc
- Functia de hash:
int hash(std::string key);
2. Hashtable.h
- Metoda pentru adaugarea unui jucator in Hashtable:
void put(Tkey key, Tvalue value, int echipa);
- Metoda folosita la inceputul unui nou joc:
void initializare_joc(int numar_senzori);
- Metoda ce returneaza structura elem_info specifica unui jucator:
elem_info<Tkey, Tvalue> get(Tkey key);
- Metoda ce adauga un senzor la lista de senzori unici, retine ultimul senzor si
actualizeaza top_explorer:
void set_senzor(int senzor, int distanta, Tkey key);
- Metoda apelata la primirea comenzii END_CHAMPIONSHIP:
void end_championship();
- Metoda ce actualizeaza best_shooter al tragatorului si viata jucatorului
impuscat:
void best_shooter(Tkey key1, Tkey key2);
3. List.h
- Metoda ce returneaza primul nod din lista:
Node<T> *front();
- Metoda ce adauga un nod cu valoarea == value la sfarsitul listei:
void addLast(T value);
- Metoda ce elimina elementul de la inceputul listei:
void removeFirst();
- Metoda ce intoarce true daca lista este vida, false altfel:
bool isEmpty();

B. Structuri si clase folosite:
- Structura folosita pentru top best_shooter si top_explorer:
struct Structura_Nume_Punctaj;
- Structura folosita pentru top_fire_exchange:
struct Structura_Duel;
- Structura valorii din fiecare element din lista din Hashtable:
template<typename Tkey, typename Tvalue> struct elem_info;
- Clasa Hashtable:
template<typename Tkey, typename Tvalue> class Hashtable;
- Structura unui nod din lista:
struct Node;
- Clasa lista:
class LinkedList;

C. Algoritmul utilizat:
- Se citeste numarul de senzori.
- Se adauga distantele dintre senzori in matrice_distanta (pe diagonala
principala o sa fie 0).
- Matricea de distanta are 1 linie si o coloana in plus cu 0-uri, pentru ca in
algoritm se initializeaza senzorul precedent ca fiind nr. total de senzori + 1
(de exemplu, pentru 10 senzori o sa se formeze o matrice de 11x11 cu ultima
linie si coloana 0-uri). Astfel la initializarea unui nou joc cand trece pe
langa primul senzor, distanta va fi 0.
- Se citeste numarul de jucatori al fiecarei echipe.
- Se adauga toti jucatorii in hashtable-ul "jucatori" si intr-un vector ce
stocheaza numele jucatorilor.
- Se creeaza un loop din care se va iesi la intalnirea comenzii
"END_CHAMPIONSHIP".
- Se interpreteaza comanda si se intra pe unul din cele 3 cazuri.
- Daca se incepe un joc nou, actualizam scorul echipelor (ne uitam la ultimul
tragator, echipa castigatoare fiind cea din care face parte) si apelam metoda
initializare_joc. Metoda reseteaza vectorul de senzori unici, senzorul
anterior, viata si actualizeaza top_explorer (metoda face aceste lucruri pentru
toti jucatorii).
- Daca comanda primita reprezinta deplasarea jucatorului pe langa un senzor,
se aplica metoda set_senzor care actualizeaza top_explorer, senzorul anterior
si senzorii unici ai jucatorului care trece pe langa senzor.
- Daca linia contine date furnizare de senzorii de impuscare, se actualizeaza
matricea duelurilor si se apeleaza metoda best_shooter, care actualizeaza
best_shooter-ul tragatorului si scade viata celui impuscat.
- Cand se termina campionatul, se aplica end_championship, care actualizeaza
pentru ultima oara top_explorerul fiecarui jucator, in functie de senzorii
unici prin care a trecut in jocul terminat fortat.
- In matricea sansa se stocheaza pentru toti jucatorii daca sunt "alive",
best_shooterul si top_explorerul fiecaruia (datele ne trebuie pentru calcularea
winning chance.
- Se incepe construirea topurilor.
- In cazul topurilor pentru "Top shooters" si "Top explorers", algoritmul de
sortare este acelasi. Se introduce primul jucator in vector. Dupa, pentru
fiecare jucator se calculeaza referinta (daca nu avem 5 jucatori in top
referinta va fi ultimul jucator din top, iar daca avem 5 jucatori sau mai
multi, referinta va fi jucatorul 5). Daca punctajul este mai mic decat al
referintei si sunt mai mult de 5 jucatori in topul intermediar nu se mai face
nicio verificare pentru ca sigur nu va fi in top. Jucatorul va fi comparat
cu toti jucatorii din vector, incepand cu primul, pana va fi inserat. Daca
jucatorul are punctaj mai mare decat jucatorul din vector cu care este comparat,
se va introduce in vector deasupra lui. Daca jucatorii au punctaj egal, se
compara numele acestora. Daca jucatorul ce trebuie adaugat are numele mai mic
din punct de vedere alfabetic, se va insera deasupra jucatorului din top. Daca
nu a fost inserat, se compara cu urmatorul jucator din top.
- Au fost prevazute si cazurile la "limita" (de exemplu daca nu sunt 5 oameni in
vector si jucatorul nu a indeplinit conditiile de inserare, acesta trebuie
adaugat la final).
- Pentru "Top fire exchange", cand citim jucatorii se vor stoca numele celor 2
jucatori din duel in ordine alfabetica. Apoi, algoritmul este acelasi, doar ca
apare un caz in plus. Daca si punctajul este la fel si primul jucator din duel
este acelasi in ambele dueluri, se compara si numele celorlalti 2 jucatori.
- Dupa ce avem vectorii sortati, se face afisarea.
- Problma este ca in vectorii sortati este posibil sa fie mai multi jucatori
decat ar trebui, asa ca se calculeaza referinta (dupa jucatorul 5 din top daca
sunt mai mult de 5 jucatori) si se afiseza jucatorii cu punctaje mai mari sau
egale cu punctajul de referinta.
- Se afiseaza "Top shooters", "Top explorers", "Top fire exchange".
- Se calculeaza winning chance al fiecarei echipe (se tine cont de cazurile
cand poate aprea "division by 0").
- Se actualizeaza scorul echipelor si se afiseaza scorul final.

Complexitatea temei: 9
======================

Alte precizari:
===============
- La coding style checker (ca si la prima tema) la fisierele *.h, unde trebuie
pus ifndef, checkerul ne cere sa punem calea absoluta din calculatorul nostru
pana la acel fisier. De la asistentul meu am inteles ca acolo poate fi pus
orice text, nu neaparat calea absoluta.
- Nu am utilizat grafuri pentru ca responsabilii temei au spus ca putem sa nu
folosim grafuri atata timp cat nu "pacalim" testele sau checkerul.
