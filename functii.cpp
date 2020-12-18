#include <SFML/Graphics.hpp>
#include "functii.h"

using namespace sf;

void citeste (FILE* file, desen& piesa)
{
    char sir[100];
    bool amCoord = false;   // verifica daca am ajuns la coordonate

    while (!feof (file))
    {
        fgets (sir, 100, file);
        if (sir[0] == '#')
            continue;
        if (amCoord)
            piesa.nr = iaCoord (sir, piesa);
        if (strstr (sir, "coordonate"))
            amCoord = true;
    }
}
void deseneazaPiesa (sf::RenderWindow& window, desen piesa)
{
    for (int i = 0; i < piesa.nr.lin; i++)
    {
        piesa.linie[i][0].color = Color::Magenta;
        piesa.linie[i][1].color = Color::Magenta;
        window.draw (piesa.linie[i], 2, Lines);
    }
    for (int i = 0; i < piesa.nr.drept; i++)
    {
        piesa.dreptunghi[i].setOutlineColor (Color::Magenta);
        piesa.dreptunghi[i].setOutlineThickness (1);
        piesa.dreptunghi[i].setFillColor (Color::Transparent);
        window.draw (piesa.dreptunghi[i]);
    }
    for (int i = 0; i < piesa.nr.cerc; i++)
    {
        piesa.cerc[i].setOutlineColor (Color::Magenta);
        piesa.cerc[i].setOutlineThickness (1);
        piesa.cerc[i].setFillColor (Color::Transparent);
        window.draw (piesa.cerc[i]);
    }
    for (int i = 0; i < piesa.nr.tri; i++)
    {
        piesa.triunghi[i].setOutlineColor (Color::Magenta);
        piesa.triunghi[i].setOutlineThickness (1);
        piesa.triunghi[i].setFillColor (Color::Transparent);
        window.draw (piesa.triunghi[i]);
    }
}
void muta (sf::RenderWindow& window, desen& piesa, punct pct)
{
    for (int i = 0; i < piesa.nr.lin; i++)
    {
        piesa.linie[i][0].position += Vector2f (pct.x, pct.y);
        piesa.linie[i][1].position += Vector2f (pct.x, pct.y);
    }
    for (int i = 0; i < piesa.nr.drept; i++)
    {
        Vector2f temp = piesa.dreptunghi[i].getPosition ();
        piesa.dreptunghi[i].setPosition (Vector2f (temp.x + pct.x, temp.y + pct.y));
    }
    for (int i = 0; i < piesa.nr.cerc; i++)
    {
        Vector2f temp = piesa.cerc[i].getPosition ();
        piesa.cerc[i].setPosition (Vector2f (temp.x + pct.x, temp.y + pct.y));
    }
    for (int i = 0; i < piesa.nr.tri; i++)
    {
        Vector2f temp = piesa.triunghi[i].getPosition ();
        piesa.triunghi[i].setPosition (Vector2f (temp.x + pct.x, temp.y + pct.y));
    }
}
void puneInGraf (sf::RenderWindow& window, graf g[][LATIME / 10], desen piesa)
{
    colt crd = { LATIME, INALTIME, 0, 0 };
    for (int i = 0; i < piesa.nr.lin; i++)
    {
        crd.minim.x = std::min (crd.minim.x, std::min (piesa.linie[i][0].position.x, piesa.linie[i][1].position.x));
        crd.maxim.x = std::max (crd.maxim.x, std::max (piesa.linie[i][0].position.x, piesa.linie[i][1].position.x));
        crd.minim.y = std::min (crd.minim.y, std::min (piesa.linie[i][0].position.y, piesa.linie[i][1].position.x));
        crd.maxim.y = std::max (crd.maxim.y, std::max (piesa.linie[i][0].position.y, piesa.linie[i][1].position.y));
    }
    for (int i = 0; i < piesa.nr.drept; i++)
    {
        crd.minim.x = std::min (crd.minim.x, piesa.dreptunghi[i].getGlobalBounds ().left);
        crd.maxim.x = std::max (crd.maxim.x, piesa.dreptunghi[i].getGlobalBounds ().left + piesa.dreptunghi[i].getGlobalBounds ().width);
        crd.minim.y = std::min (crd.minim.y, piesa.dreptunghi[i].getGlobalBounds ().top);
        crd.maxim.y = std::max (crd.maxim.y, piesa.dreptunghi[i].getGlobalBounds ().top + piesa.dreptunghi[i].getGlobalBounds ().height);
    }
    for (int i = 0; i < piesa.nr.cerc; i++)
    {
        crd.minim.x = std::min (crd.minim.x, piesa.cerc[i].getGlobalBounds ().left);
        crd.maxim.x = std::max (crd.maxim.x, piesa.cerc[i].getGlobalBounds ().left + piesa.cerc[i].getGlobalBounds ().width);
        crd.minim.y = std::min (crd.minim.y, piesa.cerc[i].getGlobalBounds ().top);
        crd.maxim.y = std::max (crd.maxim.y, piesa.cerc[i].getGlobalBounds ().top + piesa.cerc[i].getGlobalBounds ().height);
    }
    for (int i = 0; i < piesa.nr.tri; i++)
    {
        crd.minim.x = std::min (crd.minim.x, piesa.triunghi[i].getGlobalBounds ().left);
        crd.maxim.x = std::max (crd.maxim.x, piesa.triunghi[i].getGlobalBounds ().left + piesa.triunghi[i].getGlobalBounds ().width);
        crd.minim.y = std::min (crd.minim.y, piesa.triunghi[i].getGlobalBounds ().top);
        crd.maxim.y = std::max (crd.maxim.y, piesa.triunghi[i].getGlobalBounds ().top + piesa.triunghi[i].getGlobalBounds ().height);
    }

    // test pentru coordonatele fiecarei forme
    printf("%f %f \t %f %f\n", crd.minim.x, crd.minim.y, crd.maxim.x, crd.maxim.y);
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
numarPiese iaCoord (char s[], desen& piesa)
{
    int numar = 0;
    float temp;
    if (strchr (s, 'l'))
    {
        char* p = strtok (s, " ");
        while (p)
        {
            if (numar == 2)
                piesa.linie[piesa.nr.lin][0].position.x = atof (p);
            else if (numar == 3)
                piesa.linie[piesa.nr.lin][0].position.y = atof (p);
            else if (numar == 4)
                piesa.linie[piesa.nr.lin][1].position.x = atof (p);
            else if (numar == 5)
                piesa.linie[piesa.nr.lin++][1].position.y = atof (p);
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
                piesa.cerc[piesa.nr.cerc].setPosition (temp, atof (p));
            else if (numar == 4)
                piesa.cerc[piesa.nr.cerc++].setRadius (atof (p));
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
                piesa.dreptunghi[piesa.nr.drept].setSize (Vector2f (temp, atof (p)));
            else if (numar == 4)
                temp = atof (p);
            else if (numar == 5)
                piesa.dreptunghi[piesa.nr.drept++].setPosition (temp, atof (p));
            numar++;
            p = strtok (NULL, " ");
        }
    }
    else if (strchr (s, 't'))
    {
        char* p = strtok (s, " ");
        piesa.triunghi[piesa.nr.tri].setPointCount (3);
        while (p)
        {
            if (numar == 2)
                temp = atof (p);
            else if (numar == 3)
                piesa.triunghi[piesa.nr.tri].setPoint (0, Vector2f (temp, atof (p)));
            else if (numar == 4)
                temp = atof (p);
            else if (numar == 5)
                piesa.triunghi[piesa.nr.tri].setPoint (1, Vector2f (temp, atof (p)));
            else if (numar == 6)
                temp = atof (p);
            else if (numar == 7)
                piesa.triunghi[piesa.nr.tri++].setPoint (2, Vector2f (temp, atof (p)));
            numar++;
            p = strtok (NULL, " ");
        }
    }
    return piesa.nr;
}
void salveaza ()
{

}
void deschide ()
{

}