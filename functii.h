#pragma once

#define INALTIME        600
#define LATIME          1000
#define OBIECTE_MENIU   8
#define LATIME_SEP      3
#define DIMENSIUNE      6

// Punct definit de o coordonata x si una y
struct punct
{
    float x, y;
};

// Coordonatele colturilor dreptunghiului ce inconjoara o piesa
struct colt
{
    punct minim, maxim;
};

// Contor pentru numarul de obiecte ce constituie fiecare piesa
struct numarPiese
{
    int lin, drept, cerc, tri;
};

// Structura de date ce retine numarul componentelor unei piese
struct numarEle
{
    numarPiese piese;   // numarul de obiecte ce compun piesa
    int varfuri;        // numarul de noduri de legatura
};

// Structura de date ce retine forma unei piese (alcatuita din linii, dreptunghiuri, cercuri si triunghuri)
struct desen
{
    sf::Vertex linie[DIMENSIUNE][2];            // linii luate ca vectori de doua puncte
    sf::RectangleShape dreptunghi[DIMENSIUNE];  // 
    sf::CircleShape cerc[DIMENSIUNE];           // 
    sf::ConvexShape triunghi[DIMENSIUNE];       // triunghi ca poligon convex cu trei varfuri
    punct varfuri[DIMENSIUNE];                  // punctele de legatura ale piesei
    struct numarEle numar;                      // numarul de elemente
};

// Structura de date pentru a memora coordonatele piesei si a facilita mutarea lor si salvarea fisierului
struct graf
{
    punct nod;          // nod de legatura intre circuite
    punct varf[10];     // nodurile de legatura intre piesa si circuit
    punct piesa;        // coordonatele centrului piesei
    char tip;           // tipul de piesa (D: dioda, N: poarta not etc)
};

// Citeste date din fisier, considera liniile ce incep cu '#' ca fiind comentarii si retine forma data cu ajutorul functiei iaCoord()
void citeste (FILE* file, desen& piesa);

// Initializeaza fereastra cu bara de meniu si piese
void init (sf::RenderWindow& window, sf::RectangleShape baraMeniu, sf::RectangleShape baraParti, sf::RectangleShape separatori[], sf::Text titluri[]);

// Ia coordonatele obiectelor din fisier in functie de tipul lor: linie, cerc, dreptunghi, triunghi
void iaCoord (char s[], desen& piesa);

// Ia coordonatele nodurilor de legatura ale piesei
void iaVarfuri (char s[], desen& piesa);

// Deseneaza piesa, afisand toate formele ce o alcatuiesc
void deseneazaPiesa (sf::RenderWindow& window, desen piesa);

// Muta piesa in directia specificata
void muta (sf::RenderWindow& window, desen& piesa, punct pct);

// Ia coordonatele mijlocului piesei si (TODO) le pastreaza in graf
void puneInGraf (sf::RenderWindow& window, graf g[][LATIME / 10], desen piesa);

// (TODO) Salveaza circuitul facut intr-un fisier
void salveaza ();

// (TODO) Deschide un fisier unde a fost salvat un circuit
void deschide ();
