#pragma once

#define INALTIME        600     // Inaltime fereastra aplicatie
#define LATIME          1000    // Latime fereastra aplicatie
#define OBIECTE_MENIU   8       // Numar obiecte din bara de meniu
#define LATIME_SEP      3       // Latime separatori meniu
#define DIMENSIUNE      6       // Numar maxim de obiecte pe piesa

// Punct in plan definit de doua coordonate
struct Punct
{
    float x;    // Coordonata pe axa Ox
    float y;    // Coordonata pe axa Oy
};

// Coordonatele colturilor dreptunghiului ce inconjoara o piesa
struct Colt
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

// Structura de date ce retine obiectele(pozitie, dimensiune, culoare, etc)
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

// Structura de date pentru a memora coordonatele piesei si a facilita mutarea
// lor si salvarea fisierului
struct Graf
{
    Punct nod;          // Nod de legatura intre circuite
    Punct varf[10];     // Nodurile de legatura intre piesa si circuit
    Punct piesa;        // Coordonatele centrului piesei
    char tip;           // Tipul de piesa (D: dioda, N: poarta not, 1: nod intermediar etc)
};

// Citeste date din fisier si retine forma data cu ajutorul functiei iaCoord()
void citeste (FILE* file, Desen& piesa);

// Initializeaza fereastra cu bara de meniu si piese
void init (sf::RenderWindow& window);

// Ia coordonatele obiectelor din fisier in functie de tipul lor: linie, cerc, dreptunghi, triunghi
void iaCoord (char s[], Desen& piesa);

// Ia coordonatele nodurilor de legatura ale piesei
void iaVarfuri (char s[], Desen& piesa);

// Deseneaza piesa, afisand toate formele ce o alcatuiesc
void deseneazaPiesa (sf::RenderWindow& window, Desen piesa);

// Muta piesa in directia specificata
void muta (sf::RenderWindow& window, Desen& piesa, Punct pct);

// Ia coordonatele mijlocului piesei si le pastreaza in graf
void puneInGraf (sf::RenderWindow& window, char g[INALTIME][LATIME], Desen piesaCrt);

// Adauga element in graf
void adaugaElement (char graf[INALTIME][LATIME], Punct ini, Punct fin);

// (TODO) Salveaza circuitul facut intr-un fisier
void salveaza ();

// (TODO) Deschide un fisier unde a fost salvat un circuit
void deschide ();

// Afiseaza graful
void afiseazaGraf (char graf[INALTIME][LATIME]);

// Trage o linie intre doua puncte stabilite prin click
void trageLinii (sf::RenderWindow& window, Punct& t, sf::Vertex linie[][2], int& i, bool& pressed);