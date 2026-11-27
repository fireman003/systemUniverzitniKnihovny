#ifndef EVIDENCE_H
#define EVIDENCE_H

#include "../uloziste.h"
#include "nastaveni.h"

#define EV_OK            0
#define EV_PLNO         -1
#define EV_NENALEZENO   -2
#define EV_VYPUJCENA    -3
#define EV_NEVYPUJCENA  -4
#define EV_MA_VYPUJCKY  -5

typedef struct
{
    int   id_knihy;
    int   id_ctenare;
    Datum od;
    Datum do_kdy;
    int   aktivni;
} ZaznamVypujcky;

extern Kniha          knihy[MAX_KNIH];
extern int            pocet_knih;
extern Ctenar         ctenari[MAX_CTENARU];
extern int            pocet_ctenaru;
extern ZaznamVypujcky vypujcky[MAX_VYPUJCEK];
extern int            pocet_vypujcek;

int evidence_nacti(void);
int evidence_uloz(void);

/* Knihy */
const char* kontrola_knihy(const Kniha* k);
int    pridej_knihu(Kniha* k);
Kniha* najdi_knihu(int id);
int    hledej_knihy(const char* text, Kniha* vysledky[], int max);
int    vsechny_knihy(Kniha* vysledky[], int max);
int    uprav_knihu(const Kniha* k);
int    smaz_knihu(int id);

/* Čtenáři */
const char* kontrola_ctenare(const Ctenar* c);
int     pridej_ctenare(Ctenar* c);
Ctenar* najdi_ctenare(int id);
int     hledej_ctenare(const char* text, Ctenar* vysledky[], int max);
int     vsichni_ctenari(Ctenar* vysledky[], int max);
int     uprav_ctenare(const Ctenar* c);
int     smaz_ctenare(int id);

/* Výpůjčky */
int             vytvor_vypujcku(int id_ctenare, int id_knihy);
int             ukonci_vypujcku(int id_knihy);
ZaznamVypujcky* vypujcka_knihy(int id_knihy);
int             vypujcky_ctenare(int id_ctenare, ZaznamVypujcky* vysledky[], int max);

#endif