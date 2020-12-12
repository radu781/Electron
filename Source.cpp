/*
*   fisierele sa nu contina doar coordonate pentru linii
*   ar fi bine si cu cercuri, dreptunghiuri etc
* 
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
#define DIMENSIUNE 10

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
    int indice;
    punct ini, fin, muta;
};
struct numarPiese
{
    int linii, drept, cerc, triun;
};
struct desen
{
    Vertex linie[DIMENSIUNE][2];
    RectangleShape dreptunghi[DIMENSIUNE];
    CircleShape cerc[DIMENSIUNE];
    Vertex triunghi[DIMENSIUNE][3];
};
// sa organizez structurile
numarPiese iaCoord (char s[], Vertex linie[][2], RectangleShape dreptunghi[], CircleShape cerc[], Vertex triunghi[][3])
{
    // ia coordonatele din fisier in functie de tipul obiectelor: linie, cerc, dreptunghi, triunghi
    char* p = strtok (s, " ");
    int x, numar = 0;
    numarPiese nr;
    nr.linii = nr.drept = nr.cerc = nr.triun = 0;
    if (strchr (s, 'l'))
        while (p)
        {
            if (numar == 2)
                linie[nr.linii][0].position.x = atoi (p);
            else if (numar == 3)
                linie[nr.linii][0].position.y = atoi (p);
            else if (numar == 4)
                linie[nr.linii][1].position.x = atoi (p);
            else if (numar == 5)
                linie[nr.linii++][1].position.y = atoi (p);
            numar++;
            p = strtok (NULL, " ");
        }
    else if (strchr (s, 'c'))
        while (p)
        {
            if (numar == 2)
                x = atoi (p);
            else if (numar == 3)
                cerc[nr.cerc].setPosition (x, atoi (p));
            else if (numar == 4)
                cerc[nr.cerc++].setRadius (atoi (p));
            numar++;
            p = strtok (NULL, " ");
        }
    else if (strchr (s, 'd'))
        while (p)
        {
            if (numar == 2)
                x = atoi (p);
            else if (numar == 3)
                dreptunghi[nr.drept].setSize (Vector2f (x, atoi (p)));
            else if (numar == 4)
                x = atoi (p);
            else if (numar == 5)
                dreptunghi[nr.drept++].setPosition (x, atoi (p));
            numar++;
            p = strtok (NULL, " ");
        }
    else if (strchr (s, 't'))
        while (p)
        {
            if (numar == 2)
                x = atoi (p);
            else if (numar == 3)
                triunghi[nr.triun][0] = Vertex (Vector2f (x, atoi (p)));
            else if (numar == 4)
                x = atoi (p);
            else if (numar == 5)
                triunghi[nr.triun][1] = Vertex (Vector2f (x, atoi (p)));
            else if (numar == 6)
                x = atoi (p);
            else if (numar == 7)
                triunghi[nr.triun++][2] = Vertex (Vector2f (x, atoi (p)));
            numar++;
            p = strtok (NULL, " ");
        }
    return nr;
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
void citeste (FILE* file, Vertex linie[][2], RectangleShape dreptunghi[], CircleShape cerc[], Vertex triunghi[][3], numarPiese &nr)
{
    piesa fig;
    fig.amCoord = 0;
    char sir[65];
    int i = 0;

    while (!feof (file))
    {
        // dimensiunea e constanta, sau eventual inmultita cu un scalar
        // adauga scalar
        fgets (sir, 65, file);
        if (sir[0] == '#')
            continue;
        if (fig.amCoord)
            nr = iaCoord (sir, linie, dreptunghi, cerc, triunghi);
        if (strstr (sir, "coordonate"))
            fig.amCoord = true;
    }
}
void deseneazaPiesa (RenderWindow &window, Vertex linie[][2], RectangleShape dreptunghi[], CircleShape cerc[], Vertex triunghi[][3], numarPiese& nr)
{
    for (int i = 0; i < nr.linii; i++)
        window.draw (linie[i], 2, Lines);
    for (int i = 0; i < nr.drept; i++)
        window.draw (dreptunghi[i]);
    for (int i = 0; i < nr.cerc; i++)
        window.draw (cerc[i]);
    for (int i = 0; i < nr.triun; i++)
        window.draw (triunghi[i], 3, Lines);
}
void adauga (Vertex linie[][2], piesa coord)
{
    // trebuie pentru toate tipurile de piesa
    for (int i = 0; i < 6; i++)
    {
        std::cout << coord.indice << '\n';
        // problema de mai tarziu
        linie[i][0].position += Vector2f (coord.muta.x, coord.muta.y);
        linie[i][1].position += Vector2f (coord.muta.x, coord.muta.y);
    }
}
int main ()
{   
    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close);    

    piesa temp;

    Vertex linii[10][2];
    RectangleShape drept[10];
    CircleShape cerc[10];
    Vertex tri[10][3];
    numarPiese nr;
    nr.linii = nr.drept = nr.cerc = nr.triun = 0;
    citeste (dio, linii, drept, cerc, tri, nr);
    temp.muta.x = 350;
    temp.muta.y = 150;
    adauga (linii, temp);

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

        deseneazaPiesa (window, linii, drept, cerc, tri, nr);

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