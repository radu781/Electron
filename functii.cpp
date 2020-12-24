#include <SFML/Graphics.hpp>
#include "functii.h"

using namespace sf;
using namespace std;

void citeste (FILE* file, Desen& piesaCrt)
{
    char sir[100];
    bool amCoord = false, amVarf = false;

    while (!feof (file))
    {
        fgets (sir, 100, file);
        if (sir[0] == '#')
            continue;   // considera liniile ce incep cu '#' ca fiind comentarii
        if (amVarf)
            iaVarfuri (sir, piesaCrt);
        if (amCoord && !isalpha (sir[0]))
            iaCoord (sir, piesaCrt);
        if (strstr (sir, "varfuri"))
            amVarf = true;
        if (strstr (sir, "coordonate"))
        {
            amCoord = true;
            amVarf = false;
        }
    }
}
void deseneazaPiesa (sf::RenderWindow& window, Desen piesaCrt)
{
    for (int i = 0; i < piesaCrt.numar.lin; i++)
    {
        piesaCrt.linie[i][0].color = Color::Magenta;
        piesaCrt.linie[i][1].color = Color::Magenta;
        window.draw (piesaCrt.linie[i], 2, Lines);
    }
    for (int i = 0; i < piesaCrt.numar.drept; i++)
    {
        piesaCrt.dreptunghi[i].setOutlineColor (Color::Magenta);
        piesaCrt.dreptunghi[i].setOutlineThickness (1);
        piesaCrt.dreptunghi[i].setFillColor (Color::Transparent);
        window.draw (piesaCrt.dreptunghi[i]);
    }
    for (int i = 0; i < piesaCrt.numar.cerc; i++)
    {
        piesaCrt.cerc[i].setOutlineColor (Color::Magenta);
        piesaCrt.cerc[i].setOutlineThickness (1);
        piesaCrt.cerc[i].setFillColor (Color::Transparent);
        window.draw (piesaCrt.cerc[i]);
    }
    for (int i = 0; i < piesaCrt.numar.tri; i++)
    {
        piesaCrt.triunghi[i].setOutlineColor (Color::Magenta);
        piesaCrt.triunghi[i].setOutlineThickness (1);
        piesaCrt.triunghi[i].setFillColor (Color::Transparent);
        window.draw (piesaCrt.triunghi[i]);
    }
}
void muta (sf::RenderWindow& window, Desen& piesaCrt, Punct pct)
{
    for (int i = 0; i < piesaCrt.numar.lin; i++)
    {
        piesaCrt.linie[i][0].position += Vector2f (pct.x, pct.y);
        piesaCrt.linie[i][1].position += Vector2f (pct.x, pct.y);
    }
    for (int i = 0; i < piesaCrt.numar.drept; i++)
    {
        Vector2f temp = piesaCrt.dreptunghi[i].getPosition ();
        piesaCrt.dreptunghi[i].setPosition (Vector2f (temp.x + pct.x, temp.y + pct.y));
    }
    for (int i = 0; i < piesaCrt.numar.cerc; i++)
    {
        Vector2f temp = piesaCrt.cerc[i].getPosition ();
        piesaCrt.cerc[i].setPosition (Vector2f (temp.x + pct.x, temp.y + pct.y));
    }
    for (int i = 0; i < piesaCrt.numar.tri; i++)
    {
        Vector2f temp = piesaCrt.triunghi[i].getPosition ();
        piesaCrt.triunghi[i].setPosition (Vector2f (temp.x + pct.x, temp.y + pct.y));
    }
}
void adaugaElement (char graf[INALTIME][LATIME], Punct ini, Punct fin)
{
    graf[int (ini.x)][int (ini.y)] = graf[int (fin.x)][int (fin.y)] = 1;
    return;
}
void afiseazaGraf (char graf[INALTIME][LATIME])
{
    for (int i = 0; i < INALTIME; i++)
    {
        for (int j = 0; j < LATIME; j++)
            printf ("%d ", graf[i][j]);
        printf ("\n");
    }
    printf ("\n\n-----\n\n");
}
void puneInGraf (sf::RenderWindow& window, char graf[INALTIME][LATIME], Desen piesaCrt/*inca o piesa aici*/)
{
    Colt crd = { LATIME, INALTIME, 0, 0 };
    for (int i = 0; i < piesaCrt.numar.lin; i++)
    {
        crd.minim.x = min (crd.minim.x, min (piesaCrt.linie[i][0].position.x, piesaCrt.linie[i][1].position.x));
        crd.maxim.x = max (crd.maxim.x, max (piesaCrt.linie[i][0].position.x, piesaCrt.linie[i][1].position.x));
        crd.minim.y = min (crd.minim.y, min (piesaCrt.linie[i][0].position.y, piesaCrt.linie[i][1].position.x));
        crd.maxim.y = max (crd.maxim.y, max (piesaCrt.linie[i][0].position.y, piesaCrt.linie[i][1].position.y));
    }
    for (int i = 0; i < piesaCrt.numar.drept; i++)
    {
        crd.minim.x = min (crd.minim.x, piesaCrt.dreptunghi[i].getGlobalBounds ().left);
        crd.maxim.x = max (crd.maxim.x, piesaCrt.dreptunghi[i].getGlobalBounds ().left + piesaCrt.dreptunghi[i].getGlobalBounds ().width);
        crd.minim.y = min (crd.minim.y, piesaCrt.dreptunghi[i].getGlobalBounds ().top);
        crd.maxim.y = max (crd.maxim.y, piesaCrt.dreptunghi[i].getGlobalBounds ().top + piesaCrt.dreptunghi[i].getGlobalBounds ().height);
    }
    for (int i = 0; i < piesaCrt.numar.cerc; i++)
    {
        crd.minim.x = min (crd.minim.x, piesaCrt.cerc[i].getGlobalBounds ().left);
        crd.maxim.x = max (crd.maxim.x, piesaCrt.cerc[i].getGlobalBounds ().left + piesaCrt.cerc[i].getGlobalBounds ().width);
        crd.minim.y = min (crd.minim.y, piesaCrt.cerc[i].getGlobalBounds ().top);
        crd.maxim.y = max (crd.maxim.y, piesaCrt.cerc[i].getGlobalBounds ().top + piesaCrt.cerc[i].getGlobalBounds ().height);
    }
    for (int i = 0; i < piesaCrt.numar.tri; i++)
    {
        crd.minim.x = min (crd.minim.x, piesaCrt.triunghi[i].getGlobalBounds ().left);
        crd.maxim.x = max (crd.maxim.x, piesaCrt.triunghi[i].getGlobalBounds ().left + piesaCrt.triunghi[i].getGlobalBounds ().width);
        crd.minim.y = min (crd.minim.y, piesaCrt.triunghi[i].getGlobalBounds ().top);
        crd.maxim.y = max (crd.maxim.y, piesaCrt.triunghi[i].getGlobalBounds ().top + piesaCrt.triunghi[i].getGlobalBounds ().height);
    }

    adaugaElement (graf, crd.minim, crd.maxim);
}
void init (RenderWindow& window)
{
    RectangleShape baraMeniu, baraParti, separatori[OBIECTE_MENIU + 1];

    // bara de meniu
    baraMeniu.setSize (Vector2f (LATIME, INALTIME / 20));
    baraMeniu.setFillColor (Color::Green);
    baraMeniu.setPosition (0, 0);
    window.draw (baraMeniu);

    // bara de parti/piese
    baraParti.setSize (Vector2f (LATIME, INALTIME / 20));
    baraParti.setFillColor (Color::Cyan);
    baraParti.setPosition (0, INALTIME / 20);
    window.draw (baraParti);

    // separatori pentru meniu
    for (int i = 0; i < OBIECTE_MENIU - 1; i++)
    {
        separatori[i].setPosition (Vector2f (LATIME / OBIECTE_MENIU * (i + 1), 0));
        separatori[i].setSize (Vector2f (LATIME_SEP, INALTIME / 20));
        separatori[i].setFillColor (Color::Red);
        window.draw (separatori[i]);
    }

    // text meniuri
    Text titluri[OBIECTE_MENIU + 1];
    Font fontMeniu;
    fontMeniu.loadFromFile ("arial.ttf");
    titluri[0].setString ("Fisiere");
    titluri[1].setString ("Biblioteci");
    titluri[2].setString ("Piese");
    titluri[3].setString ("Legaturi");
    titluri[4].setString ("Descriere");
    titluri[5].setString ("Optiuni");
    titluri[6].setString ("Zoom");
    titluri[7].setString ("Ajutor");
    for (int i = 0; i <= OBIECTE_MENIU; i++)
    {
        titluri[i].setFont (fontMeniu);
        titluri[i].setCharacterSize (16);
        float latTemp = titluri[i].getLocalBounds ().width, inaTemp = titluri[i].getLocalBounds ().height;
        titluri[i].setPosition (Vector2f (LATIME / OBIECTE_MENIU * i + latTemp / 2, INALTIME / 80));    // va trebui centrat mai bine
        titluri[i].setFillColor (Color::Blue);
        window.draw (titluri[i]);
    }
}
void iaCoord (char s[], Desen& piesaCrt)
{
    int numar = 0;
    float temp;
    if (strchr (s, 'l'))
    {
        char* p = strtok (s, " ");
        while (p)
        {
            if (numar == 2)
                piesaCrt.linie[piesaCrt.numar.lin][0].position.x = atof (p);
            else if (numar == 3)
                piesaCrt.linie[piesaCrt.numar.lin][0].position.y = atof (p);
            else if (numar == 4)
                piesaCrt.linie[piesaCrt.numar.lin][1].position.x = atof (p);
            else if (numar == 5)
                piesaCrt.linie[piesaCrt.numar.lin++][1].position.y = atof (p);
            numar++;
            p = strtok (NULL, " ");
        }
    }
    else if (strchr (s, 'c'))
    {
        char* p = strtok (s, " ");
        while (p)
        {
            if (numar == 2)
                temp = atof (p);
            else if (numar == 3)
                piesaCrt.cerc[piesaCrt.numar.cerc].setPosition (temp, atof (p));
            else if (numar == 4)
                piesaCrt.cerc[piesaCrt.numar.cerc++].setRadius (atof (p));
            numar++;
            p = strtok (NULL, " ");
        }
    }
    else if (strchr (s, 'd'))
    {
        char* p = strtok (s, " ");
        while (p)
        {
            if (numar == 2)
                temp = atof (p);
            else if (numar == 3)
                piesaCrt.dreptunghi[piesaCrt.numar.drept].setSize (Vector2f (temp, atof (p)));
            else if (numar == 4)
                temp = atof (p);
            else if (numar == 5)
                piesaCrt.dreptunghi[piesaCrt.numar.drept++].setPosition (temp, atof (p));
            numar++;
            p = strtok (NULL, " ");
        }
    }
    else if (strchr (s, 't'))
    {
        char* p = strtok (s, " ");
        piesaCrt.triunghi[piesaCrt.numar.tri].setPointCount (3);
        while (p)
        {
            if (numar == 2)
                temp = atof (p);
            else if (numar == 3)
                piesaCrt.triunghi[piesaCrt.numar.tri].setPoint (0, Vector2f (temp, atof (p)));
            else if (numar == 4)
                temp = atof (p);
            else if (numar == 5)
                piesaCrt.triunghi[piesaCrt.numar.tri].setPoint (1, Vector2f (temp, atof (p)));
            else if (numar == 6)
                temp = atof (p);
            else if (numar == 7)
                piesaCrt.triunghi[piesaCrt.numar.tri++].setPoint (2, Vector2f (temp, atof (p)));
            numar++;
            p = strtok (NULL, " ");
        }
    }
}
void iaVarfuri (char s[], Desen& piesaCrt)
{
    char* p = strtok (s, " ");
    int numar = 0;
    while (p)
    {
        if (numar == 1)
            piesaCrt.varfuri[piesaCrt.numar.varfuri].x = atof (p);
        else if (numar == 2)
            piesaCrt.varfuri[piesaCrt.numar.varfuri++].y = atof (p);
        numar++;
        p = strtok (NULL, " ");
    }
}
void salveaza ()
{

}
void deschide ()
{

}
void trageLinii (RenderWindow& window, Punct& t, sf::Vertex linie[][2], int& i, bool& pressed)
{
    //printf ("ai dat click la: ");
    if (Mouse::isButtonPressed (Mouse::Left))
        if (!pressed)
        {
            t = { (float)Mouse::getPosition (window).x, (float)Mouse::getPosition (window).y };
            pressed = true;
            printf ("primul click: %f, %f\n", (float)Mouse::getPosition (window).x, (float)Mouse::getPosition (window).y);
        }
        else
        {
            linie[i][0].position.x = t.x;
            linie[i][0].position.y = t.y;
            linie[i][1].position.x = (float)Mouse::getPosition (window).x;
            linie[i++][1].position.y = t.y;

            linie[i][0].position.x = (float)Mouse::getPosition (window).x;
            linie[i][0].position.y = t.y;
            linie[i][1].position.x = (float)Mouse::getPosition (window).x;
            linie[i++][1].position.y = (float)Mouse::getPosition (window).y;

            pressed = false;
        }
    else if (pressed && !Mouse::isButtonPressed (Mouse::Right))
    {
        Vertex temp[2][2];
        temp[0][0].position.x = t.x;
        temp[0][0].position.y = t.y;
        temp[0][1].position.x = (float)Mouse::getPosition (window).x;
        temp[0][1].position.y = t.y;

        temp[1][0].position.x = (float)Mouse::getPosition (window).x;
        temp[1][0].position.y = t.y;
        temp[1][1].position.x = (float)Mouse::getPosition (window).x;
        temp[1][1].position.y = (float)Mouse::getPosition (window).y;

        temp[0][0].color = Color::Color (127, 127, 191, 255);
        temp[0][1].color = Color::Color (127, 127, 191, 255);
        temp[1][0].color = Color::Color (127, 127, 191, 255);
        temp[1][1].color = Color::Color (127, 127, 191, 255);
        window.draw (temp[0], 2, Lines);
        window.draw (temp[1], 2, Lines);

    }
    //printf ("%d, %d\n", (int)Mouse::getPosition (window).x, (int)Mouse::getPosition (window).y);
}