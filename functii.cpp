#include <SFML/Graphics.hpp>
#include <cmath>
#include "functii.h"

using namespace sf;
using namespace std;

int power (int x, unsigned int y)
{
    int res = 1;
    while (y > 0) 
    { 
        if (y & 1)
            res = res * x;
        y = y >> 1;
        x = x * x;
    }
    return res;
}
Desen citeste (FILE* file)
{
    Desen piesaCrt;
    piesaCrt.numar = {};
    piesaCrt.id[0] = piesaCrt.id[1] = piesaCrt.id[2] = piesaCrt.id[3] = 0;
    for (int i = 0; i < DIMENSIUNE; i++)
        piesaCrt.varfuri[i] = {};
    char sir[100];
    bool amCoord = false, amVarf = false, amId = false;

    while (!feof (file))
    {
        fgets (sir, 100, file);
        if (sir[0] == '#')
            continue;
        if (amId)
        {
            amId = false;
            for (int i = 0; i < strlen (sir) && sir[i] != '\n'; i++)
                piesaCrt.id[strlen (piesaCrt.id)] = sir[i];
            piesaCrt.id[strlen (piesaCrt.id)] = 0;
        }
        if (amVarf && !strstr (sir, "coordonate"))
            iaVarfuri (piesaCrt, sir);
        else if (amCoord)
        {
            amVarf = false;
            iaCoord (piesaCrt, sir);
        }
        if (strstr (sir, "id"))
            amId = true;
        else if (strstr (sir, "varfuri"))
            amVarf = true;
        else if (strstr (sir, "coordonate"))
        {
            amCoord = true;
            amVarf = false;
        }
    }
    printf ("am citit %s\n", piesaCrt.id);
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
    Cadran crd = { 0, 0, LATIME, INALTIME / 10 };
    float vit = 0.5;
    deMutat.numar.lin = -1;     // piesa nu exista

    if (cursorInZona (window, crd))
    {
        //zonaRosie (window, crd, vit);
    }
    else
    {
        // copiez numarul maxim de obiecte ale piesei
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
            FloatRect tempDrept = piesaCrt.dreptunghi[i].getGlobalBounds ();

            deMutat.dreptunghi[i].setSize (Vector2f (tempDrept.width, tempDrept.height));
            deMutat.dreptunghi[i].setPosition (Vector2f (tempDrept.left + poz.x, tempDrept.top + poz.y));
        }
        for (int i = 0; i < piesaCrt.numar.cerc; i++)
        {
            Vector2f tempCerc = piesaCrt.cerc[i].getPosition ();

            deMutat.cerc[i].setRadius (piesaCrt.cerc[i].getRadius ());
            deMutat.cerc[i].setPosition (Vector2f (tempCerc.x + poz.x, tempCerc.y + poz.y));
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
    }
    strcpy (deMutat.id, piesaCrt.id);

    return deMutat;
}
Cadran limitePiesa (Desen piesaCrt)
{
    Cadran crd = { LATIME, INALTIME, 0, 0 };

    for (int i = 0; i < piesaCrt.numar.lin; i++)
    {
        Vector2f tempLin0 = piesaCrt.linie[i][0].position;
        Vector2f tempLin1 = piesaCrt.linie[i][1].position;

        crd.minim.x = min (crd.minim.x, min (tempLin0.x, tempLin1.x));
        crd.maxim.x = max (crd.maxim.x, max (tempLin0.x, tempLin1.x));
        crd.minim.y = min (crd.minim.y, min (tempLin0.y, tempLin1.x));
        crd.maxim.y = max (crd.maxim.y, max (tempLin0.y, tempLin1.y));
    }
    for (int i = 0; i < piesaCrt.numar.drept; i++)
    {
        FloatRect tempDrept = piesaCrt.dreptunghi[i].getGlobalBounds ();

        crd.minim.x = min (crd.minim.x, tempDrept.left);
        crd.maxim.x = max (crd.maxim.x, tempDrept.left + tempDrept.width);
        crd.minim.y = min (crd.minim.y, tempDrept.top);
        crd.maxim.y = max (crd.maxim.y, tempDrept.top + tempDrept.height);
    }
    for (int i = 0; i < piesaCrt.numar.cerc; i++)
    {
        FloatRect tempCerc = piesaCrt.cerc[i].getGlobalBounds ();

        crd.minim.x = min (crd.minim.x, tempCerc.left);
        crd.maxim.x = max (crd.maxim.x, tempCerc.left + tempCerc.width);
        crd.minim.y = min (crd.minim.y, tempCerc.top);
        crd.maxim.y = max (crd.maxim.y, tempCerc.top + tempCerc.height);
    }
    for (int i = 0; i < piesaCrt.numar.tri; i++)
    {
        FloatRect tempTri = piesaCrt.triunghi[i].getGlobalBounds ();

        crd.minim.x = min (crd.minim.x, tempTri.left);
        crd.maxim.x = max (crd.maxim.x, tempTri.left + tempTri.width);
        crd.minim.y = min (crd.minim.y, tempTri.top);
        crd.maxim.y = max (crd.maxim.y, tempTri.top + tempTri.height);
    }
    return crd;
}
void puneInLista (Lista*& listaCrt, Lista*& capLista, Lista*& coadaLista, Desen piesaCrt, char id[])
{
    Cadran temp = limitePiesa (piesaCrt);

    insereazaLista (listaCrt, capLista, coadaLista, { (temp.minim.x + temp.maxim.x) / 2, (temp.minim.y + temp.maxim.y) / 2 }, id);
    afiseazaLista (listaCrt, capLista);
}
void init (RenderWindow& window, Desen piesaNoua[])
{
    RectangleShape baraMeniu, baraParti, separatori[NR_MENIU + 1];

    // bara de meniu
    baraMeniu.setSize (Vector2f (LATIME, INALTIME / 20));
    baraMeniu.setFillColor (Color::Color (202, 250, 254, 255));
    baraMeniu.setPosition (0, 0);
    window.draw (baraMeniu);

    // bara de parti/piese
    baraParti.setSize (Vector2f (LATIME, INALTIME / 20));
    baraParti.setFillColor (Color::Color (151, 202, 239, 255));
    baraParti.setPosition (0, INALTIME / 20);
    window.draw (baraParti);

    // separatori pentru meniu
    for (int i = 0; i < NR_MENIU - 1; i++)
    {
        separatori[i].setPosition (Vector2f (LATIME / NR_MENIU * (i + 1), 0));
        separatori[i].setSize (Vector2f (LATIME_SEP, INALTIME / 20));
        separatori[i].setFillColor (Color::Color (252, 68, 69, 255));
        window.draw (separatori[i]);
    }

    // text meniuri
    Text numeTitluri[NR_MENIU + 1];
    Font fontMeniu;
    fontMeniu.loadFromFile ("Fonturi\\arial.ttf");
    for (int i = 0; i < NR_MENIU; i++)
        numeTitluri[i].setString (NUME_TITLURI[i]);

    for (int i = 0; i <= NR_MENIU; i++)
    {
        numeTitluri[i].setFont (fontMeniu);
        numeTitluri[i].setCharacterSize (16);

        FloatRect tempDrept = numeTitluri[i].getLocalBounds ();

        // centreaza titlul intre barele separatoare
        numeTitluri[i].setOrigin (tempDrept.left + (int)tempDrept.width / 2, tempDrept.top + (int)tempDrept.height / 2);
        numeTitluri[i].setPosition (Vector2f ((int)LATIME / NR_MENIU * (i + .5), (int)INALTIME / 40));
        numeTitluri[i].setFillColor (Color::Black);

        window.draw (numeTitluri[i]);
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
    // de testat
    else if (strchr (s, 'x'))
    {
        char* p = strtok (s, " "), poz[4][2] = { 0, 0, 0, 0, 0, 0, 0, 0 };  // [0]: pozitie, [1]: numar
        int i = 0;
        while (p)
        {
            if (numar == 2)
                poz[i][0] = p[0];
            else if (numar == 3)
            {
                if (strchr (p, '^'))
                    poz[i][1] = power (2, atoi (p + 2));
                else poz[i][1] = atoi (p);
                piesaCrt.numar.lin += poz[i + 1][1];
            }
            numar++;
            p = strtok (NULL, " ");
        }
        int temp1 = piesaCrt.linie[piesaCrt.numar.lin - 1][0].position.y;
        int temp2 = piesaCrt.linie[piesaCrt.numar.lin - 1][1].position.y;
        int totalLin = poz[0][1] + poz[1][1] + poz[2][1] + poz[3][1];
        for (int j = 0; j < totalLin; j++)
        {
            piesaCrt.linie[j + piesaCrt.numar.lin - 1][0].position.x;
            piesaCrt.linie[j + piesaCrt.numar.lin - 1][0].position.y = (float)(temp2 - temp1) / totalLin * (j + .5);
            piesaCrt.linie[j + piesaCrt.numar.lin - 1][0].position.x;
            piesaCrt.linie[j + piesaCrt.numar.lin - 1][1].position.y = (float)(temp2 - temp1) / totalLin * (j + .5);
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
void salveaza (Nod* grafCrt, Nod* capGraf, Lista* listaCrt, Lista* capLista, char text[])
{
    char temp[50] = "Salvari\\";
    strcat (temp, text);
    strcat (temp, ".txt");

    FILE* fisier = fopen (temp, "r");
    if (fisier)
    {
        printf ("Am suprascris fisierul \"%s\".\n", temp);
        fclose (fisier);
    }

    FILE* fisier1 = fopen (temp, "w");
    fprintf (fisier1, "# In acest fisier sunt salvate piesele si legaturile corespunzatoare circuitului\n# Primele seturi de coordonate reprezinta \"punctul de plecare\" al legaturii, iar cele ce urmeaza dupa \":\" sunt \"puncte de sosire\", asemenea celor dintr-o lista de adiacenta.\n# Dupa \"=====\" urmeaza lista creata urmata de identificatorul piesei.\n\n");

    grafCrt = capGraf;
    while (capGraf != NULL)
    {
        Nod* temp1 = capGraf;
        fprintf (fisier1, "(%.1f, %.1f): ", temp1->coord.x, temp1->coord.y);
        temp1 = temp1->drp;
        while (temp1 != NULL)
        {
            fprintf (fisier1, "(%.1f %.1f), ", temp1->coord.x, temp1->coord.y);
            temp1 = temp1->drp;
        }
        fprintf (fisier1, "\n");
        capGraf = capGraf->jos;
    }

    fprintf (fisier1, "\n=====\n\n");

    listaCrt = capLista;
    while (listaCrt)
    {
        fprintf (fisier1, "(%.1f, %.1f), {%s}\n", listaCrt->coord.x, listaCrt->coord.y, listaCrt->id);
        listaCrt = listaCrt->urm;
    }

    fclose (fisier1);
}
void deschide (Nod*& grafCrt, Nod*& capGraf, Lista*& listaCrt, Lista*& capLista, Lista*& coadaLista, char text[])
{
    char file[50] = { "Salvari\\" };
    bool egal = false;
    strcat (file, text);
    strcat (file, ".txt");
    FILE* fisier = fopen (file, "r");

    if (fisier == NULL)
    {
        printf ("Fisierul \"%s\" nu exista\n", file);
        return;
    }

    while (!feof (fisier))
    {
        char sir[200], c[3]{};
        int numar = 0;
        float temp1, temp2, temp3, temp4;

        fgets (sir, 200, fisier);
        char* p = strtok (sir, "():,={} ");
        if (strchr (sir, '#'))
            continue;
        if (strchr (sir, '='))
            egal = true;
        if (!egal)
            while (p)
            {
                if (numar == 0)
                    temp1 = atof (p);
                else if (numar == 1)
                    temp2 = atof (p);
                else if (numar > 1 && numar % 2 == 0)
                    temp3 = atof (p);
                else if (numar > 1 && numar % 2)
                    temp4 = atof (p);
                if (numar > 1 && numar % 2 == 1)
                    insereazaGraf (grafCrt, capGraf, { temp1, temp2 }, { temp3, temp4 });
                numar++;
                p = strtok (NULL, "():,={} ");
            }
        else
        {
            while (p)
            {
                if (numar == 0)
                    temp1 = atof (p);
                else if (numar == 1)
                    temp2 = atof (p);
                else if (numar == 2)
                    strcpy (c, p);
                numar++;
                p = strtok (NULL, "():,={} ");
            }
            if (c)
                insereazaLista (listaCrt, capLista, coadaLista, { temp1, temp2 }, c);
        }
    }

    grafCrt = capGraf;
    listaCrt = capLista;

    fclose (fisier);
}
void trageLinii (RenderWindow& window, Punct& t, Vertex linie[][2], int& nr)
{
    Cadran tempFereastra = { 0, 0, LATIME, INALTIME };
    static bool pressed = false;

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
            sf::sleep (milliseconds (200));
        }
        sf::sleep (milliseconds (200));
    }
    else if (pressed && !Mouse::isButtonPressed (Mouse::Right))
    {
        Vertex temp[2][2];
        Vector2i tempPoz = Mouse::getPosition (window);

        temp[0][0].position.x = t.x;
        temp[0][0].position.y = t.y;
        temp[0][1].position.x = tempPoz.x;
        temp[0][1].position.y = t.y;

        temp[1][0].position.x = tempPoz.x;
        temp[1][0].position.y = t.y;
        temp[1][1].position.x = tempPoz.x;
        temp[1][1].position.y = tempPoz.y;

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
void zonaRosie (RenderWindow& window, Cadran zona)
{
    RectangleShape tempDrept;
    static float viteza = 0;
    viteza += .075;
    tempDrept.setPosition (Vector2f (zona.minim.x, zona.minim.y));
    tempDrept.setSize (Vector2f (zona.maxim.x - zona.minim.x, zona.maxim.y - zona.minim.y));

    int culoare = (int)((sin (viteza) * 255) + 255) / 2;
    tempDrept.setFillColor (Color::Color (255, 63, 63, ((min (culoare, 191) + max (culoare, 63)) / 2)));
    window.draw (tempDrept);
}
bool existaPiesa (Desen piesaCrt)
{
    return (piesaCrt.numar.lin + piesaCrt.numar.cerc + piesaCrt.numar.drept + piesaCrt.numar.tri);
}
char* numeFisier (int linie, int coloana)
{
    char var[50] = { "Piese\\" };

    strcat (var, NUME_FISIERE[0][linie]);
    strcat (var, "\\");
    strcat (var, NUME_FISIERE[linie + 1][coloana]);
    strcat (var, ".txt");

    FILE* file = fopen (var, "r");
    if (file == NULL)
        printf ("Nu exista fisierul \"%s\", piesa nu va putea fi folosita.\n", var);
    char* nume = var;
    
    return nume;
}
Desen dragAndDrop (RenderWindow& window, Cadran zona)
{
    Desen temp;
    temp.numar = {};
    for (int i = 0; i < DIMENSIUNE; i++)
        temp.varfuri[i] = {};

    if (cursorInZona (window, zona) && Mouse::isButtonPressed (Mouse::Left));

    return temp;
}
void insereazaLista (Lista*& listaCrt, Lista*& capLista, Lista*& coadaLista, Punct coord, char id[])
{
    Lista* temp = new Lista;
    temp->coord = coord;
    temp->urm = NULL;
    strcpy (temp->id, id);
    if (capLista == NULL)
        capLista = coadaLista = temp;
    else
    {
        coadaLista->urm = temp;
        coadaLista = coadaLista->urm;
    }
}
void stergeLista ()
{
}
void afiseazaLista (Lista* listaCrt, Lista* capLista)
{
    listaCrt = capLista;
    while (listaCrt)
    {
        printf ("(%.2f, %.2f), {%s}\n", listaCrt->coord.x, listaCrt->coord.y, listaCrt->id);
        listaCrt = listaCrt->urm;
    }
}
void mutaLista (Lista*& listaCrt, Lista* capLista, Punct vechi, Punct nou)
{
    listaCrt = capLista;
    while (listaCrt)
    {
        if (listaCrt->coord.x == vechi.x && listaCrt->coord.y == vechi.y)
            listaCrt->coord = nou;
        listaCrt = listaCrt->urm;
    }
}
void insereazaGraf (Nod*& grafCrt, Nod*& capGraf, Punct src, Punct dest)
{
    Nod* temp = capGraf;
    if (capGraf == NULL)
    {
        temp = new Nod;
        temp->coord = src;
        temp->jos = temp->drp = NULL;
        temp->drp = new Nod;
        temp->drp->coord = dest;
        temp->drp->drp = temp->drp->jos = NULL;

        capGraf = temp;
    }
    else
    {
        while (capGraf->jos != NULL && capGraf->coord.x != src.x && capGraf->coord.y != src.y)
            capGraf = capGraf->jos;
        if (capGraf == NULL)
        {
            capGraf = new Nod;
            capGraf->coord = src;
            capGraf->jos = capGraf->drp = NULL;
            capGraf->drp = new Nod;
            capGraf->drp->coord = dest;
            capGraf->drp->drp = capGraf->drp->jos = NULL;
        }
        else if (capGraf->coord.x == src.x && capGraf->coord.y == src.y)
        {
            while (capGraf->drp != NULL)
                capGraf = capGraf->drp;
            capGraf->drp = new Nod;
            capGraf->drp->coord = dest;
            capGraf->drp->drp = capGraf->drp->jos = NULL;
        }
        else
        {
            capGraf->jos = new Nod;
            capGraf->jos->coord = src;
            capGraf->jos->jos = NULL;
            capGraf->jos->drp = new Nod;
            capGraf->jos->drp->coord = dest;
            capGraf->jos->drp->drp = capGraf->jos->drp->drp = NULL;
        }
        capGraf = temp;
    }
}
void stergeGraf ()
{
}
void afiseazaGraf (Nod* grafCrt, Nod* cap)
{
    grafCrt = cap;
    while (cap != NULL)
    {
        Nod* temp = cap;
        printf ("(%.1f, %.1f): ", temp->coord.x, temp->coord.y);
        temp = temp->drp;
        while (temp != NULL)
        {
            printf ("(%.1f, %.1f), ", temp->coord.x, temp->coord.y);
            temp = temp->drp;
        }
        cap = cap->jos;
        printf ("\b\b \n");
    }
    cap = grafCrt;
}
void mutaGraf (Nod* grafCrt, Nod* cap, Punct vechi, Punct nou)
{
    grafCrt = cap;
    while (grafCrt != NULL)
    {
        Nod* temp = grafCrt;
        if (grafCrt->coord.x == vechi.x && grafCrt->coord.y == vechi.y)
            grafCrt->coord = nou;
        while (grafCrt != NULL)
        {
            if (grafCrt->coord.x == vechi.x && grafCrt->coord.y == vechi.y)
                grafCrt->coord = nou;
            grafCrt = grafCrt->drp;
        }
        grafCrt = temp;
        grafCrt = grafCrt->jos;
    }
    cap = grafCrt;
}