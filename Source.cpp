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

int main ()
{
    ContextSettings settings;
    settings.antialiasingLevel = 0;
    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close, settings);

    desen piesaNoua[6];
    for (int i = 0; i < 6; i++)
        piesaNoua[i].numar = {};

    citeste (bat, piesaNoua[0]);
    citeste (anD, piesaNoua[1]);
    citeste (dio, piesaNoua[2]);
    citeste (noT, piesaNoua[3]);
    citeste (rez, piesaNoua[4]);

    graf g[INALTIME / 10][LATIME / 10];

    bool amMutat = false;
    while (window.isOpen ())
    {
        Event event;
        window.clear ();
        while (window.pollEvent (event))
            if (event.type == Event::Closed || Keyboard::isKeyPressed (Keyboard::Escape))
                window.close ();

        RectangleShape meniu, parti, sep[OBIECTE_MENIU + 1];
        Text nume[OBIECTE_MENIU + 1];
        init (window, meniu, parti, sep, nume);

        for (int i = 0; i < 6; i++)
            if (i == 1)
                deseneazaPiesa (window, piesaNoua[i]);

        for (int i = 0; i < 6 && !amMutat; i++)
        {
            punct coord = { (i + 1) * 50, i * 50 };
            if (i != 1)
                muta (window, piesaNoua[i], coord);
        }
        amMutat = true;
        for (int i = 0; i < 5; i++)
            puneInGraf (window, g, piesaNoua[i]);

        // test varfuri piesa
        CircleShape cerc[3];
        cerc[0].setPosition (60, 120);
        cerc[1].setPosition (170, 120);
        //cerc[2].setPosition (90, 20);
        for (int i = 0; i < 3; i++)
        {
            cerc[i].setFillColor (Color::Red);
            cerc[i].setRadius (1);
            window.draw (cerc[i]);
        }

        // mouse input
        /*for (int i = 0; i <= OBIECTE_MENIU; i++)
        {
            // verifica pe ce meniu dai click
            if (Mouse::isButtonPressed (Mouse::Left) && !pressed)
            {
                if (Mouse::getPosition (window).x >= LATIME / OBIECTE_MENIU * i && Mouse::getPosition (window).x <= LATIME / OBIECTE_MENIU * (i + 1))
                    if (Mouse::getPosition (window).y >= 0 && Mouse::getPosition (window).y <= INALTIME / 20)
                    {
                        cout << "esti in dreptunghiul " << i + 1 << '\n';
                        cout << "x: " << Mouse::getPosition (window).x << " y: " << Mouse::getPosition (window).y << '\n';
                    }
                pressed = 1;
            }
            else pressed = 0;
        }*/
        window.display ();
    }
    return 0;
}