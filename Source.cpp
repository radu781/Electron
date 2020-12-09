/*
bara de sus cu piesele si drag and drop - pana pe 12 decembrie
bara de meniu de sus (biblioteci, piese, legaturi, optiuni, lupa, ajutor) - pana pe 19 decembrie

trasare circuit, rotire si dimensionare piese, editare continut piese - pana pe 9 ianuarie
salvare si deschidere - 16 ianuarie

optional, dar ar fi bine sa facem:
corectitudine circuit
calcule fizice
creativitate

final: demo
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#define INALTIME 600
#define LATIME 800
#define MENU_ITEMS 8
#define LATIME_SEP 5
using namespace sf;
int main ()
{
    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close);
    View view = window.getDefaultView ();

    float x1 = -1, y1 = -1, x2 = -1, y2 = -1;
    bool pressed = 0;
    while (window.isOpen ())
    {
        Event event;
        window.clear ();
        RectangleShape baterie[4];
        while (window.pollEvent (event));
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed (Keyboard::Escape))
                window.close ();
            //if (Mouse::isButtonPressed (Mouse::Left) && !pressed)
            //{
            //    x1 = Mouse::getPosition (window).x;
            //    y1 = Mouse::getPosition (window).y;
            //    pressed = 1 - pressed;
            //    // coordonate vechi
            //}
            //if (Mouse::isButtonPressed (Mouse::Left) && pressed)
            //{
            //    baterie[0].setSize (Vector2f (30, 1));
            //    baterie[0].setPosition (Vector2f (x1, y1));
            //    baterie[1].setSize (Vector2f (30, 1));
            //    baterie[1].setPosition (Vector2f (x1 + 40, y1));
            //    baterie[2].setSize (Vector2f (1, 30));
            //    baterie[2].setPosition (Vector2f (x1 + 30, y1 - 15));
            //    baterie[3].setSize (Vector2f (1, 15));
            //    baterie[3].setPosition (Vector2f (x1 + 40, y1 - 7));
            //    // x1 y1 x1 y1
            //    //       x2 y2
            //    std::cout << "x1: " << x1 << " y1: " << y1 << "\tx2: " << x2 << " y2: " << y2 << '\n';
            //    x2 = x1;
            //    y2 = y1;
            //    pressed = 1 - pressed;
            //}
            //for (int i = 0; i < 4 && !pressed; i++)
            //    window.draw (baterie[i]);

            // pentru alta data
            // desen baterie, nu merge deocamdata
        }

        RectangleShape baraMeniu;
        baraMeniu.setSize (Vector2f (LATIME, INALTIME / 20));
        baraMeniu.setFillColor (Color::Green);
        baraMeniu.setPosition (0, 0);
        // bara de meniu
        RectangleShape baraParti;
        baraParti.setSize (Vector2f (LATIME, INALTIME / 20));
        baraParti.setFillColor (Color::Cyan);
        baraParti.setPosition (0, INALTIME / 20);
        // bara de parti/piese

        RectangleShape separatori[MENU_ITEMS];
        for (int i = 0; i < MENU_ITEMS - 1; i++)
        {
            separatori[i].setPosition (Vector2f (LATIME / MENU_ITEMS * (i + 1), 0));
            separatori[i].setSize (Vector2f (LATIME_SEP, INALTIME / 20));
            separatori[i].setFillColor (Color::Red);
        }
        // separatori pentru meniu

        Font fontMeniu;
        fontMeniu.loadFromFile ("arial.ttf");
        Text nume[MENU_ITEMS + 1];
        nume[0].setString ("Fisiere");
        nume[1].setString ("Biblioteci");
        nume[2].setString ("Piese");
        nume[3].setString ("Legaturi");
        nume[4].setString ("Descriere");
        nume[5].setString ("Optiuni");
        nume[6].setString ("Zoom");
        nume[7].setString ("Ajutor");
        // text meniu
        for (int i = 0; i <= MENU_ITEMS; i++)
        {
            nume[i].setCharacterSize (15);
            nume[i].setFont (fontMeniu);
            float temp = nume[i].getLocalBounds ().width;
            nume[i].setPosition (Vector2f (LATIME / MENU_ITEMS * i + temp / 2, INALTIME / 80));
            // justificare 60
            nume[i].setFillColor (Color::Blue);
        }

        for (int i = 0; i <= MENU_ITEMS; i++)
        {
            // verifica pe ce meniu dai click
            if (Mouse::isButtonPressed (Mouse::Left) && !pressed)
            {
                if (Mouse::getPosition (window).x >= LATIME / MENU_ITEMS * i && Mouse::getPosition (window).x <= LATIME / MENU_ITEMS * (i + 1))
                    if (Mouse::getPosition (window).y >= 0 && Mouse::getPosition (window).y <= INALTIME / 20)
                    {
                        std::cout << "esti in dreptunghiul " << i + 1 << '\n';
                        std::cout << "x: " << Mouse::getPosition (window).x << " y: " << Mouse::getPosition (window).y << '\n';
                    }
                pressed = 1;
            }
            else pressed = 0;
        }
        // de verificat

        baterie[0].setSize (Vector2f (30, 1));
        baterie[0].setPosition (Vector2f (30, INALTIME / 13));
        baterie[1].setSize (Vector2f (30, 1));
        baterie[1].setPosition (Vector2f (70, INALTIME / 13));
        baterie[2].setSize (Vector2f (1, 30));
        baterie[2].setPosition (Vector2f (60, INALTIME / 13 - 15));
        baterie[3].setSize (Vector2f (1, 15));
        baterie[3].setPosition (Vector2f (70, INALTIME / 13 - 7));

        window.draw (baraMeniu);
        window.draw (baraParti);
        for (int i = 0; i < 4; i++)
        {
            baterie[i].setFillColor (Color::Black);
            window.draw (baterie[i]);
        }
        for (int i = 0; i < MENU_ITEMS; i++)
        {
            window.draw (separatori[i]);
            window.draw (nume[i]);
        }
        window.display ();
    }
    return 0;
}