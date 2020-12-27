/*
    important:
    adaugat dimensiuni la celelalte piese
    adaugat piese noi

    mai trebuie facut:
    drag and drop la bara cu piese
    informatii cand dai click pe meniuri 
    rotire si dimensionare piese, editare continut piese
    salvare si deschidere
    demo

    optional:
    corectitudine circuit
    calcule fizice
    creativitate

    facut:
    bara de sus cu piesele
    bara de meniu
    citire piese din fisier
    mutarea pieselor
    trasare circuit
*/

#include <SFML/Graphics.hpp>
#include <cmath>
#include "functii.h"

// mod de a face un loop aici
FILE* bat = fopen ("Piese\\Simple\\baterie.txt", "r");
FILE* dio = fopen ("Piese\\Simple\\dioda.txt", "r");
FILE* rez = fopen ("Piese\\Simple\\rezistor.txt", "r");
FILE* ter = fopen ("Piese\\Simple\\termistor.txt", "r");
FILE* sur = fopen ("Piese\\Simple\\sursa.txt", "r");
FILE* noT = fopen ("Piese\\Logice\\not.txt", "r");
FILE* anD = fopen ("Piese\\Logice\\and.txt", "r");

using namespace sf;
using namespace std;

char g[INALTIME][LATIME];
Desen piesaNoua[6], piesaTemp[6];
int main ()
{
    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close);

    for (int i = 0; i < 6; i++)
        piesaNoua[i].numar = {};

    piesaNoua[0] = citeste (bat);
    piesaNoua[1] = citeste (anD);
    piesaNoua[2] = citeste (dio);
    piesaNoua[3] = citeste (noT);
    piesaNoua[4] = citeste (rez);

    bool click = false;
    int i = 0, m = 0;
    float iter = 0;
    Punct t = {};
    RectangleShape drept;
    Vertex linie[30][2];
    while (window.isOpen ())
    {
        Event event;
        window.clear ();
        init (window);

        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed (Keyboard::Escape))
                window.close ();
        }

        Cadran tempColt = { 0, 0, LATIME, INALTIME / 10 };
        if (cursorInZona (window, tempColt))
            zonaRosie (window, tempColt, iter);

        if (Mouse::isButtonPressed (Mouse::Left) && m < 6)
        {
            printf ("am mutat piesa %d\n", m % 6);
            sleep (milliseconds (200));
            piesaTemp[m % 6] = muta (window, piesaNoua[m++ % 6], Mouse::getPosition (window));
        } 
        else if (m > 5)
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
        for (int i = 0; i < 6; i++)
            deseneazaPiesa (window, piesaTemp[i]);

        window.display ();
    }
    return 0;
}