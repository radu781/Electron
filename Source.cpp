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

#define INALTIME 600
#define LATIME 800
#define OBIECTE_MENIU 8
#define LATIME_SEP 3
#define DIMENSIUNE 6

FILE* bat = fopen ("Piese\\Simple\\baterie.txt", "r");
FILE* dio = fopen ("Piese\\Simple\\dioda.txt", "r");
FILE* rez = fopen ("Piese\\Simple\\rezistor.txt", "r");
FILE* ter = fopen ("Piese\\Simple\\termistor.txt", "r");
FILE* sur = fopen ("Piese\\Simple\\sursa.txt", "r");
FILE* noT = fopen ("Piese\\Logice\\not.txt", "r");
FILE* anD = fopen ("Piese\\Logice\\and.txt", "r");
// mod de a face un loop aici
using namespace sf;
using namespace std;
struct punct
{
    float x, y;
};
struct colt
{
    punct minim, maxim;
};
struct numarPiese
{
    int lin, drept, cerc, tri;
};
struct desen
{
    Vertex linie[DIMENSIUNE][2];
    RectangleShape dreptunghi[DIMENSIUNE];
    CircleShape cerc[DIMENSIUNE];
    ConvexShape triunghi[DIMENSIUNE];
    numarPiese nr;
    // poate cu liste in loc de vectori
};
struct graf
{
    // piesa: centrul piesei
    // varf: nodurile de legatura intre piesa si circuit
    // nod: nod de legatura intre circuite
    // tip: tipul de piesa (D: dioda, N: poarta not, B: baterie etc)
    punct nod, varf[10], piesa;
    char tip;
};

numarPiese iaCoord (char s[], desen& piesa)
{
    // ia coordonatele din fisier in functie de tipul obiectelor: linie, cerc, dreptunghi, triunghi
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
void init (RenderWindow &window, RectangleShape baraMeniu, RectangleShape baraParti, RectangleShape separatori[], Text titluri[])
{
    // initializeaza fereastra cu bara de meniu si piese, mereu fixe
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
void citeste (FILE* file, desen& piesa)
{
    // citeste un fisier si retine forma
    char sir[65];
    int i = 0;
    bool amCoord = false;

    while (!feof (file))
    {
        // dimensiunea e constanta, sau eventual inmultita cu un scalar
        // adauga scalar
        fgets (sir, 65, file);
        if (sir[0] == '#')
            continue;
        if (amCoord)
            piesa.nr = iaCoord (sir, piesa);
        if (strstr (sir, "coordonate"))
            amCoord = true;
    }
}
void deseneazaPiesa (RenderWindow& window, desen piesa)
{
    // deseneaza piesa (deocamdata doar) din citeste ()
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
void muta (RenderWindow& window, desen& piesa, punct pct)
{
    // muta piesa in directia x y
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
void salveaza ();
void deschide ();
void puneInGraf (RenderWindow& window, graf g[][LATIME / 10], desen piesa)
{
    colt crd = {LATIME, INALTIME, 0, 0};
    for (int i = 0; i < piesa.nr.lin; i++)
    {
        crd.minim.x = min (crd.minim.x, min (piesa.linie[i][0].position.x, piesa.linie[i][1].position.x));
        crd.maxim.x = max (crd.maxim.x, max (piesa.linie[i][0].position.x, piesa.linie[i][1].position.x));
        crd.minim.y = min (crd.minim.y, min (piesa.linie[i][0].position.y, piesa.linie[i][1].position.x));
        crd.maxim.y = max (crd.maxim.y, max (piesa.linie[i][0].position.y, piesa.linie[i][1].position.y));
    }
    for (int i = 0; i < piesa.nr.drept; i++)
    {
        crd.minim.x = min (crd.minim.x, piesa.dreptunghi[i].getGlobalBounds ().left);
        crd.maxim.x = max (crd.maxim.x, piesa.dreptunghi[i].getGlobalBounds ().left + piesa.dreptunghi[i].getGlobalBounds ().width);
        crd.minim.y = min (crd.minim.y, piesa.dreptunghi[i].getGlobalBounds ().top);
        crd.maxim.y = max (crd.maxim.y, piesa.dreptunghi[i].getGlobalBounds ().top + piesa.dreptunghi[i].getGlobalBounds ().height);
    }
    for (int i = 0; i < piesa.nr.cerc; i++)
    {
        crd.minim.x = min (crd.minim.x, piesa.cerc[i].getGlobalBounds ().left);
        crd.maxim.x = max (crd.maxim.x, piesa.cerc[i].getGlobalBounds ().left + piesa.cerc[i].getGlobalBounds ().width);
        crd.minim.y = min (crd.minim.y, piesa.cerc[i].getGlobalBounds ().top);
        crd.maxim.y = max (crd.maxim.y, piesa.cerc[i].getGlobalBounds ().top + piesa.cerc[i].getGlobalBounds ().height);
    }
    for (int i = 0; i < piesa.nr.tri; i++)
    {
        crd.minim.x = min (crd.minim.x, piesa.triunghi[i].getGlobalBounds ().left);
        crd.maxim.x = max (crd.maxim.x, piesa.triunghi[i].getGlobalBounds ().left + piesa.triunghi[i].getGlobalBounds ().width);
        crd.minim.y = min (crd.minim.y, piesa.triunghi[i].getGlobalBounds ().top);
        crd.maxim.y = max (crd.maxim.y, piesa.triunghi[i].getGlobalBounds ().top + piesa.triunghi[i].getGlobalBounds ().height);
    }
    CircleShape cerc[3];
    cerc[0].setPosition (Vector2f (crd.minim.x, crd.minim.y));
    cerc[0].setRadius (3);
    cerc[1].setPosition (Vector2f (crd.maxim.x, crd.maxim.y));
    cerc[1].setRadius (3);
    cerc[2].setPosition (Vector2f ((crd.minim.x + crd.maxim.x) / 2, (crd.minim.y + crd.maxim.y) / 2));
    cerc[2].setRadius (3);
    cerc[2].setFillColor (Color::Red);
    window.draw (cerc[0]);
    window.draw (cerc[1]);
    window.draw (cerc[2]);
    cout << crd.minim.x << " " << crd.minim.y << "\t\t" << crd.maxim.x << " " << crd.maxim.y << '\n';
}
int main ()
{
    ContextSettings settings;
    settings.antialiasingLevel = 0;
    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close, settings);

    desen* piesaNoua = new desen[6];
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
        //init (window, meniu, parti, sep, nume);

        //deseneazaPiesa (window, piesaNoua[0]);
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
        //window.draw (test);

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
        // de verificat
        window.display ();
    }
    return 0;
}