#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "evidence.h"

Kniha          knihy[MAX_KNIH];
int            pocet_knih = 0;
Ctenar         ctenari[MAX_CTENARU];
int            pocet_ctenaru = 0;
ZaznamVypujcky vypujcky[MAX_VYPUJCEK];
int            pocet_vypujcek = 0;

/* ======================== Data ======================== */

int evidence_nacti(void)
{
    int n = nacti_knihy(knihy, MAX_KNIH, SOUBOR_KNIH);
    if (n < 0)
        return 0;
    pocet_knih = n;
    return 1;
}

int evidence_uloz(void)
{
    return uloz_knihy(knihy, pocet_knih, SOUBOR_KNIH);
}

/* ======================== Knihy ======================== */

const char* kontrola_knihy(const Kniha* k)
{
    if (k->nazev_knihy[0] == '\0')
        return "Název nesmí být prázdný";
    if (k->nazev_autora[0] == '\0')
        return "Autor nesmí být prázdný";
    if (k->datum_vydani.rok < 1450 || k->datum_vydani.rok > 2100)
        return "Rok vydání musí být 1450 až 2100";
    if (k->pocet_stran < 0 || k->pocet_stran > 20000)
        return "Počet stran musí být 0 až 20000";
    return NULL;
}

int pridej_knihu(Kniha* k)
{
    int max_id = 0;

    if (pocet_knih >= MAX_KNIH)
        return EV_PLNO;

    for (int i = 0; i < pocet_knih; i++)
        if (knihy[i].ID > max_id)
            max_id = knihy[i].ID;

    k->ID = max_id + 1;
    k->status = V_knihovne;
    knihy[pocet_knih++] = *k;

    return k->ID;
}

Kniha* najdi_knihu(int id)
{
    for (int i = 0; i < pocet_knih; i++)
        if (knihy[i].ID == id)
            return &knihy[i];
    return NULL;
}

int hledej_knihy(const char* text, Kniha* vysledky[], int max)
{
    int n = 0;

    for (int i = 0; i < pocet_knih && n < max; i++)
        if (strstr(knihy[i].nazev_knihy, text) != NULL ||
            strstr(knihy[i].nazev_autora, text) != NULL)
            vysledky[n++] = &knihy[i];
    return n;
}

int vsechny_knihy(Kniha* vysledky[], int max)
{
    int n = 0;

    for (int i = 0; i < pocet_knih && n < max; i++)
        vysledky[n++] = &knihy[i];
    return n;
}

int uprav_knihu(const Kniha* k)
{
    Kniha* puvodni = najdi_knihu(k->ID);
    Status stav;

    if (puvodni == NULL)
        return EV_NENALEZENO;

    stav = puvodni->status;
    *puvodni = *k;
    puvodni->status = stav;
    return EV_OK;
}

int smaz_knihu(int id)
{
    for (int i = 0; i < pocet_knih; i++) {
        if (knihy[i].ID != id)
            continue;

        if (knihy[i].status == Vypujceno)
            return EV_VYPUJCENA;

        for (int j = i; j < pocet_knih - 1; j++)
            knihy[j] = knihy[j + 1];
        pocet_knih--;
        return EV_OK;
    }
    return EV_NENALEZENO;
}

/* ======================== Čtenáři ======================== */

int pridej_ctenare(Ctenar* c)
{
    int max_id = PRVNI_ID_CTENARE - 1;

    if (pocet_ctenaru >= MAX_CTENARU)
        return EV_PLNO;

    for (int i = 0; i < pocet_ctenaru; i++)
        if (ctenari[i].ID > max_id)
            max_id = ctenari[i].ID;

    c->ID = max_id + 1;
    ctenari[pocet_ctenaru++] = *c;

    return c->ID;
}

Ctenar* najdi_ctenare(int id)
{
    for (int i = 0; i < pocet_ctenaru; i++)
        if (ctenari[i].ID == id)
            return &ctenari[i];
    return NULL;
}