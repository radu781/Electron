#include <SFML/Graphics.hpp>
#include "functii.h"

using namespace sf;
using namespace std;

float zoom = 1;    // Nivel de zoomPiesa, variabil
Desen piesaPerm[3 * NR_PIESE], piesaMeniu[3 * NR_PIESE], piesaMuta[3 * NR_PIESE], piesaFinal[3 * NR_PIESE];
Nod* grafCurent, * capGraf;
Lista* listaPiese, * capLista, * coadaLista;

int main ()
{
    int nrPieseValide = 0, meniuID = -1;
    bool amFisier = false;
    char fisierCrt[20] = {}, fisierTemp[20] = {};
    printf ("Alegeti una dintre optiunile din meniu:\n[1]: Deschideti fisier\n[2]: Salvati in fisier nou\n");
    scanf ("%d", &meniuID);
    switch (meniuID)
    {
    case 1:
    {
        printf ("Alegeti ce fisier sa deschideti: ");
        scanf ("%s", &fisierCrt);

        char tempFisier[20];
        strcpy (tempFisier, "Salvari\\");
        strcat (tempFisier, fisierCrt);
        strcat (tempFisier, ".txt");
        FILE* temp = fopen (tempFisier, "r");
        while (temp == NULL)
        {
            printf ("Fisierul nu exista, incercati alt nume: ");
            scanf ("%s", &fisierCrt);

            strcpy (tempFisier, "Salvari\\");
            strcat (tempFisier, fisierCrt);
            strcat (tempFisier, ".txt");

            temp = fopen (tempFisier, "r");
        }
        fclose (temp);
        deschide (grafCurent, capGraf, listaPiese, capLista, coadaLista, fisierCrt);
        break;
    }
    case 2:
    {
        printf ("Alegeti cum sa se numeasca fisierul pe care il salvati: ");
        scanf ("%s", &fisierCrt);

        strcpy (fisierTemp, "Salvari\\");
        strcat (fisierTemp, fisierCrt);
        strcat (fisierTemp, ".txt");

        char c[5];
        FILE* temp = fopen (fisierTemp, "r");
        while (temp != NULL)
        {
            printf ("Fisierul deja exista, doriti sa il suprascrieti? [d/n]\n");

            scanf ("%s", &c);
            while (c[0] != 'd' && c[0] != 'n')
            {
                printf ("Invalid\n");
                scanf ("%s", &c);
            }
            if (c[0] == 'n')
            {
                printf ("Dati un nume nou fisierului: ");
                scanf ("%s", &fisierCrt);

                strcpy (fisierTemp, "Salvari\\");
                strcat (fisierTemp, fisierCrt);
                strcat (fisierTemp, ".txt");

                temp = fopen (fisierTemp, "r");
            }
            else
            {
                printf ("Fisierul a fost suprascris\n");
                temp = NULL;
            }
        }
        if (c[0] == 'n')
            printf ("\"%s\" a fost creat\n", fisierTemp);
        break;
    }
    default:
        printf ("Proiect nou\n");
        break;
    }

    RenderWindow window (VideoMode (LATIME, INALTIME), "Proiect Electron", Style::Titlebar | Style::Close);

    // citeste toate piesele din toate fisierele
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < NR_PIESE && NUME_FISIERE[i + 1][j][0]; j++)
        {
            piesaPerm[NR_PIESE * i + j].numar = {};
            const char* numeCurent = numeFisier (i, j);
            FILE* tempFile = fopen (numeCurent, "r");
            if (tempFile != NULL)
            {
                piesaPerm[NR_PIESE * i + j] = citeste (tempFile);
                if (existaPiesa (piesaPerm[6 * i + j]))
                    nrPieseValide++;
            }
        }

    // pune piesele in bara de meniu
    for (int i = 0; i < 3 * NR_PIESE; i++)
        piesaMeniu[i] = muta (window, piesaPerm[i], Vector2i (LATIME / nrPieseValide * (i + .5), INALTIME / 13.5));

    bool anulat = false;
    int meniu = 2, luat = -1, nr = 0, ajutor = -1, ajutorVechi = -1;
    Punct coordLinie = {};
    Vertex linie[30][2];
    Cadran linInter = {};

    while (window.isOpen ())
    {

        Event event;
        window.clear ();

        init (window);

        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed || Keyboard::isKeyPressed (Keyboard::Escape))
                window.close ();

            for (int i = 0; i < NR_MENIU; i++)
                if (cursorInZona (window, { (float)LATIME / NR_MENIU * i, 0, (float)LATIME / NR_MENIU * (i + 1), INALTIME / 20 }))
                    if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Left))
                        meniu = i;

            switch (meniu)
            {
            case -1:
                break;
            case 0:
                for (int i = 0; i < nrPieseValide; i++)
                    if (cursorInZona (window, { (float)LATIME / nrPieseValide * i, INALTIME / 20, (float)LATIME / nrPieseValide * (i + 1), INALTIME / 10 }))
                        if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Left))
                        {
                            luat = i;
                            break;
                        }
                        else;
                    else
                        if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Right) && luat != -1)
                        {
                            luat = -1;
                            break;
                        }
                        else if (event.type == Event::MouseButtonReleased && luat != -1)
                        {
                            piesaMuta[luat] = muta (window, piesaPerm[luat], Mouse::getPosition (window));

                            if (!cursorInZona (window, { 0, 0, LATIME, INALTIME / 10 }))
                            {
                                puneInLista (listaPiese, capLista, coadaLista, piesaMuta[luat], piesaMuta[luat].id);
                                salveaza (grafCurent, capGraf, listaPiese, capLista, fisierCrt);
                            }
                            else
                                printf ("[WARN] nu puteti pune piesa in bara de meniuri\n");
                            luat = -1;
                            break;
                        }
                break;
            case 1:
                if (!cursorInZona (window, { 0, 0, LATIME, INALTIME / 10 }))
                {
                    Cadran temp = linInter;

                    linInter = trageLinii (window, event, piesaMuta);
                    // coordonatele anterioare sa nu fie egale cu cele noi
                    if (linInter.minim.x && linInter.minim.y && linInter.maxim.x && linInter.maxim.y && temp != linInter)
                        insereazaGraf (grafCurent, capGraf, linInter.minim, linInter.maxim);
                    salveaza (grafCurent, capGraf, listaPiese, capLista, fisierCrt);
                }
                break;
            case 2:
                for (int i = 0; i < NR_AJUTOR; i++)
                    if (cursorInZona (window, { (float)LATIME / NR_AJUTOR * i, INALTIME / 20, (float)LATIME / NR_AJUTOR * (i + 1), INALTIME / 10 }))
                        if (event.type == Event::MouseButtonPressed && Mouse::isButtonPressed (Mouse::Left))
                            ajutor = i;
                break;
            default:
                meniu = -1;
            }
            if (event.type == Event::KeyPressed && Keyboard::isKeyPressed (Keyboard::Add))
                zoom += .1;
            else if (event.type == Event::KeyPressed && Keyboard::isKeyPressed (Keyboard::Subtract))
                zoom -= .1;
            else if (event.type == Event::KeyPressed && Keyboard::isKeyPressed (Keyboard::Equal))
                zoom = 1;
        }

        switch (meniu)
        {
        case -1:
            break;
        case 0:
            // separatori bara de piese
            for (int i = 0; i < nrPieseValide - 1; i++)
            {
                RectangleShape rect;

                rect.setPosition (Vector2f (LATIME / nrPieseValide * (i + 1), INALTIME / 20));
                rect.setSize (Vector2f (LATIME_SEP - 1, INALTIME / 20));
                rect.setFillColor (Color::ROSU2);

                window.draw (rect);
            }
            // piesele in sine
            for (int i = 0; i < 3 * NR_PIESE; i++)
                deseneazaPiesa (window, piesaMeniu[i]);
            // muta piesa dupa cursor
            if (luat != -1)
            {
                piesaMuta[luat] = muta (window, piesaPerm[luat], Mouse::getPosition (window));
                //if (limitePiesa (piesaMuta[luat]).minim.y <= INALTIME / 10)
                //    zonaRosie (window, { 0, 0, LATIME, INALTIME / 10 });
                //for (int i = 0; i < 3 * NR_PIESE; i++)
                //    if (intersectie (piesaMuta[i], piesaMuta[luat]) && i != luat)
                //    {
                //        Cadran p1 = limitePiesa (piesaMuta[i]), p2 = limitePiesa (piesaMuta[luat]);
                //        zonaRosie (window, { min (p1.minim.x, p2.minim.x), min (p1.minim.y, p2.minim.y), max (p1.maxim.x, p2.maxim.x), max (p1.maxim.y, p2.maxim.y) });
                //    }
                deseneazaPiesa (window, piesaMuta[luat]);
            }
            break;
            ///////////////////////////////////////////////////////////////////////////
            /// 
            ///////////////////////////////////////////////////////////////////////////
        case 1:
        {
            Text text;
            Font font;
            font.loadFromFile ("Fonturi\\arial.ttf");
            text.setFont (font);
            text.setString ("Puteti trage legaturi intre piese dand click pe pozitia initiala si eliberand pe pozitia finala");
            text.setCharacterSize (14);

            FloatRect rect = text.getLocalBounds ();

            text.setOrigin (rect.left + (int)rect.width / 2, rect.top + (int)rect.height / 2);
            text.setPosition (Vector2f ((int)LATIME / 2, (int)INALTIME / 13));
            text.setFillColor (Color::Black);

            window.draw (text);

            // intre click si eliberare click trage linii
            if (linInter.minim.x && linInter.minim.y && linInter.maxim.x == 0 && linInter.maxim.y == 0)
            {
                if (Mouse::getPosition (window).y <= INALTIME / 10)
                    zonaRosie (window, { 0, 0, LATIME, INALTIME / 10 });

                Vertex lin[2][2];

                lin[0][0].position = Vector2f (linInter.minim.x, linInter.minim.y);
                lin[0][1].position = Vector2f (Mouse::getPosition (window).x, linInter.minim.y);
                lin[1][0].position = Vector2f (Mouse::getPosition (window).x, linInter.minim.y);
                lin[1][1].position = Vector2f (Mouse::getPosition (window).x, Mouse::getPosition (window).y);

                lin[0][0].color = Color::VERDE1;
                lin[0][1].color = Color::GALBEN1;
                lin[1][0].color = Color::GALBEN1;
                lin[1][1].color = Color::ROSU1;

                window.draw (lin[0], 2, Lines);
                window.draw (lin[1], 2, Lines);
            }
            break;
        }
        case 2:
        {
            if (ajutor != ajutorVechi)
                switch (ajutor)
                {
                case -1:
                    break;
                case 0:
                    printf ("[Plasare piese]\nPentru a plasa o piesa, trebuie mai intai sa accesati meniul \"Piese\", sa dati click pe piesa dorita, sa mutati cursorul in locul unde doriti sa adaugati piesa si sa eliberati click\n\n");
                    break;
                case 1:
                    printf ("[Plasare circuit]\nPentru a conecta doua piese cu ajutorul unei legaturi trebuie sa accesati meniul \"Legaturi\", dupa care sa dati click in cercul din jurul varfului unei piese, sa mutati cursorul in interiorul altui cerc (sau in apropierea acestuia) si sa eliberati click. Punctul de \"pornire\" al legaturii va fi colorat cu verde, iar cel de \"sosire\" cu rosu\n\n");
                    break;
                case 2:
                    printf ("[Salvare circuit]\nSalvarea circuitului se efectueaza automat, la fiecare mutare/inserare de piese sau legaturi in fisierul precizat la lansarea programului dupa ce a fost aleasa a doua optiune\n\n");
                    break;
                case 3:
                    printf ("[Deschidere]\nDeschiderea unui fisier nou se poate efectua doar la lansarea programului, alegand prima optiune si specificand numele unui fisier existent\n\n");
                    break;
                case 4:
                {
                    for (int i = 0; i < 3; i++)
                        for (int j = 0; j < NR_PIESE && NUME_FISIERE[i + 1][j][0]; j++)
                        {
                            char var[50] = "Descriere\\";

                            strcat (var, NUME_FISIERE[i + 1][j]);
                            strcat (var, ".txt");

                            printf ("\n");
                            FILE* tempFile = fopen (var, "r");
                            char sir[100];
                            while (!feof (tempFile))
                            {
                                fgets (sir, 100, tempFile);
                                printf ("%s", sir);
                            }
                            printf ("\n\n");
                            fclose (tempFile);
                        }
                    break;
                }
                default:
                    break;
                }
            ajutorVechi = ajutor;

            Text text[NR_AJUTOR];
            Font font;
            font.loadFromFile ("Fonturi\\arial.ttf");
            for (int i = 0; i < NR_AJUTOR; i++)
            {
                text[i].setFont (font);
                text[i].setCharacterSize (14);
                text[i].setString (NUME_AJUTOR[i]);

                FloatRect rect = text[i].getLocalBounds ();

                text[i].setOrigin (rect.left + (int)rect.width / 2, rect.top + (int)rect.height / 2);
                text[i].setPosition (Vector2f ((int)LATIME / NR_AJUTOR * (i + .5), (int)INALTIME / 13));
                text[i].setFillColor (Color::Black);

                window.draw (text[i]);
            }
            for (int i = 0; i < NR_AJUTOR - 1; i++)
            {
                RectangleShape rect;
                rect.setPosition (Vector2f (LATIME / NR_AJUTOR * (i + 1), INALTIME / 20));
                rect.setSize (Vector2f (LATIME_SEP - 1, INALTIME / 20));
                rect.setFillColor (Color::ROSU2);
                window.draw (rect);
            }
            break;
        }
        default: 
            printf ("aici\n");
            break;
        }

        int totPiese = 0, totLinii = 0;
        //deschide (grafCurent, capGraf, listaPiese, capLista, coadaLista, fisierCrt);
        //afiseazaGraf (grafCurent, capGraf);
        //afiseazaLista (listaPiese, capLista);
        restituie (window, grafCurent, capGraf, listaPiese, capLista, coadaLista, piesaPerm, piesaFinal, linie, totPiese, totLinii);

        // piesele in urma mutarii
        for (int i = 0; i < 3 * NR_PIESE; i++)
        {
            piesaFinal[i] = zoomPiesa (window, piesaFinal[i]);
            deseneazaPiesa (window, piesaFinal[i]);
        }
        // legaturile in urma mutarii
        for (int i = 0; i < totLinii; i++)
        {
            Vertex temp[2][2];

            temp[0][0] = Vector2f (linie[i][0].position.x, linie[i][0].position.y);
            temp[0][1] = Vector2f (linie[i][1].position.x, linie[i][0].position.y);
            temp[1][0] = Vector2f (linie[i][1].position.x, linie[i][0].position.y);
            temp[1][1] = Vector2f (linie[i][1].position.x, linie[i][1].position.y);

            temp[0][0].color = Color::VERDE1;
            temp[0][1].color = Color::GALBEN1;
            temp[1][0].color = Color::GALBEN1;
            temp[1][1].color = Color::ROSU1;

            window.draw (temp[0], 2, Lines);
            window.draw (temp[1], 2, Lines);
        }
        window.display ();
    }
    return 0;
}
