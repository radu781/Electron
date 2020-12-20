#include <SFML/Graphics.hpp>
#include "functii.h"

using namespace sf;
using namespace std;

void citeste (FILE* file, desen& piesaCrt)
{
    char sir[100];
    bool amCoord = false, amVarf = false;

    while (!feof (file))
    {
        fgets (sir, 100, file);
        if (sir[0] == '#')
            continue;
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

        // test coordonate varfuri
        if (amVarf)
        {
            printf ("x\t\ty\n");
            printf ("%f\t%f\n\n", piesaCrt.varfuri[piesaCrt.numar.varfuri - 1].x, piesaCrt.varfuri[piesaCrt.numar.varfuri - 1].y);
        }
    }
}
void deseneazaPiesa (sf::RenderWindow& window, desen piesaCrt)
{
    for (int i = 0; i < piesaCrt.numar.piese.lin; i++)
    {
        piesaCrt.linie[i][0].color = Color::Magenta;
        piesaCrt.linie[i][1].color = Color::Magenta;
        window.draw (piesaCrt.linie[i], 2, Lines);
    }
    for (int i = 0; i < piesaCrt.numar.piese.drept; i++)
    {
        piesaCrt.dreptunghi[i].setOutlineColor (Color::Magenta);
        piesaCrt.dreptunghi[i].setOutlineThickness (1);
        piesaCrt.dreptunghi[i].setFillColor (Color::Transparent);
        window.draw (piesaCrt.dreptunghi[i]);
    }
    for (int i = 0; i < piesaCrt.numar.piese.cerc; i++)
    {
        piesaCrt.cerc[i].setOutlineColor (Color::Magenta);
        piesaCrt.cerc[i].setOutlineThickness (1);
        piesaCrt.cerc[i].setFillColor (Color::Transparent);
        window.draw (piesaCrt.cerc[i]);
    }
    for (int i = 0; i < piesaCrt.numar.piese.tri; i++)
    {
        piesaCrt.triunghi[i].setOutlineColor (Color::Magenta);
        piesaCrt.triunghi[i].setOutlineThickness (1);
        piesaCrt.triunghi[i].setFillColor (Color::Transparent);
        window.draw (piesaCrt.triunghi[i]);
    }
}
void muta (sf::RenderWindow& window, desen& piesaCrt, punct pct)
{
    for (int i = 0; i < piesaCrt.numar.piese.lin; i++)
    {
        piesaCrt.linie[i][0].position += Vector2f (pct.x, pct.y);
        piesaCrt.linie[i][1].position += Vector2f (pct.x, pct.y);
    }
    for (int i = 0; i < piesaCrt.numar.piese.drept; i++)
    {
        Vector2f temp = piesaCrt.dreptunghi[i].getPosition ();
        piesaCrt.dreptunghi[i].setPosition (Vector2f (temp.x + pct.x, temp.y + pct.y));
    }
    for (int i = 0; i < piesaCrt.numar.piese.cerc; i++)
    {
        Vector2f temp = piesaCrt.cerc[i].getPosition ();
        piesaCrt.cerc[i].setPosition (Vector2f (temp.x + pct.x, temp.y + pct.y));
    }
    for (int i = 0; i < piesaCrt.numar.piese.tri; i++)
    {
        Vector2f temp = piesaCrt.triunghi[i].getPosition ();
        piesaCrt.triunghi[i].setPosition (Vector2f (temp.x + pct.x, temp.y + pct.y));
    }
}
void puneInGraf (sf::RenderWindow& window, graf g[][LATIME / 10], desen piesaCrt)
{
    colt crd = { LATIME, INALTIME, 0, 0 };
    for (int i = 0; i < piesaCrt.numar.piese.lin; i++)
    {
        crd.minim.x = min (crd.minim.x, min (piesaCrt.linie[i][0].position.x, piesaCrt.linie[i][1].position.x));
        crd.maxim.x = max (crd.maxim.x, max (piesaCrt.linie[i][0].position.x, piesaCrt.linie[i][1].position.x));
        crd.minim.y = min (crd.minim.y, min (piesaCrt.linie[i][0].position.y, piesaCrt.linie[i][1].position.x));
        crd.maxim.y = max (crd.maxim.y, max (piesaCrt.linie[i][0].position.y, piesaCrt.linie[i][1].position.y));
    }
    for (int i = 0; i < piesaCrt.numar.piese.drept; i++)
    {
        crd.minim.x = min (crd.minim.x, piesaCrt.dreptunghi[i].getGlobalBounds ().left);
        crd.maxim.x = max (crd.maxim.x, piesaCrt.dreptunghi[i].getGlobalBounds ().left + piesaCrt.dreptunghi[i].getGlobalBounds ().width);
        crd.minim.y = min (crd.minim.y, piesaCrt.dreptunghi[i].getGlobalBounds ().top);
        crd.maxim.y = max (crd.maxim.y, piesaCrt.dreptunghi[i].getGlobalBounds ().top + piesaCrt.dreptunghi[i].getGlobalBounds ().height);
    }
    for (int i = 0; i < piesaCrt.numar.piese.cerc; i++)
    {
        crd.minim.x = min (crd.minim.x, piesaCrt.cerc[i].getGlobalBounds ().left);
        crd.maxim.x = max (crd.maxim.x, piesaCrt.cerc[i].getGlobalBounds ().left + piesaCrt.cerc[i].getGlobalBounds ().width);
        crd.minim.y = min (crd.minim.y, piesaCrt.cerc[i].getGlobalBounds ().top);
        crd.maxim.y = max (crd.maxim.y, piesaCrt.cerc[i].getGlobalBounds ().top + piesaCrt.cerc[i].getGlobalBounds ().height);
    }
    for (int i = 0; i < piesaCrt.numar.piese.tri; i++)
    {
        crd.minim.x = min (crd.minim.x, piesaCrt.triunghi[i].getGlobalBounds ().left);
        crd.maxim.x = max (crd.maxim.x, piesaCrt.triunghi[i].getGlobalBounds ().left + piesaCrt.triunghi[i].getGlobalBounds ().width);
        crd.minim.y = min (crd.minim.y, piesaCrt.triunghi[i].getGlobalBounds ().top);
        crd.maxim.y = max (crd.maxim.y, piesaCrt.triunghi[i].getGlobalBounds ().top + piesaCrt.triunghi[i].getGlobalBounds ().height);
    }

    // test pentru coordonatele fiecarei forme
    // printf("%f %f \t %f %f\n", crd.minim.x, crd.minim.y, crd.maxim.x, crd.maxim.y);
}
void init (sf::RenderWindow& window, sf::RectangleShape baraMeniu, sf::RectangleShape baraParti, sf::RectangleShape separatori[], sf::Text titluri[])
{
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
        titluri[i].setPosition (Vector2f (LATIME / OBIECTE_MENIU * i + latTemp / 2, INALTIME / 80));
        // va trebui centrat mai bine
        titluri[i].setFillColor (Color::Blue);
        window.draw (titluri[i]);
    }
}
void iaCoord (char s[], desen& piesaCrt)
{
    int numar = 0;
    float temp;
    if (strchr (s, 'l'))
    {
        char* p = strtok (s, " ");
        while (p)
        {
            if (numar == 2)
                piesaCrt.linie[piesaCrt.numar.piese.lin][0].position.x = atof (p);
            else if (numar == 3)
                piesaCrt.linie[piesaCrt.numar.piese.lin][0].position.y = atof (p);
            else if (numar == 4)
                piesaCrt.linie[piesaCrt.numar.piese.lin][1].position.x = atof (p);
            else if (numar == 5)
                piesaCrt.linie[piesaCrt.numar.piese.lin++][1].position.y = atof (p);
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
                piesaCrt.cerc[piesaCrt.numar.piese.cerc].setPosition (temp, atof (p));
            else if (numar == 4)
                piesaCrt.cerc[piesaCrt.numar.piese.cerc++].setRadius (atof (p));
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
                piesaCrt.dreptunghi[piesaCrt.numar.piese.drept].setSize (Vector2f (temp, atof (p)));
            else if (numar == 4)
                temp = atof (p);
            else if (numar == 5)
                piesaCrt.dreptunghi[piesaCrt.numar.piese.drept++].setPosition (temp, atof (p));
            numar++;
            p = strtok (NULL, " ");
        }
    }
    else if (strchr (s, 't'))
    {
        char* p = strtok (s, " ");
        piesaCrt.triunghi[piesaCrt.numar.piese.tri].setPointCount (3);
        while (p)
        {
            if (numar == 2)
                temp = atof (p);
            else if (numar == 3)
                piesaCrt.triunghi[piesaCrt.numar.piese.tri].setPoint (0, Vector2f (temp, atof (p)));
            else if (numar == 4)
                temp = atof (p);
            else if (numar == 5)
                piesaCrt.triunghi[piesaCrt.numar.piese.tri].setPoint (1, Vector2f (temp, atof (p)));
            else if (numar == 6)
                temp = atof (p);
            else if (numar == 7)
                piesaCrt.triunghi[piesaCrt.numar.piese.tri++].setPoint (2, Vector2f (temp, atof (p)));
            numar++;
            p = strtok (NULL, " ");
        }
    }
}
void iaVarfuri (char s[], desen& piesaCrt)
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