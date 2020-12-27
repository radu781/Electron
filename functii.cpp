#include <SFML/Graphics.hpp>
#include "functii.h"

using namespace sf;
using namespace std;

Desen citeste (FILE* file)
{
    Desen piesaCrt;
    piesaCrt.numar = {};
    for (int i = 0; i < DIMENSIUNE; i++)
        piesaCrt.varfuri[i] = {};
    char sir[100];
    bool amCoord = false, amVarf = false;

    while (!feof (file))
    {
        fgets (sir, 100, file);
        if (sir[0] == '#')
            continue;   // considera liniile ce incep cu '#' ca fiind comentarii
        if (amVarf)
            iaVarfuri (piesaCrt, sir);
        else if (amCoord && !isalpha (sir[0]))
            iaCoord (piesaCrt, sir);
        if (strstr (sir, "varfuri"))
            amVarf = true;
        else if (strstr (sir, "coordonate"))
        {
            amCoord = true;
            amVarf = false;
        }
    }
    return piesaCrt;
}
void deseneazaPiesa (RenderWindow& window, Desen piesaCrt)
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
Desen muta (RenderWindow& window, Desen& piesaCrt, Vector2i poz)
{
    Desen deMutat;
    deMutat.numar.cerc = piesaCrt.numar.cerc;
    deMutat.numar.drept = piesaCrt.numar.drept;
    deMutat.numar.lin = piesaCrt.numar.lin;
    deMutat.numar.tri = piesaCrt.numar.tri;

    for (int i = 0; i < piesaCrt.numar.lin; i++)
    {
        Vector2f temp0 = piesaCrt.linie[i][0].position;
        Vector2f temp1 = piesaCrt.linie[i][1].position;

        deMutat.linie[i][0].position = Vector2f (temp0.x + poz.x, temp0.y + poz.y);
        deMutat.linie[i][1].position = Vector2f (temp1.x + poz.x, temp1.y + poz.y);
    }
    for (int i = 0; i < piesaCrt.numar.drept; i++)
    {
        Vector2f temp0 = { piesaCrt.dreptunghi[i].getGlobalBounds ().width, piesaCrt.dreptunghi[i].getGlobalBounds ().height };
        Vector2f temp1 = { piesaCrt.dreptunghi[i].getGlobalBounds ().left, piesaCrt.dreptunghi[i].getGlobalBounds ().top };

        deMutat.dreptunghi[i].setSize (Vector2f (temp0.x, temp0.y));
        deMutat.dreptunghi[i].setPosition (Vector2f (temp1.x + poz.x, temp1.y + poz.y));
    }
    for (int i = 0; i < piesaCrt.numar.cerc; i++)
    {
        Vector2f temp = piesaCrt.cerc[i].getPosition ();

        deMutat.cerc[i].setRadius (piesaCrt.cerc[i].getRadius ());
        deMutat.cerc[i].setPosition (Vector2f (temp.x + poz.x, temp.y + poz.y));
    }
    for (int i = 0; i < piesaCrt.numar.tri; i++)
    {
        Vector2f temp0 = piesaCrt.triunghi[i].getPoint (0);
        Vector2f temp1 = piesaCrt.triunghi[i].getPoint (1);
        Vector2f temp2 = piesaCrt.triunghi[i].getPoint (2);

        deMutat.triunghi[i].setPointCount (3);
        deMutat.triunghi[i].setPoint (0, Vector2f (temp0.x + poz.x, temp0.y + poz.y));
        deMutat.triunghi[i].setPoint (1, Vector2f (temp1.x + poz.x, temp1.y + poz.y));
        deMutat.triunghi[i].setPoint (2, Vector2f (temp2.x + poz.x, temp2.y + poz.y));
    }
    return deMutat;
}
void puneInGraf (RenderWindow& window, char graf[INALTIME][LATIME], Desen piesaCrt/*inca o piesa aici*/)
{
    Cadran crd = { LATIME, INALTIME, 0, 0 };

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

        FloatRect tempDrept = titluri[i].getLocalBounds ();

        // centreaza titlul intre barele separatoare
        titluri[i].setOrigin (tempDrept.left + tempDrept.width / 2, tempDrept.top + tempDrept.height / 2);
        titluri[i].setPosition (Vector2f (LATIME / OBIECTE_MENIU * (i + .5), INALTIME / 40));
        titluri[i].setFillColor (Color::Blue);

        window.draw (titluri[i]);
    }
}
void iaCoord (Desen& piesaCrt, char s[])
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
void iaVarfuri (Desen& piesaCrt, char s[])
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
void trageLinii (RenderWindow& window, Punct& t, Vertex linie[][2], int& nr, bool& pressed)
{
    Cadran tempFereastra = { 0, 0, LATIME, INALTIME };
    if (pressed && Mouse::isButtonPressed (Mouse::Right) && cursorInZona (window, tempFereastra))
        pressed = false;

    Cadran tempCadran = { 0, 0, LATIME, INALTIME / 10 };
    if (!cursorInZona (window, tempCadran) && cursorInZona (window, tempFereastra) && Mouse::isButtonPressed (Mouse::Left))
    {
        if (!pressed)
        {
            t = { (float)Mouse::getPosition (window).x, (float)Mouse::getPosition (window).y };

            pressed = true;
        }
        // daca primul si al doilea click nu au aceeasi pozitie
        else if (!(t.x == (float)Mouse::getPosition (window).x && t.y == (float)Mouse::getPosition (window).y))
        {
            linie[nr][0].position.x = t.x;
            linie[nr][0].position.y = t.y;
            linie[nr][1].position.x = (float)Mouse::getPosition (window).x;
            linie[nr++][1].position.y = t.y;

            linie[nr][0].position.x = (float)Mouse::getPosition (window).x;
            linie[nr][0].position.y = t.y;
            linie[nr][1].position.x = (float)Mouse::getPosition (window).x;
            linie[nr++][1].position.y = (float)Mouse::getPosition (window).y;

            pressed = false;
            sleep (milliseconds (200));
        }
        sleep (milliseconds (200));
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

        temp[0][0].color = Color::Color (143, 191, 143, 255);
        temp[0][1].color = Color::Color (191, 191, 143, 255);
        temp[1][0].color = Color::Color (191, 191, 143, 255);
        temp[1][1].color = Color::Color (191, 143, 143, 255);

        window.draw (temp[0], 2, Lines);
        window.draw (temp[1], 2, Lines);
    }
}
bool cursorInZona (RenderWindow& window, Cadran zona)
{
    return (Mouse::getPosition (window).x >= zona.minim.x && Mouse::getPosition (window).x <= zona.maxim.x &&
        Mouse::getPosition (window).y >= zona.minim.y && Mouse::getPosition (window).y <= zona.maxim.y);
}
void zonaRosie (RenderWindow& window, Cadran zona, float& viteza)
{
    RectangleShape tempDrept;
    viteza += .075;

    tempDrept.setPosition (Vector2f (zona.minim.x, zona.minim.y));
    tempDrept.setSize (Vector2f (zona.maxim.x - zona.minim.x, zona.maxim.y - zona.minim.y));

    int culoare = (int)((sin (viteza) * 255) + 255) / 2;
    tempDrept.setFillColor (Color::Color (255, 63, 63, ((min (culoare, 191) + max (culoare, 63)) / 2)));
    window.draw (tempDrept);
}