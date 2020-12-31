#pragma once

const int INALTIME      = 600;  // Inaltime fereastra aplicatie
const int LATIME        = 1000; // Latime fereastra aplicatie
const int OBIECTE_MENIU = 8;    // Numar obiecte din bara de meniu
const int LATIME_SEP    = 3;    // Latime separatori meniu
const int DIMENSIUNE    = 6;    // Numar maxim de obiecte diferite ce alcatuiesc o piesa (din fisier)
const int NR_PIESE      = 6;    // Numar maxim de piese pe tip

// Numele titlurilor din bara de meniuri
const char titluri[][11]= { "Fisiere", "Biblioteci", "Piese", "Legaturi", "Descriere", "Optiuni", "Zoom", "Ajutor" };

// Numele pieselor din toate fisierele
const char fisier[][NR_PIESE][12] = {
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
    Punct minim;    // Varful de jos stanga
    Punct maxim;    // Varful de sus dreapta
};

// Structura de date ce retine numarul componentelor unei piese
// (obiecte si varfuri)
struct NumarEle
{
    int cerc;       // Numarul de cercuri ce compun piesa
    int drept;      // Numarul de dreptunghiuri ce compun piesa
    int lin;        // Numarul de linii ce compun piesa
    int tri;        // Numarul de triunghiuri ce compun piesa
    int varfuri;    // Numarul de noduri de legatura ale piesei
};

// Structura de date ce retine obiectele (pozitie, dimensiune, culoare, etc)
// componente ale unei piese 
struct Desen
{
    sf::Vertex linie[DIMENSIUNE][2];            // Linie luata ca vectori de doua puncte
    sf::RectangleShape dreptunghi[DIMENSIUNE];  // Forma de dreptunghi
    sf::CircleShape cerc[DIMENSIUNE];           // Forma de cerc
    sf::ConvexShape triunghi[DIMENSIUNE];       // Triunghi luat ca poligon convex cu trei varfuri
    Punct varfuri[DIMENSIUNE];                  // Punctele de legatura ale piesei
    struct NumarEle numar;                      // Numarul de elemente
};

// (VA FI SCHIMBAT)
// Structura de date pentru a memora coordonatele piesei si a facilita mutarea
// lor si salvarea fisierului
struct Graf
{
    Punct nod;          // Nod de legatura intre circuite
    Punct varf[10];     // Nodurile de legatura intre piesa si circuit
    Punct piesa;        // Coordonatele centrului piesei
    char tip;           // Tipul de piesa (D: dioda, N: poarta not, 1: nod intermediar etc)
};

// Citeste date din fisier si retine forma data
// \param file Fisierul cu descrierea piesei
// \return Piesa rezultata din fisier
Desen citeste (FILE* file);

// Initializeaza fereastra cu bara de meniu si piese
// \param window Fereastra de lucru
void init (sf::RenderWindow& window, Desen piesaNoua[]);

// Ia coordonatele obiectelor din fisier in functie de tipul lor: 
// linie, cerc, dreptunghi, triunghi
// \param piesa Piesa curenta
// \param sir O linie
void iaCoord (Desen& piesa, char sir[]);

// Ia coordonatele nodurilor de legatura ale piesei
// \param piesa Piesa curenta
// \param sir O linie
void iaVarfuri (Desen& piesa, char sir[]);

// Deseneaza piesa, afisand toate formele ce o alcatuiesc
// \param window Fereastra de lucru
// \param piesa Piesa curenta
void deseneazaPiesa (sf::RenderWindow& window, Desen piesa);

// Muta piesa in directia specificata
// \param window Fereastra de lucru
// \param piesaCrt Piesa curenta
// \param poz Pozitie cursor in functie de fereastra
// \return Piesa noua la pozitia data
Desen muta (sf::RenderWindow& window, Desen& piesaCrt, sf::Vector2i poz);

// (TODO)
// Ia coordonatele mijlocului piesei si le pastreaza in graf
void puneInGraf (sf::RenderWindow& window, char g[INALTIME][LATIME], Desen piesaCrt);

// (TODO)
// Salveaza circuitul facut intr-un fisier
void salveaza ();

// (TODO)
// Deschide un fisier unde a fost salvat un circuit
void deschide ();

// Trage linii intre doua puncte
// \param t Punct provizoriu ce retine coordonatele primului click valid
// \param linie Liniile permanente intre noduri ce trebuie memorate
// \param nr Numarul de linii permanente pana intr-un punct
// \param pressed Daca s-a efectuat click pentru inceputul liniei
void trageLinii (sf::RenderWindow& window, Punct& t, sf::Vertex linie[][2], int& nr, bool& pressed);

// Verifica daca cursorul este intr-o anumita zona
// \param window Fereastra de lucru
// \param zona Dreptunghiul in care se testeaza 
// \return True daca cursorul este in zona
bool cursorInZona (sf::RenderWindow& window, Cadran zona);

// Deseneaza un dreptunghi ce palpaie in zona
// \param window Fereastra de lucru
// \param zona Dreptunghiul considerat zona inaccesibila
// \param viteza Viteza tranzitiei culorii
void zonaRosie (sf::RenderWindow& window, Cadran zona, float& viteza);

// Verifica daca piesa curenta exista, adunand numarul de obiecte componente
// \param piesaCrt Piesa curenta
// \return True daca piesa curenta exista
bool existaPiesa (Desen piesaCrt);