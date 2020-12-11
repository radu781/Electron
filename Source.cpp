/*
    facut:
    bara de sus cu piesele
    bara de meniu
    citire piese din fisier

    mai trebuie facut:
    drag and drop la bara cu piese - pana pe 12 decembrie
    informatii cand dai click pe meniuri - pana pe 19 decembrie
    trasare circuit, rotire si dimensionare piese, editare continut piese - pana pe 9 ianuarie
    salvare si deschidere - pana pe 16 ianuarie
    demo

    optional, dar ar fi bine sa facem:
    corectitudine circuit
    calcule fizice
    creativitate
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstring>

#define INALTIME 600
#define LATIME 800
#define OBIECTE_MENIU 8
#define LATIME_SEP 3

FILE* bat = fopen ("Piese\\baterie.txt", "r");
FILE* dio = fopen ("Piese\\dioda.txt", "r");
FILE* ter = fopen ("Piese\\termistor.txt", "r");
FILE* rez = fopen ("Piese\\rezistor.txt", "r");

using namespace sf;
struct punct
{
    int x, y;
};
struct piesa
{
    bool amCoord;
    punct ini, fin;
};
void pie (char* p, char semn, int& piesa)
{
    // nu face nimic deocamdata
    // daca coordonatele sunt in functie de inaltime si au + sau -
    // exemplu: h/13+15
    if (strchr (p, semn))
    {
        char temp[5] = { 0, 0, 0, 0, 0 };
        strncpy (temp, p + 2, strchr (p, semn) - p - 2);
        piesa = INALTIME / atoi (temp);      
        piesa += atoi (strchr (p, semn) + 1);
    }
}
void iaCoord (char s[], piesa &figura)
{
    char* p = strtok (s, " ");
    int numar = 0;
    while (p)
    {
        if (numar == 1)
            figura.ini.x = atoi (p);
        else if (numar == 2)
            figura.ini.y = atoi (p);
        else if (numar == 3)
            figura.fin.x = atoi (p);
        else if (numar == 4)
            figura.fin.y = atoi (p);
        numar++;
        p = strtok (NULL, " ");
    }
}
// structuri pentru parti/nume
void init (RenderWindow &window, RectangleShape baraMeniu, RectangleShape baraParti, RectangleShape separatori[], Text titluri[])
{
    baraMeniu.setSize (Vector2f (LATIME, INALTIME / 20));
    baraMeniu.setFillColor (Color::Green);
    baraMeniu.setPosition (0, 0);
    window.draw (baraMeniu);
    // bara de meniu
    baraParti.setSize (Vector2f (LATIME, INALTIME / 20));
    baraParti.setFillColor (Color::Cyan);
    baraParti.setPosition (0, INALTIME / 20);
    window.draw (baraParti);
    // bara de parti/piese
    for (int i = 0; i < OBIECTE_MENIU - 1; i++)
    {
        separatori[i].setPosition (Vector2f (LATIME / OBIECTE_MENIU * (i + 1), 0));
        separatori[i].setSize (Vector2f (LATIME_SEP, INALTIME / 20));
        separatori[i].setFillColor (Color::Red);
        window.draw (separatori[i]);
    }
    // separatori pentru meniu
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
    // text meniu
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
void citeste (FILE* file, Vertex linie[][2])
{
    piesa fig;
    fig.amCoord = 0;
    char sir[60];
    int i = 0;

    while (!feof (file))
    {
        // dimensiunea e constanta, sau eventual inmultita cu un scalar
        // adauga scalar
        fgets (sir, 50, file);
        if (sir[0] == '#')
            continue;
        if (fig.amCoord)
        {
            iaCoord (sir, fig);
            linie[i][0] = Vertex (Vector2f (fig.ini.x, fig.ini.y), Color::Yellow);
            linie[i++][1] = Vertex (Vector2f (fig.fin.x, fig.fin.y), Color::Yellow);
        }
        if (strstr (sir, "coordonate"))
            fig.amCoord = true;
    }
}
int main ()
{   
    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close);    

    Vertex linii[10][2];
    citeste (bat, linii);

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

        for (int i = 0; i < 10; i++)
            window.draw (linii[i], 2, Lines);

        /*for (int i = 0; i <= OBIECTE_MENIU; i++)
        {
            // verifica pe ce meniu dai click
            if (Mouse::isButtonPressed (Mouse::Left) && !pressed)
            {
                if (Mouse::getPosition (window).x >= LATIME / OBIECTE_MENIU * i && Mouse::getPosition (window).x <= LATIME / OBIECTE_MENIU * (i + 1))
                    if (Mouse::getPosition (window).y >= 0 && Mouse::getPosition (window).y <= INALTIME / 20)
                    {
                        std::cout << "esti in dreptunghiul " << i + 1 << '\n';
                        std::cout << "x: " << Mouse::getPosition (window).x << " y: " << Mouse::getPosition (window).y << '\n';
                    }
                pressed = 1;
            }
            else pressed = 0;
        }*/
        // de verificat
        window.display ();
    }
    return 0;
}