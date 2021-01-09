/*
    important:
    adaugat varfuri la piese
    mai trebuie facut:
     + drag and drop piese
     + rotire si dimensionare piese, editare continut piese
     - informatii cand dai click pe meniuri
     - prezentare si executabil
    optional:
     + corectitudine circuit
     + calcule fizice
     - creativitate
    facut:
    bara de sus cu piesele
    bara de meniu
    citire piese din fisier
    mutarea pieselor (initiala)
    trasare circuit
    salvare si deschidere
*/

// TODO ultima piesa nu apare
// TODO desenare piese din lista
// TODO trage linii cu event
// TODO desenare linii din lista

#include <SFML/Graphics.hpp>
#include <cstring>
#include "functii.h"

using namespace sf;
using namespace std;

// de folosit lista
Desen piesaPerm[3 * NR_PIESE], piesaMeniu[3 * NR_PIESE], piesaMuta[3 * NR_PIESE], piesaPerm2[3 * NR_PIESE];
Nod* grafCurent, * capGraf;
Lista* listaPiese, * capPiese, * coadaPiese, * listaLeg, * capLeg, * coadaLeg;

int main ()
{
#if 1
    int nrPieseValide = 0;

    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close);

    // citeste toate piesele din toate fisierele
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < NR_PIESE && NUME_FISIERE[i + 1][j][0]; j++)
        {
            piesaPerm[NR_PIESE * i + j].numar = {};
            const char* numeCurent = numeFisier (i, j);
            FILE* tempFile = fopen (numeCurent, "r");
            if (tempFile)
            {
                piesaPerm[NR_PIESE * i + j] = citeste (tempFile);
                printf ("");
                if (existaPiesa (piesaPerm[6 * i + j]))
                    nrPieseValide++;
            }
        }
    }

    printf ("Piese valide: %d\n", nrPieseValide);
    bool anulat = false;
    int i = 0, meniu = -1, luat = -1, nr = 0;
    Punct coordLinie = {};
    Vertex linie[30][2];
    Cadran linInter = {};

    // pune piesele in bara de meniu
    for (int i = 0; i < 3 * NR_PIESE; i++)
        piesaMeniu[i] = muta (window, piesaPerm[i], Vector2i (LATIME / nrPieseValide * (i + .5), INALTIME / 13.5));

    while (window.isOpen ())
    {
        Event event;
        window.clear ();

        init (window, piesaPerm);

        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed (Keyboard::Escape))
                window.close ();

            for (int i = 0; i < NR_MENIU; i++)
                if (cursorInZona (window, { (float)LATIME / NR_MENIU * i, 0, (float)LATIME / NR_MENIU * (i + 1), INALTIME / 20 }))
                    if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Left))
                    {
                        meniu = i;
                        printf ("[MENIU] %s\n", *(NUME_TITLURI + meniu));
                    }

            switch (meniu)
            {
            case -1:
                break;
            case 2:
                for (int i = 0; i < nrPieseValide; i++)
                {
                    if (cursorInZona (window, { (float)LATIME / nrPieseValide * i, INALTIME / 20, (float)LATIME / nrPieseValide * (i + 1), INALTIME / 10 }))
                        if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Left))
                        {
                            luat = i;
                            printf ("\n[PIESA] ridicat %s\n", NUME_FISIERE[1 + luat / 6][luat % 6]);
                            break;
                        }
                        else;
                    else
                    {
                        if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Right) && luat != -1)
                        {
                            printf ("[PIESA] anulata: %s\n", NUME_FISIERE[1 + luat / 6][luat % 6]);
                            luat = -1;
                            break;
                        }
                        else if (event.type == Event::MouseButtonReleased && luat != -1)
                        {
                            piesaMuta[luat] = muta (window, piesaPerm[luat], Mouse::getPosition (window));
                            if (!cursorInZona (window, { 0, 0, LATIME, INALTIME / 10 }))
                            {
                                printf ("[PIESA] pusa jos %s\n", NUME_FISIERE[1 + luat / 6][luat % 6]);
                                puneInLista (listaPiese, capPiese, coadaPiese, piesaMuta[luat], piesaMuta[luat].id);
                                char a[] = "save";
                                salveaza (grafCurent, capGraf, listaPiese, capPiese, a);
                            }
                            else printf ("[PIESA] nu poti pune piesa in meniu\n");
                            luat = -1;
                            break;
                        }
                    }
                }
                break;
            case 3:
                if (!cursorInZona (window, { 0, 0, LATIME, INALTIME / 10 }))
                {
                    Cadran cop = linInter;
                    linInter = trageLinii (window, event);
                    if (linInter.minim.x && linInter.minim.y && linInter.maxim.x && linInter.maxim.y && cop == linInter)
                        printf ("%.0f %.0f > %.0f %.0f\n", linInter.minim.x, linInter.minim.y, linInter.maxim.x, linInter.maxim.y);
                }
                break;
            case 7:
                break;
            default:
                printf ("Ai dat click pe un meniu la care nu am facut nimic inca: %s\n\n", NUME_TITLURI[meniu]);
                meniu = -1;
            }
        }
        // TODO meniu
        switch (meniu)
        {
        case -1:
            break;
        case 0:
            break;
        case 1:
            break;
        case 2:
            for (int i = 0; i < nrPieseValide - 1; i++)
            {
                RectangleShape rect;

                rect.setPosition (Vector2f (LATIME / nrPieseValide * (i + 1), INALTIME / 20));
                rect.setSize (Vector2f (LATIME_SEP - 1, INALTIME / 20));
                rect.setFillColor (Color (Color::Color (252, 68, 69, 255)));

                window.draw (rect);
            }

            for (int i = 0; i < 3 * NR_PIESE; i++)
                deseneazaPiesa (window, piesaMeniu[i]);
            if (luat != -1)
            {
                piesaPerm2[luat] = muta (window, piesaPerm[luat], Mouse::getPosition (window));
                deseneazaPiesa (window, piesaPerm2[luat]);
            }
            break;
        case 3:
            // ai primul click, deseneaza pana la cursor
            if (linInter.minim.x && linInter.minim.y && linInter.maxim.x == 0 && linInter.maxim.y == 0)
            {
                Vertex lin[2][2];

                lin[0][0].position = Vector2f (linInter.minim.x, linInter.minim.y);
                lin[0][1].position = Vector2f (Mouse::getPosition (window).x, linInter.minim.y);
                lin[1][0].position = Vector2f (Mouse::getPosition (window).x, linInter.minim.y);
                lin[1][1].position = Vector2f (Mouse::getPosition (window).x, Mouse::getPosition (window).y);

                lin[0][0].color = Color::Color (48, 191, 48, 255);
                lin[0][1].color = Color::Color (191, 191, 95, 255);
                lin[1][0].color = Color::Color (191, 191, 95, 255);
                lin[1][1].color = Color::Color (191, 48, 48, 255);

                window.draw (lin[0], 2, Lines);
                window.draw (lin[1], 2, Lines);
            }
            // daca pun jos piesa
            else if (linInter.minim.x && linInter.minim.y && linInter.maxim.x && linInter.maxim.y)
            {
                puneInGraf (grafCurent, capGraf, linInter);
                char a[] = "save";
                salveaza (grafCurent, capGraf, listaPiese, capPiese, a);
                linInter = {};
            }
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
        {
            Text text[NR_AJUTOR];
            Font font;
            font.loadFromFile ("Fonturi\\arial.ttf");
            for (int i = 0; i < NR_AJUTOR; i++)
            {
                text[i].setFont (font);
                text[i].setCharacterSize (14);
                text[i].setString (NUME_AJUTOR[i]);

                FloatRect rect = text[i].getLocalBounds ();

                text[i].setOrigin (rect.left + (int)rect.width / 2, rect.top + (int)rect.height / 2);
                text[i].setPosition (Vector2f ((int)LATIME / NR_AJUTOR * (i + .5), (int)INALTIME / 13));
                text[i].setFillColor (Color::Black);

                window.draw (text[i]);
            }
            for (int i = 0; i < NR_AJUTOR - 1; i++)
            {
                RectangleShape rect;
                rect.setPosition (Vector2f (LATIME / NR_AJUTOR * (i + 1), INALTIME / 20));
                rect.setSize (Vector2f (LATIME_SEP - 1, INALTIME / 20));
                rect.setFillColor (Color::Color (252, 68, 69, 255));
                window.draw (rect);
            }
            break;
        }
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        default: printf ("Prea multe obiecte in meniu (%d)\n", meniu);
        }

        for (int i = 0; i < 3 * NR_PIESE; i++)
            deseneazaPiesa (window, piesaMuta[i]);

        window.display ();
    }
    char b[] = "nou5";
    salveaza (grafCurent, capGraf, listaPiese, capPiese, b);
#endif
#if 0// test verificare salvare si deschidere fisiere
    char a[] = "nou4";

    Nod* grafNou = {}, * cap = {};
    Lista* listaNoua = {}, * varf = {}, * coada = {};
    deschide (grafNou, cap, listaNoua, varf, coada, a);
    afiseazaGraf (grafNou, cap);
    afiseazaLista (listaNoua, varf);

    Nod* g = {}, * c = {};
    Lista* l = {}, * v = {}, * q = {};
    insereazaGraf (g, c, { 100, 100 }, { 20, 50 });
    insereazaGraf (g, c, { 100, 100 }, { 20, 50 });
    insereazaGraf (g, c, { 101, 1050 }, { 22, 45 });
    insereazaGraf (g, c, { 151, 160 }, { 22, 40 });
    insereazaGraf (g, c, { 100, 100 }, { 20, 54 });
    insereazaLista (l, v, q, { 1, 1 }, 'c');
    insereazaLista (l, v, q, { 1, 2 }, 'b');
    insereazaLista (l, v, q, { 4, 1 }, 'd');
    salveaza (g, c, l, v, a);
#endif
    afiseazaGraf (grafCurent, capGraf);
    printf ("\n");
    afiseazaLista (listaPiese, capPiese);
    return 0;
}