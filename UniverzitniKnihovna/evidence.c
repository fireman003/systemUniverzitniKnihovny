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
    k->vypujceno = 0;
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