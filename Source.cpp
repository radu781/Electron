/*
    important:
    adaugat dimensiuni la celelalte piese
    adaugat piese noi

    mai trebuie facut:
    drag and drop la bara cu piese
    informatii cand dai click pe meniuri
    trasare circuit, rotire si dimensionare piese, editare continut piese
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
*/

#include <SFML/Graphics.hpp>
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
int main ()
{
    ContextSettings settings;
    settings.antialiasingLevel = 0;
    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close, settings);

    Desen piesaNoua[6];
    for (int i = 0; i < 6; i++)
        piesaNoua[i].numar = {};

    citeste (bat, piesaNoua[0]);
    citeste (anD, piesaNoua[1]);
    citeste (dio, piesaNoua[2]);
    citeste (noT, piesaNoua[3]);
    citeste (rez, piesaNoua[4]);

    bool amMutat = false;
    bool pressed = false;
    int i = 0;
    Punct t = {};
    Vertex linie[10][2];
    while (window.isOpen ())
    {
        Event event;
        window.clear ();
        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed (Keyboard::Escape))
                    window.close ();
            if (pressed && Mouse::isButtonPressed (Mouse::Right))
                pressed = false;
            trageLinii (window, t, linie, i, pressed);
        }
        for (int j = 0; j < i; j++)
        {
            linie[j][0].color = Color::Blue;
            linie[j][1].color = Color::Blue;
            window.draw (linie[j], 2, Lines);
        }

        init (window);

        for (int i = 0; i < 6; i++)
            deseneazaPiesa (window, piesaNoua[i]);

        for (int i = 0; i < 1 && !amMutat; i++)
        {
            Punct coord = { (i + 1) * 50, i * 50 };
            muta (window, piesaNoua[i], coord);
        }
        amMutat = true;
        window.display ();
    }
    return 0;
}