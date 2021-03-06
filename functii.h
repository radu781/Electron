#pragma once

const int INALTIME      = 600;    // Inaltime fereastra aplicatie
const int LATIME        = 1000;   // Latime fereastra aplicatie
const int NR_MENIU      = 3;      // Numar titluri din meniul principal
const int NR_AJUTOR     = 6;      // Numar titluri din meniul "Ajutor"
const int LATIME_SEP    = 5;      // Latime separatori meniu
const int DIMENSIUNE    = 22;     // Numar maxim de obiecte diferite ce alcatuiesc o piesa (din fisier)
const int NR_PIESE      = 6;      // Numar maxim de piese pe tip (logice, simple, complexe)

#define VERDE1      Color ( 25, 102,  25, 255)  // Folosita la trasarea legaturilor
#define GALBEN1     Color (146, 146,  58, 255)  // Folosita la trasarea legaturilor
#define ROSU1       Color (122,  31,  31, 255)  // Folosita la trasarea legaturilor
#define ROSU2       Color (252,  68,  69, 255)  // Folosita la separatorii din meniu
#define ROZ1        Color (179,   0, 255, 255)  // Folosita la piese
#define ALBASTRU1   Color (202, 250, 254, 255)  // Folosita la bara de meniu
#define ALBASTRU2   Color (151, 202, 239, 255)  // Folosita la bara de submeniu

// Numele titlurilor din bara de meniuri
const char NUME_TITLURI[NR_MENIU][11] = { "Piese", "Legaturi", "Ajutor" };

// Numele titlurilor din submeniul de ajutor
const char NUME_AJUTOR[NR_AJUTOR][20] = { "Plasare piese", "Plasare circuit", "Salvare circuit", "Deschidere", "Zoom", "Descriere piese" };

// Numele pieselor din toate fisierele
const char NUME_FISIERE[][NR_PIESE][12] = {
    { "Logice", "Simple", "Complexe" },
    { "and", "or", "nand", "nor", "not", "xor" },
    { "baterie", "dioda", "rezistor", "sursa", "termistor" },
    { "decodor", "memorie", "multiplexor" } };

// Punct in plan definit de doua coordonate
struct Punct
{
    float x;    // Coordonata pe axa Ox
    float y;    // Coordonata pe axa Oy
};

// Coordonatele colturilor dreptunghiului ce inconjoara o piesa
struct Cadran
{
    Punct minim;    // Varful de sus stanga
    Punct maxim;    // Varful de jos dreapta
};

// Structura de date ce retine numarul componentelor unei piese
// (obiecte si varfuri)
struct NumarEle
{
    short int cerc;     // Numarul de cercuri ce compun piesa
    short int drept;    // Numarul de dreptunghiuri ce compun piesa
    short int lin;      // Numarul de linii ce compun piesa
    short int tri;      // Numarul de triunghiuri ce compun piesa
    short int varfuri;  // Numarul de noduri de legatura ale piesei
};

// Structura de date ce retine obiectele (pozitie, dimensiune, culoare, etc)
// componente ale unei piese 
struct Desen
{
    sf::Vertex linie[DIMENSIUNE][2];            // Linie considerata vector de doua puncte
    sf::RectangleShape dreptunghi[DIMENSIUNE];  // Forma de dreptunghi
    sf::CircleShape cerc[DIMENSIUNE];           // Forma de cerc
    sf::ConvexShape triunghi[DIMENSIUNE];       // Triunghi considerat poligon convex cu trei varfuri
    Punct varfuri[DIMENSIUNE];                  // Punctele de legatura ale piesei
    struct NumarEle numar;                      // Numarul de elemente
    char id[4];                                 // Identificatorul piesei (DIO: dioda, NOT: poarta not, NAN: poarta logica nand, etc)
};

// Date despre o piesa
struct Lista
{
    Punct coord;        // Coordonatele unei piese
    Lista* urm;         // Pointer la piesa urmatoare
    char id[4];         // Tipul de piesa (DIO: dioda, NOT: poarta not, NAN: poarta logica nand, etc)
};

// Date despre un nod
struct Nod
{
    Punct coord;        // Coordonatele unui nod de legatura
    Nod* drp;           // Pointer catre noduri adiacente
    Nod* jos;           // Pointer catre urmatorul nod (nu neaparat adiacent)
};

// Insereaza o piesa intr-o lista simpla inlantuita
// \param listaCrt Lista curenta in care se insereaza
// \param capLista Capul listei
// \param coadaLista Coada listei
// \param coord Coordonatele unde va fi pusa piesa
// \param id Tipul piesei
void insereazaLista (Lista*& listaCrt, Lista*& capLista, Lista*& coadaLista, Punct coord, char id[]);

// Afiseaza lista inlantuita
// \param listaCrt Lista curenta ce va fi afisata
// \param capLista Capul listei
void afiseazaLista (Lista* listaCrt, Lista* capLista);

// Schimba valoarea unui element din lista, tipul piesei ramane acelasi
// \param listaCrt Lista curenta
// \param capLista Capul listei
// \param vechi Punctul ce trebuie modificat
// \param nou Valoarea cu care se modifica
void mutaLista (Lista*& listaCrt, Lista* capLista, Punct vechi, Punct nou);

// Insereaza element in graf
// \param grafCrt Graf curent
// \param capGraf Varful/punctul initial din graf
// \param srs Punct initial pentru legatura
// \param dest Punct final pentru legatura
void insereazaGraf (Nod*& grafCrt, Nod*& capGraf, Punct srs, Punct dest);

// Afiseaza graful
// \param grafCrt Graf curent ce va fi afisat
// \param capGraf Varful/punctul initial din graf
void afiseazaGraf (Nod* grafCrt, Nod* capGraf);

// Schimba valoarea unui element din graf
// \param grafCrt Graf curent
// \param capGraf Varful/punctul initial din graf
// \param vechi Punctul ce trebuie modificat
// \param nou Punctul cu care se modifica
void mutaGraf (Nod* grafCrt, Nod* capGraf, Punct vechi, Punct nou);

// Salveaza circuitul creat intr-un fisier, numele e specificat de utilizator
// la rularea programului
// \param grafCrt Graful care se salveaza
// \param capGraf Varful/punctul initial din graful ce va fi salvat
// \param listaCrt Lista inlantuita care se salveaza
// \param capLista Varful listei
// \param text Numele cu care va fi salvat fisierul
void salveaza (Nod* grafCrt, Nod* capGraf, Lista* listaCrt, Lista* capLista, char text[]);

// Deschide un fisier unde a fost salvat un circuit si preia graful si lista
// \param grafCrt Graful in care sunt memorate datele despre graf din fisier
// \param capGraf Varful/punctul initial din graf
// \param listaCrt Lista inlantuita in care sunt memorate datele despre lista din fisier
// \param capLista Varful listei
// \param coadaLista Coada listei
// \param text Numele fisierului care va fi deschis
void deschide (Nod*& grafCrt, Nod*& capGraf, Lista*& listaCrt, Lista*& capLista, Lista*& coadaLista, char text[]);

// Restituie datele din fisier
// \param window Fereastra de lucru
// \param grafCrt Graful curent in care se restituie datele referitoare la legaturi din fisier
// \param capGraf Radacina grafului
// \param listaCrt Lista curenta in care se restituie datele referitoate la piese din fisier
// \param coadaLista
// \param piesaPerm Piesa permanenta care ajuta la desenarea pieselor mutate de utilizator
// \param piesaGata Piesa mutata in functe de datele din fisier 
// \param linie Liniile de legatura ce au fost trasate
// \param totalLinii Numarul total de linii/legaturi ce trebuie desenate
void restituie (sf::RenderWindow& window, Nod* grafCrt, Nod* capGraf, Lista* listaCrt, Lista* capLista, Lista* coadaLista, Desen piesaPerm[], Desen piesaGata[], sf::Vertex linie[][2], int& totalLinii);

// Citeste date din fisier
// \param file Fisierul cu descrierea piesei
// \return Piesa rezultata din datele citite din fisier
Desen citeste (FILE* file);

// Initializeaza fereastra cu bara de meniu si piese
// \param window Fereastra de lucru
void init (sf::RenderWindow& window);

// Ia coordonatele obiectelor din fisier in functie de tipul lor: 
// linie, cerc, dreptunghi, triunghi
// \param piesa Piesa curenta in care vor fi salvate obiectele
// \param sir O linie din fisier
void iaCoord (Desen& piesa, char sir[]);

// Ia coordonatele nodurilor de legatura ale piesei
// \param piesa Piesa curenta in care vor fi salvate varfurile
// \param sir O linie din fisier
void iaVarfuri (Desen& piesa, char sir[]);

// Deseneaza piesa, afisand toate formele ce o alcatuiesc
// \param window Fereastra de lucru
// \param piesa Piesa curenta ce trebuie desenata
void deseneazaPiesa (sf::RenderWindow& window, Desen piesa);

// Muta piesa pe directia specificata (centrat pe mouse)
// \param window Fereastra de lucru
// \param piesaCrt Piesa curenta/permanenta
// \param poz Coordonate noi pentru piesa (pozitie cursor in functie de fereastra)
// \return Piesa noua la pozitia indicata de cursor
Desen muta (sf::RenderWindow& window, Desen& piesaCrt, sf::Vector2i poz);

// Calculeaza dimensiunea piesei, gasind coordonatele x si y minime si maxime
// \param piesaCrt Piesa curenta
// \return Cadran cu dreptunghiul cel mai mic ce acopera toata piesa
Cadran limitePiesa (Desen piesaCrt);

// Trage linii intre doua puncte
// \param window Fereastra de lucru
// \param event Evenimentul care este testat
// \param piesaPerm Piesele permanente din fisiere
// \return Cadran cu coordonatele extremitatilor liniei
Cadran trageLinii (sf::RenderWindow& window, sf::Event event, Desen piesaPerm[]);

// Verifica daca cursorul este intr-o anumita zona
// \param window Fereastra de lucru
// \param zona Dreptunghiul in care se testeaza 
// \return True daca cursorul este in zona, false daca nu este
bool cursorInZona (sf::RenderWindow& window, Cadran zona);

// Deseneaza un dreptunghi ce palpaie in zona
// \param window Fereastra de lucru
// \param zona Dreptunghiul considerat zona inaccesibila
void zonaRosie (sf::RenderWindow& window, Cadran zona);

// Verifica daca piesa curenta exista, adunand numarul de obiecte componente
// \param piesaCrt Piesa curenta
// \return True daca piesa curenta exista, false daca nu
bool existaPiesa (Desen piesaCrt);

// Creeaza numele fisierului necesar citirii
// \param linie Tipul piesei (1: logica, 2: simpla, 3: complexa)
// \param coloana Indicele piesei de pe linie
// \return Numele fisierului creat
char* numeFisier (int linie, int coloana);

// Pune in lista specificata coordonatele si identificatorul piesei
// \param listaCrt Lista curenta in care se adauga elemente
// \param capLista Capul listei curente
// \param coadaLista Coada listei curente
// \param piesaCrt Piesa curenta
// \param id Identificator piesa
void puneInLista (Lista*& listaCrt, Lista*& capLista, Lista*& coadaLista, Desen piesaCrt, char id[]);

// Supraincarcare de operatori, folositor la functiile pentru lista si graf
bool operator== (Punct a, Punct b);
bool operator!= (Punct a, Punct b);
bool operator== (Cadran a, Cadran b);
bool operator!= (Cadran a, Cadran b);
bool operator>= (Punct a, Punct b);
bool operator<= (Punct a, Punct b);