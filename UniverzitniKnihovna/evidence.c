#include "evidence.h"

Kniha knihy[MAX_KNIH];
int   pocet_knih = 0;

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

int pridej_knihu(Kniha* k)
{
    int max_id = 0;

    if (pocet_knih >= MAX_KNIH)
        return -1;

    for (int i = 0; i < pocet_knih; i++)
        if (knihy[i].ID > max_id)
            max_id = knihy[i].ID;

    k->ID = max_id + 1;
    k->status = V_knihovne;
    knihy[pocet_knih++] = *k;

    return k->ID;
}

int hledej_knihy(const char* text, Kniha* vysledky[], int max)
{
    int n = 0;

    for (int i = 0; i < pocet_knih && n < max; i++) {
        if (strstr(knihy[i].nazev_knihy, text) != NULL ||
            strstr(knihy[i].nazev_autora, text) != NULL) {
            vysledky[n++] = &knihy[i];
        }
    }
    return n;
}

Ctenar ctenari[MAX_CTENARU];
int    pocet_ctenaru = 0;

int pridej_ctenare(Ctenar* c)
{
    int max_id = PRVNI_ID_CTENARE - 1;

    if (pocet_ctenaru >= MAX_CTENARU)
        return -1;

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