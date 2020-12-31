/*
    important:
    adaugat dimensiuni la celelalte piese
    adaugat piese noi

    mai trebuie facut:
     + drag and drop la bara cu piese
     + rotire si dimensionare piese, editare continut piese
     - informatii cand dai click pe meniuri
     - salvare si deschidere
     - demo

    optional:
     + corectitudine circuit
     + calcule fizice
     - creativitate

    facut:
    bara de sus cu piesele
    bara de meniu
    citire piese din fisier
    mutarea pieselor
    trasare circuit
*/

#include <SFML/Graphics.hpp>
#include <cstring>
#include "functii.h"

using namespace sf;
using namespace std;

Desen piesaPerm[3 * NR_PIESE], piesaMeniu[3 * NR_PIESE], piesaMuta[3 * NR_PIESE];

int main ()
{
    int numarPieseValide = 0;

    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close);

    // citeste toate piesele din toate fisierele
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 6 && fisier[i + 1][j][0]; j++)
        {
            piesaPerm[6* i + j].numar = {};
            char tempFisier[30] = { "Piese\\" };
            strcat (tempFisier, fisier[0][i]);
            strcat (tempFisier, "\\");
            strcat (tempFisier, fisier[i + 1][j]);
            strcat (tempFisier, ".txt");
            FILE* tempFile = fopen (tempFisier, "r");
            if (tempFile == NULL)
                printf ("\n===Nu exista fisierul %s===\n", tempFisier);
            else
            {
                printf ("\n---Inceput piesa %d (%s)---\n\n", 6 * i + j, tempFisier + 6);
                piesaPerm[6 * i + j] = citeste (tempFile);
                if (existaPiesa (piesaPerm[6 * i + j]))
                    numarPieseValide++;
                printf ("\n---Final piesa %d (%s)---\n", 6 * i + j, tempFisier + 6);
            }
        }
    }

    bool click = false, amMutat = false;
    int i = 0, m = 0;
    float iter = 0;
    Punct t = {};
    Vertex linie[30][2];

    printf ("\nNumar piese valide/cu date: %d\n", numarPieseValide);

    while (window.isOpen ())
    {
        Event event;
        window.clear ();

        init (window, piesaPerm);

        // pune piesele in bara de meniu
        Cadran cadr = { 0, 0, LATIME, INALTIME / 10 };

        for (int i = 0; i < 3 * NR_PIESE && !amMutat; i++)
            piesaMeniu[i] = muta (window, piesaPerm[i], Vector2i (LATIME / 1.5 / numarPieseValide * (i + .5), INALTIME / 20));
        for (int i = 0; i < 3 * NR_PIESE; i++)
            deseneazaPiesa (window, piesaMeniu[i]);
        amMutat = true;

        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed (Keyboard::Escape))
                window.close ();
        }

        Cadran tempColt = { 0, 0, LATIME, INALTIME / 10 };
        if (cursorInZona (window, tempColt))
            zonaRosie (window, tempColt, iter);
        
        // pune toate piesele in ordine
        if (Mouse::isButtonPressed (Mouse::Left) && m < 3 * NR_PIESE)
        {
            while (!existaPiesa (piesaPerm[m]) && m < 3 * NR_PIESE - 1)     // sare peste piesele goale
                m++;
            if (m == 3 * NR_PIESE - 1 && !existaPiesa (piesaPerm[m]));
            else
            {
                printf ("Ai mutat piesa %d: ", m);
                if (m < NR_PIESE)
                    printf ("%s\n", fisier[1][m]);
                else if (m >= NR_PIESE && m < 2 * NR_PIESE)
                    printf ("%s\n", fisier[2][m - NR_PIESE]);
                else printf ("%s\n", fisier[3][m - 2 * NR_PIESE]);
            }
            sleep (milliseconds (200));
            piesaMuta[m] = muta (window, piesaPerm[m], Mouse::getPosition (window));
            if (piesaMuta[m].numar.lin == -1)   // piesa in pozitie invalida, mai incearca
            {
                printf ("invalid/zona rosie\n");
                i--;
                m--;
            }
            m++;
        }
        else if (m >= 3 * NR_PIESE)
        {
            trageLinii (window, t, linie, i, click);

            // liniile permanente si temporare au culorile verde > galben > rosu
            for (int j = 0; j < 2 * i; j += 2)
            {
                linie[j][0].color = Color::Color (48, 191, 48, 255);
                linie[j][1].color = Color::Color (191, 191, 95, 255);
                linie[j + 1][0].color = Color::Color (191, 191, 95, 255);
                linie[j + 1][1].color = Color::Color (191, 48, 48, 255);

                window.draw (linie[j], 2, Lines);
                window.draw (linie[j + 1], 2, Lines);
            }
        }
        for (int i = 0; i < 3 * NR_PIESE; i++)
            deseneazaPiesa (window, piesaMuta[i]);

        window.display ();
    }
    return 0;
}