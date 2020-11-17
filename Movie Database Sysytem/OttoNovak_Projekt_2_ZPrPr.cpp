#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N_MAX 100

typedef struct name {
    char first[N_MAX];
    char last[N_MAX];
} NAME;

typedef struct actor {
    struct name name;
    int birth_y;
    struct actor* next;
} ACTOR;

typedef struct movie {
    char title[N_MAX];
    int year;
    struct name regizor;
    struct actor* actory;
    struct movie* next;
} MOVIE;

typedef struct herci {
    struct name name;
    int birth_y;
} HERCI;


MOVIE* nacitaj()
{
    MOVIE* akt_f = NULL;
    MOVIE* base = NULL;
    MOVIE* pomocFilm = NULL;

    ACTOR* akt_a = NULL;
    ACTOR* pomocAct = NULL;
    char nazov[N_MAX], c;
    int i = 0;

    FILE* fr;       // otvorim subor a preverim ci sa podravilo
    if ((fr = fopen("filmy.txt", "r")) == NULL)
    {
        printf("Subor sa nepodarilo otvorit\n");
        return 0;
    }
      // pre kazdy nacitany nazov priradim pamat pre novy film a nacitam vlastnosti
    while (fgets(nazov, N_MAX, fr) != NULL)
    {
        nazov[strlen(nazov) - 1] = '\0';

        akt_f = (MOVIE*)malloc(sizeof(MOVIE));  
        akt_f->actory = NULL;
        strcpy(akt_f->title, nazov);    
        fscanf(fr, "%d", &akt_f->year);
        fscanf(fr, "%s", akt_f->regizor.first);
        fscanf(fr, "%s", akt_f->regizor.last);
        akt_f->next = NULL;

        fscanf(fr, "%c", &c);
        fscanf(fr, "%c", &c);
        // nasledne nacitam hercov
        while (c == '*')
        {   
            akt_a = (ACTOR*)malloc(sizeof(ACTOR));
            fscanf(fr, "%s", akt_a->name.first);
            fscanf(fr, "%s", akt_a->name.last);
            fscanf(fr, "%d", &akt_a->birth_y);
            akt_a->next = NULL;

            if (akt_f->actory == NULL)       // preverim ak film ma priradenich hercov
                akt_f->actory = akt_a;       // pripojim hercov

            else                              // v pripade ak uz mam jedneho herca
            {                                 // pripojim noveho herca k predoslemu
                pomocAct = akt_f->actory;
                while (pomocAct->next != NULL)
                    pomocAct = pomocAct->next;

                pomocAct->next = akt_a;
            }

            fscanf(fr, "%c", &c);
            if (fscanf(fr, "%c", &c) == EOF)
                break;
            if (c != '*')
                ungetc(c, fr);

        }

        if (base == NULL)
            base = akt_f;
        else
        {
            pomocFilm = base;
            while (pomocFilm->next != NULL)
                pomocFilm = pomocFilm->next;

            pomocFilm->next = akt_f;

        }

    }
    return base;
}

void vypis(MOVIE* base)
{
    MOVIE* akt_f;
    ACTOR* akt_a;
    akt_f = base;

    while (akt_f != NULL)       // prejdem vsetky filmy
    {
        printf("%s (%d) %s %s\n", akt_f->title, akt_f->year, akt_f->regizor.first, akt_f->regizor.last);


        akt_a = akt_f->actory;
        printf("    Hraju:");
        while (akt_a != NULL)
        {
            printf(" %s %s (%d)", akt_a->name.first, akt_a->name.last, akt_a->birth_y);
            if (akt_a->next != NULL)
                printf(",");
            else
                printf("\n");

            akt_a = akt_a->next;
        }
        akt_f = akt_f->next;
    }
}


void pridaj(MOVIE* base)
{
    MOVIE* akt_f;
    MOVIE* pomocFilm;
    ACTOR* akt_a;
    ACTOR* pomocAct;
    char nazov[N_MAX], c;

    scanf("%c", &c);
    fgets(nazov, N_MAX, stdin);
    nazov[strlen(nazov) - 1] = '\0';

    akt_f = (MOVIE*)malloc(sizeof(MOVIE));
    akt_f->actory = NULL;
    strcpy(akt_f->title, nazov);
    scanf("%d", &akt_f->year);
    scanf("%s", akt_f->regizor.first);
    scanf("%s", akt_f->regizor.last);
    akt_f->next = NULL;

    scanf("%s", nazov);

    while (nazov[0] != '*')  // je * ked uz nemam citat hercov
    {
        akt_a = (ACTOR*)malloc(sizeof(ACTOR));
        strcpy(akt_a->name.first, nazov);
        scanf("%s", akt_a->name.last);
        scanf("%d", &akt_a->birth_y);
        akt_a->next = NULL;

        if (akt_f->actory == NULL)
            akt_f->actory = akt_a;

        else
        {
            pomocAct = akt_f->actory;

            while (pomocAct->next != NULL)
                pomocAct = pomocAct->next;

            pomocAct->next = akt_a;
        }

        scanf("%s", nazov);
    }



    if (base == NULL)
        base = akt_f;
    else
    {
        pomocFilm = base;
        while (pomocFilm->next != NULL)
            pomocFilm = pomocFilm->next;

        pomocFilm->next = akt_f;
    }


}


MOVIE* vymaz(MOVIE* base)
{

    MOVIE* akt_f;
    MOVIE* pomocFilm;
    ACTOR* akt_a;
    ACTOR* pomocAct;

    char nazov[N_MAX], c;
    int i = 0;

    scanf("%c", &c);
    fgets(nazov, N_MAX, stdin);
    nazov[strlen(nazov) - 1] = '\0';


    akt_f = base;

    // ak je prvy film
    if (strcmp(akt_f->title, nazov) == 0)
    {
        akt_a = akt_f->actory;
        while (akt_a != NULL)
        {
            pomocAct = akt_a;
            akt_a = akt_a->next;
            free(pomocAct);
        }
        pomocFilm = akt_f;
        akt_f = akt_f->next;
        free(pomocFilm);

        return akt_f;
    }

    // film v strede zoznamu
    while (akt_f->next != NULL)
    {
        if (strcmp(akt_f->next->title, nazov) == 0)
        {
            akt_a = akt_f->next->actory;
            while (akt_a != NULL)
            {
                pomocAct = akt_a;
                akt_a = akt_a->next;
                free(pomocAct);
            }

            pomocFilm = akt_f->next;
            akt_f->next = akt_f->next->next;
            free(pomocFilm);

        }
        akt_f = akt_f->next;
    }



    return base;
}


void filmy(MOVIE* base)
{
    MOVIE* akt_f;
    ACTOR* akt_a;
    char meno[N_MAX], priezvisko[N_MAX];
    int sw = 0;

    scanf("%s %s", meno, priezvisko);

    akt_f = base;
    while (akt_f != NULL)
    {
        sw = 0;
        akt_a = akt_f->actory;

        while (akt_a != NULL)
        {
            if (strcmp(akt_a->name.first, meno) == 0 && strcmp(akt_a->name.last, priezvisko) == 0)
                sw = 1;
            akt_a = akt_a->next;
        }


        if (sw)
            printf("%s (%d)\n", akt_f->title, akt_f->year);

        akt_f = akt_f->next;
    }


}


void herci(MOVIE* base)
{
    MOVIE* akt_f;
    ACTOR* actor1, * actor2;
    char nazov1[N_MAX], nazov2[N_MAX], c;

    scanf("%c", &c); 
    fgets(nazov1, N_MAX, stdin);  
    nazov1[strlen(nazov1) - 1] = '\0';


    fgets(nazov2, N_MAX, stdin);    
    nazov2[strlen(nazov2) - 1] = '\0';

    akt_f = base;
    while (akt_f != NULL)     // hladam filmy
    {
        if (strcmp(akt_f->title, nazov1) == 0)
            actor1 = akt_f->actory;
        if (strcmp(akt_f->title, nazov2) == 0)
            actor2 = akt_f->actory;

        akt_f = akt_f->next;
    }


    while (actor1->next != NULL)
    {

        while (actor2->next != NULL)
        {
            if ((strcmp(actor1->name.first, actor2->name.first) == 0) &&
                (strcmp(actor1->name.last, actor2->name.last) == 0))

                printf("%s %s (%d)\n", actor1->name.first, actor1->name.last, actor1->birth_y);
            actor2 = actor2->next;
        }
        actor1 = actor1->next;
    }
}


void rok(MOVIE* base)
{
    MOVIE* akt_f;
    ACTOR* akt_a;
    int rok, i, m = 0, sw;
    HERCI poleH[N_MAX];

    akt_f = base;
    scanf("%d", &rok);
    while (akt_f != NULL)
    {
        if (akt_f->year == rok)
        {
            akt_a = akt_f->actory;
            while (akt_a != NULL)       
            {
                sw = 0;
                for (i = 0; i < m; i++)
                {   // prejdem cele pole a haldam ci uz mam tochto herca, ak nie ho pridam na konieec
                    if ((strcmp(akt_a->name.first, poleH[i].name.first) == 0)
                        && (strcmp(akt_a->name.last, poleH[i].name.last) == 0)
                        && poleH[i].birth_y == akt_a->birth_y)
                        sw = 1;
                }
                if (sw != 1)
                {
                    strcpy(poleH[m].name.first, akt_a->name.first);
                    strcpy(poleH[m].name.last, akt_a->name.last);
                    poleH[m].birth_y = akt_a->birth_y;
                    m++;

                }

                akt_a = akt_a->next;
            }

        }

        akt_f = akt_f->next;
    }
   
    /*  NEVIEM PRECO NEFUNGUJE USPORIADANIA
    HERCI pomocP;

    for (i = 0; i < m - 1; i++)
    {       // preverim ci maju rovnake meno a prezvisko
        if (strcmp(poleH[i].name.first, poleH[i + 1].name.first == 0)
            && strcmp(poleH[i].name.last, poleH[i + 1].name.last) == 0)
            if (poleH[i].birth_y > poleH[i + 1].birth_y)
            {
                pomocP = poleH[i];
                poleH[i] = poleH[i + 1];
                poleH[i + 1] = pomocP;
                continue;
            }
        // preverim ca maju rovnkame meno
        if(strcmp(poleH[i].name.first, poleH[i+1].name.first) ==0)
            if (strcmp(poleH[i].name.last, poleH[i + 1].name.last) > 0)
            {
                pomocP = poleH[i];
                poleH[i] = poleH[i + 1];
                poleH[i + 1] = pomocP;
                continue;
            }
        // porovnam iba mena
        if (strcmp(poleH[i].name.first, poleH[i + 1].name.first) > 0)
        {
            pomocP = poleH[i];
            poleH[i] = poleH[i + 1];
            poleH[i + 1] = pomocP;
        }

    }

    */
    for (i = 0; i < m; i++)
        printf("%s %s (%d)\n", poleH[i].name.first, poleH[i].name.last, poleH[i].birth_y);

}

void koniec(MOVIE* base)
{
    ACTOR* akt_a;
    ACTOR* pomocAct;
    MOVIE* pomocFilm;
    while (base != NULL)
    {
        akt_a = base->actory;
        while (akt_a != NULL)
        {
            pomocAct = akt_a;
            akt_a = akt_a->next;
            free(pomocAct);
        }

        pomocFilm = base;
        base = base->next;
        free(pomocFilm);

    }
}


int main() {
    MOVIE* base = NULL;
    MOVIE* akt_f = NULL;
    char command[N_MAX], c;


    strcpy(command, "abyNebolPrazny");

    while (strcmp(command, "koniec"))
    {
        scanf("%s", &command);

        switch (command[2])
        {
        case 'c': base = nacitaj(); break;                // nacitanj
        case 'p': vypis(base); break;                     // vypis
        case 'i': pridaj(base); break;                    // pridaj
        case 'm': base = vymaz(base); break;              // vymaz
        case 'l': filmy(base); break;                     // filmy
        case 'r': herci(base); break;                     // herci
        case 'k': rok(base); break;                       // rok

        }
    }

    // koniec
    if (strcmp(command, "koniec") == 0)
        koniec(base);


    return 0;
}


