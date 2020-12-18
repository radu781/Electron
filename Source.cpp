/*
    important:
    adaugat dimensiuni la celelalte piese
    adaugat piese noi

    mai trebuie facut:
    drag and drop la bara cu piese - pana pe 12 decembrie
    informatii cand dai click pe meniuri - pana pe 19 decembrie
    trasare circuit, rotire si dimensionare piese, editare continut piese - pana pe 9 ianuarie
    salvare si deschidere - pana pe 16 ianuarie
    demo

    optional:
    corectitudine circuit
    calcule fizice
    creativitate

    facut:
    bara de sus cu piesele
    bara de meniu
    citire piese din fisier
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>
#include "functii.h"

#define INALTIME 600
#define LATIME 800
#define OBIECTE_MENIU 8
#define LATIME_SEP 3
#define DIMENSIUNE 6

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
        piesaNoua[i].nr = {};

    citeste (dio, piesaNoua[0]);
    citeste (anD, piesaNoua[1]);
    citeste (bat, piesaNoua[2]);
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
            deseneazaPiesa (window, piesaNoua[i]);

        for (int i = 0; i < 6 && !amMutat; i++)
        {
            punct coord = { (i + 1) * 50, i * 50 };
            muta (window, piesaNoua[i], coord);
        }
        amMutat = true;
        for (int i = 0; i < 5; i++)
            puneInGraf (window, g, piesaNoua[i]);
        system ("CLS");

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