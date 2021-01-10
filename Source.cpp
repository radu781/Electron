/*
    important:
     - adaugat varfuri la piese

    mai trebuie facut:
     + drag and drop piese din restul ferestrei
     + rotire si dimensionare piese, editare continut piese
     - informatii cand dai click pe meniuri
     - prezentare

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
    drag and drop piese doar din bara de meniu
    executabil
*/

// TODO ultima piesa nu apare
// TODO dupa ce ridici o piesa, bara sa se faca rosie

#include <SFML/Graphics.hpp>
#include "functii.h"

using namespace sf;
using namespace std;

// de folosit lista
Desen piesaPerm[3 * NR_PIESE], piesaMeniu[3 * NR_PIESE], piesaMuta[3 * NR_PIESE], piesaFinal[3 * NR_PIESE];
Nod* grafCurent, * capGraf;
Lista* listaPiese, * capPiese, * coadaPiese;

int main ()
{
    int nrPieseValide = 0;

    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close);

    // citeste toate piesele din toate fisierele
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < NR_PIESE && NUME_FISIERE[i + 1][j][0]; j++)
        {
            piesaPerm[NR_PIESE * i + j].numar = {};
            const char* numeCurent = numeFisier (i, j);
            FILE* tempFile = fopen (numeCurent, "r");
            if (tempFile)
            {
                piesaPerm[NR_PIESE * i + j] = citeste (tempFile);
                if (existaPiesa (piesaPerm[6 * i + j]))
                    nrPieseValide++;
            }
        }

    // pune piesele in bara de meniu
    for (int i = 0; i < 3 * NR_PIESE; i++)
        piesaMeniu[i] = muta (window, piesaPerm[i], Vector2i (LATIME / nrPieseValide * (i + .5), INALTIME / 13.5));

    printf ("[INFO] Piese valide: %d\n", nrPieseValide);
    bool anulat = false;
    int meniu = 4, luat = -1, nr = 0;
    char fileName[] = "save";
    Punct coordLinie = {};
    Vertex linie[30][2];
    Cadran linInter = {};

    while (window.isOpen ())
    {

        Event event;
        window.clear ();

        init (window);

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
            case 0:
                break;
            case 1:
                for (int i = 0; i < nrPieseValide; i++)
                    if (cursorInZona (window, { (float)LATIME / nrPieseValide * i, INALTIME / 20, (float)LATIME / nrPieseValide * (i + 1), INALTIME / 10 }))
                        if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Left))
                        {
                            luat = i;
                            printf ("\n[PIESA] ridicat %s\n", NUME_FISIERE[1 + luat / 6][luat % 6]);
                            break;
                        }
                        else;
                    else
                        if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Right) && luat != -1)
                        {
                            printf ("[PIESA] anulata %s\n", NUME_FISIERE[1 + luat / 6][luat % 6]);
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
                                salveaza (grafCurent, capGraf, listaPiese, capPiese, fileName);
                            }
                            else
                                printf ("[PIESA] nu poti pune piesa in meniu\n");
                            luat = -1;
                            break;
                        }
                break;
            case 2:
                if (!cursorInZona (window, { 0, 0, LATIME, INALTIME / 10 }))
                {
                    Cadran temp = linInter;

                    linInter = trageLinii (window, event);
                    // coordonatele anterioare sa nu fie egale cu cele noi
                    if (linInter.minim.x && linInter.minim.y && linInter.maxim.x && linInter.maxim.y && temp != linInter)
                        insereazaGraf (grafCurent, capGraf, linInter.minim, linInter.maxim);
                    salveaza (grafCurent, capGraf, listaPiese, capPiese, fileName);
                }
                break;
            case 3:
                break;
            case 4:
                for (int i = 0; i < NR_AJUTOR; i++)
                    if (cursorInZona (window, { (float)LATIME / NR_AJUTOR * i, INALTIME / 20, (float)LATIME / NR_AJUTOR * (i + 1), INALTIME / 10 }))
                        if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Left))
                        printf ("[Ajutor] esti in %s\n", NUME_AJUTOR[i]);
                break;
            default:
                printf ("[WARN] Nu ar fi trebuit sa ajungi aici: %s\n\n", NUME_TITLURI[meniu]);
                meniu = -1;
            }
        }
        // TODO meniu
        switch (meniu)
        {
        case -1:
            break;
        case 0:
        {
            Text text;
            Font font;
            font.loadFromFile ("Fonturi\\arial.ttf");
            text.setFont (font);
            text.setString ("Lista de fisiere disponibile: ");
            text.setCharacterSize (14); 
            
            FloatRect rect = text.getLocalBounds ();

            text.setOrigin (rect.left + (int)rect.width / 2, rect.top + (int)rect.height / 2);
            text.setPosition (Vector2f ((int)LATIME / 2, (int)INALTIME / 13));
            text.setFillColor (Color::Black);

            window.draw (text);
            break;
        }
        case 1:
            // separatori bara de piese
            for (int i = 0; i < nrPieseValide - 1; i++)
            {
                RectangleShape rect;

                rect.setPosition (Vector2f (LATIME / nrPieseValide * (i + 1), INALTIME / 20));
                rect.setSize (Vector2f (LATIME_SEP - 1, INALTIME / 20));
                rect.setFillColor (Color::ROSU2);

                window.draw (rect);
            }
            // piesele in sine
            for (int i = 0; i < 3 * NR_PIESE; i++)
                deseneazaPiesa (window, piesaMeniu[i]);
            // muta piesa dupa cursor
            if (luat != -1)
            {
                piesaMuta[luat] = muta (window, piesaPerm[luat], Mouse::getPosition (window));
                if (limitePiesa (piesaMuta[luat]).minim.y <= INALTIME / 10)
                    zonaRosie (window, { 0, 0, LATIME, INALTIME / 10 });
                /*if (cursorInZona (window, limitePiesa (piesaMuta[luat])))
                    zonaRosie (window, limitePiesa (piesaMuta[luat]));*/
                deseneazaPiesa (window, piesaMuta[luat]);
            }
            break;
        case 2:
        {
            Text text;
            Font font;
            font.loadFromFile ("Fonturi\\arial.ttf");
            text.setFont (font);
            text.setString ("Puteti trage legaturi intre piese dand click pe pozitia initiala si eliberand pe pozitia finala");
            text.setCharacterSize (14);

            FloatRect rect = text.getLocalBounds ();

            text.setOrigin (rect.left + (int)rect.width / 2, rect.top + (int)rect.height / 2);
            text.setPosition (Vector2f ((int)LATIME / 2, (int)INALTIME / 13));
            text.setFillColor (Color::Black);

            window.draw (text);

            // intre click si eliberare click trage linii
            if (linInter.minim.x && linInter.minim.y && linInter.maxim.x == 0 && linInter.maxim.y == 0)
            {
                if (Mouse::getPosition (window).y <= INALTIME / 10)
                    zonaRosie (window, { 0, 0, LATIME, INALTIME / 10 });

                Vertex lin[2][2];

                lin[0][0].position = Vector2f (linInter.minim.x, linInter.minim.y);
                lin[0][1].position = Vector2f (Mouse::getPosition (window).x, linInter.minim.y);
                lin[1][0].position = Vector2f (Mouse::getPosition (window).x, linInter.minim.y);
                lin[1][1].position = Vector2f (Mouse::getPosition (window).x, Mouse::getPosition (window).y);

                lin[0][0].color = Color::VERDE1;
                lin[0][1].color = Color::GALBEN1;
                lin[1][0].color = Color::GALBEN1;
                lin[1][1].color = Color::ROSU1;

                window.draw (lin[0], 2, Lines);
                window.draw (lin[1], 2, Lines);
            }
            break;
        }
        case 3: 
        {
            Text text;
            Font font;
            font.loadFromFile ("Fonturi\\arial.ttf");
            text.setFont (font);
            text.setString ("Alegeti nivelul de zoom pe care il doriti: ");
            text.setCharacterSize (14);

            FloatRect rect = text.getLocalBounds ();

            text.setOrigin (rect.left + (int)rect.width / 2, rect.top + (int)rect.height / 2);
            text.setPosition (Vector2f ((int)LATIME / 2 - LATIME / 20, (int)INALTIME / 13));
            text.setFillColor (Color::Black);

            window.draw (text);

            Text zoomDrept[2];
            zoomDrept[0].setString ("-");
            zoomDrept[1].setString ("+");
            RectangleShape zoomText[2];
            for (int i = 0; i < 2; i++)
            {
                zoomText[i].setPosition (LATIME / 1.5 + i * 40, INALTIME / 17);
                zoomText[i].setSize (Vector2f (20, 20));
                zoomText[i].setFillColor (Color::Cyan);

                zoomDrept[i].setFont (font);
                zoomDrept[i].setCharacterSize (30);
                zoomDrept[i].setFillColor (Color::Black);
                FloatRect rec = zoomText[i].getLocalBounds ();
                zoomDrept[i].setOrigin (rec.left + (int)rec.width / 2, rec.top + (int)rec.height / 2);
                zoomDrept[i].setPosition (Vector2f (zoomText[i].getPosition ().x + 15, zoomText[i].getPosition ().y));

                window.draw (zoomText[i]);
                window.draw (zoomDrept[i]);
            }

            break;
        }
        case 4:
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
                rect.setFillColor (Color::ROSU2);
                window.draw (rect);
            }
            break;
        }
        default: 
            printf ("[WARN] Prea multe obiecte in meniu (%d)\n", meniu);
        }

        int totPiese = 0, totLinii = 0;
        //deschide (grafCurent, capGraf, listaPiese, capPiese, coadaPiese, fileName);
        //afiseazaGraf (grafCurent, capGraf);
        //afiseazaLista (listaPiese, capPiese);
        restituie (window, grafCurent, capGraf, listaPiese, capPiese, coadaPiese, piesaPerm, piesaFinal, linie, totPiese, totLinii);

        // piesele in urma mutarii
        for (int i = 0; i < 3 * NR_PIESE; i++)
            deseneazaPiesa (window, piesaFinal[i]);
        // legaturile in urma mutarii
        for (int i = 0; i < totLinii; i++)
        {
            Vertex temp[2][2];

            temp[0][0] = Vector2f (linie[i][0].position.x, linie[i][0].position.y);
            temp[0][1] = Vector2f (linie[i][1].position.x, linie[i][0].position.y);
            temp[1][0] = Vector2f (linie[i][1].position.x, linie[i][0].position.y);
            temp[1][1] = Vector2f (linie[i][1].position.x, linie[i][1].position.y);

            temp[0][0].color = Color::VERDE1;
            temp[0][1].color = Color::GALBEN1;
            temp[1][0].color = Color::GALBEN1;
            temp[1][1].color = Color::ROSU1;

            window.draw (temp[0], 2, Lines);
            window.draw (temp[1], 2, Lines);
        }

        window.display ();
    }
    return 0;
}
