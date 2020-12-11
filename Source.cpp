/*
    facut:
    bara de sus cu piesele
    bara de meniu
    citire piese din fisier

    mai trebuie facut:
    drag and drop la bara cu piese - pana pe 12 decembrie
    (biblioteci, piese, legaturi, optiuni, lupa, ajutor) informatii cand dai click - pana pe 19 decembrie

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
#define MENU_ITEMS 8
#define LATIME_SEP 5

FILE* bat = fopen ("Piese\\baterie.txt", "r");
FILE* dio = fopen ("Piese\\dioda.txt", "r");
FILE* ter = fopen ("Piese\\termistor.txt", "r");
FILE* rez = fopen ("Piese\\rezistor.txt", "r");

//float x1 = -1, y1 = -1.0, x2 = -1, y2 = -1;
bool pressed;
int i;
using namespace sf;
struct dimensiune
{
    int lat, lung;
    char cLat[20], cLung[20];
};
struct piesa
{
    bool dimens, forma;
    dimensiune dim;
};
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
    for (int i = 0; i < MENU_ITEMS - 1; i++)
    {
        separatori[i].setPosition (Vector2f (LATIME / MENU_ITEMS * (i + 1), 0));
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
    for (int i = 0; i <= MENU_ITEMS; i++)
    {
        titluri[i].setFont (fontMeniu);
        titluri[i].setCharacterSize (15);
        float temp = titluri[i].getLocalBounds ().width;
        titluri[i].setPosition (Vector2f (LATIME / MENU_ITEMS * i + temp / 2, INALTIME / 80));
        // justificare 80
        titluri[i].setFillColor (Color::Blue);
        window.draw (titluri[i]);
    }
}
void readFile (FILE* file, RectangleShape forma[])
{
    piesa figura;
    figura.dimens = figura.forma = 0;
    char sir[50];

    while (!feof (file))
    {
        // dimensiunea e constanta, sau eventual inmultita cu un scalar
        // adauga scalar
        fgets (sir, 50, file);
        if (sir[0] == '#')
            continue;
        if (figura.dimens && !isalpha (sir[0]))
        {
            // retine latimea si lungimea liniei, mereu independente
            char* p = strtok (sir, " ");
            int numar = 0;
            while (p)
            {
                numar++;
                if (numar == 2)
                    figura.dim.lat = atoi (p);
                else if (numar == 3)    
                    figura.dim.lung = atoi (p);
                p = strtok (NULL, " ");
            }
            forma[i++].setSize (Vector2f (figura.dim.lat, figura.dim.lung));
        }
        if (figura.forma)
        {
            // retine pozitia in coordonate x si y, mereu dependente
            char* p = strtok (sir, " ");
            int numar = 0, piesaX = 0, piesaY = 0;
            while (p)
            {
                numar++;
                if (numar == 2)
                {
                    if (p[0] == 'h')
                    {
                        // poate de facut functie aici
                        // daca este in functie de inaltime
                        if (!strchr (p, '-') && !strchr (p, '+'))
                            piesaY = INALTIME / atoi (p + 2);
                        else
                            if (strchr (p, '+'))
                            {
                                char temp[5] = { 0, 0, 0, 0, 0 };
                                strncpy (temp, p + 2, strchr (p, '+') - p - 2);
                                piesaY = INALTIME / atoi (temp);      // deimpartitul
                                piesaY += atoi (strchr (p, '+') + 1); // ce e dupa + sau -
                            }
                            else if (strchr (p, '-'))
                            {
                                char temp[5] = { 0, 0, 0, 0, 0 };
                                strncpy (temp, p + 2, strchr (p, '-') - p - 2);
                                piesaY = INALTIME / atoi (temp);      // deimpartitul
                                piesaY -= atoi (strchr (p, '-') + 1); // ce e dupa + sau -
                            }
                    }
                    else
                        piesaX = atoi (p);
                }
                else if (numar == 3)
                {
                    if (p[0] == 'h') 
                    {
                        // daca e in functie de inaltime
                        if (!strchr (p, '-') && !strchr (p, '+'))
                            piesaY = INALTIME / atoi (p + 2);
                        else
                            if (strchr (p, '+'))
                            {
                                char temp[5] = { 0, 0, 0, 0, 0 };
                                strncpy (temp, p + 2, strchr (p, '+') - p - 2);
                                piesaY = INALTIME / atoi (temp);      // deimpartitul
                                piesaY += atoi (strchr (p, '+') + 1); // ce e dupa + sau -
                            }
                            else if (strchr (p, '-'))
                            {
                                char temp[5] = { 0, 0, 0, 0, 0 };
                                strncpy (temp, p + 2, strchr (p, '-') - p - 2);
                                piesaY = INALTIME / atoi (temp);      // deimpartitul
                                piesaY -= atoi (strchr (p, '-') + 1); // ce e dupa + sau -
                            }
                    }
                    else
                        piesaY = atoi (p);
                }
                p = strtok (NULL, " ");
            }
            forma[i++].setPosition (Vector2f (piesaX, piesaY));
        }
        if (strstr (sir, "dimensiune"))
            figura.dimens = 1;
        if (strstr (sir, "forma"))
        {
            i = 0;
            figura.forma = 1;
            figura.dimens = 0;
        }
    }
}
int main ()
{   
    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close);    

    RectangleShape formaCurenta[10];
    readFile (bat, formaCurenta);

    while (window.isOpen ())
    {
        Event event;
        window.clear ();
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

            // deseneaza unde dai click
        }

        RectangleShape meniu, parti, sep[MENU_ITEMS + 1];
        Text nume[MENU_ITEMS + 1];
        init (window, meniu, parti, sep, nume);

        for (int i = 0; i < 10; i++)
        {
            formaCurenta[i].setFillColor (Color::Blue);
            window.draw (formaCurenta[i]);
        }

        /*for (int i = 0; i <= MENU_ITEMS; i++)
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
        }*/
        // de verificat
        window.display ();
    }
    return 0;
}