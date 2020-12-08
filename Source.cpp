// bara de sus cu piesele si drag and drop - 12 
#include <SFML/Graphics.hpp>
#define INALTIME 600
#define LATIME 800
#define MENU_ITEMS 8
using namespace sf;
int main ()
{
    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron");

    while (window.isOpen ())
    {
        Event event;
        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed (Keyboard::Escape))
                window.close ();
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
            separatori[i].setSize (Vector2f (5, INALTIME / 20));
            separatori[i].setFillColor (Color::Red);
        }
        // separatori pentru meniu

        RectangleShape baterie[4];
        float x, y;
        if (Mouse::isButtonPressed (Mouse::Left))
        {
            x = Mouse::getPosition ().x;
            y = Mouse::getPosition ().y;
            baterie[0].setSize (Vector2f (30, 1));
            baterie[0].setPosition (Vector2f (x, y));
            baterie[1].setSize (Vector2f (30, 1));
            baterie[1].setPosition (Vector2f (x + 40, y));
            baterie[2].setSize (Vector2f (1, 30));
            baterie[2].setPosition (Vector2f (x + 30, y - 15));
            baterie[3].setSize (Vector2f (1, 15));
            baterie[3].setPosition (Vector2f (x + 40, y - 7));
        }
        // desen baterie, nu merge deocamdata

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

        window.clear ();
        for (int i = 0; i < 4; i++)
            window.draw (baterie[i]);
        window.draw (baraMeniu);
        window.draw (baraParti);
        for (int i = 0; i < MENU_ITEMS; i++)
        {
            window.draw (separatori[i]);
            window.draw (nume[i]);
        }
        window.display ();
    }
    return 0;
}