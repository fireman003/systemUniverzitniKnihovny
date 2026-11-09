#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "uloziste.h"

int uloz_knihy(Kniha pole_knih[], int pocet_knih, const char* nazev_souboru) {
    FILE* soubor = fopen(nazev_souboru, "w");

    if (soubor == NULL) 
    {
        printf("CHYBA: Nepodarilo se otevrit soubor '%s' pro zapis!\n", nazev_souboru);
        return 0;
    }

    for (int i = 0; i < pocet_knih; i++) 
    {
        
        fprintf(soubor, "%d;%s;%s;%d;%d\n",
            pole_knih[i].id,
            pole_knih[i].nazev,
            pole_knih[i].autor,
            pole_knih[i].rok_vydani,
            pole_knih[i].je_vypujcena
        );
    }

    
    fclose(soubor);

    printf("INFO: Uspesne ulozeno %d knih do souboru '%s'.\n", pocet_knih, nazev_souboru);
    return 1;