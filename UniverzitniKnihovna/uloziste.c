#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "../uloziste.h"


int uloz_knihy(Kniha pole_knih[], int pocet_knih, const char* nazev_souboru)
{

    FILE* fptr = fopen(nazev_souboru, "w");
    if (fptr == NULL)
    {
        printf("CHYBA: Nepodarilo se otevrit soubor '%s' pro zapis\n", nazev_souboru);
        return 0;
    }

    for (int i = 0; i < pocet_knih; i++)
    {

        fprintf(fptr, "%d;%s;%s;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d\n",

            pole_knih[i].ID,
            pole_knih[i].nazev_knihy,
            pole_knih[i].nazev_autora,
            pole_knih[i].zanr,
            pole_knih[i].pocet_stran,
            pole_knih[i].datum_vydani.den,
            pole_knih[i].datum_vydani.mesic,
            pole_knih[i].datum_vydani.rok,
            pole_knih[i].status
        );
    }


    fclose(fptr);

    printf("Knihy ulozeny\n");



    return 1;
}


int nacti_knihovniky(Knihovnik pole_knihovniku[])
{
    FILE* fptr = fopen("knihovnici.txt", "r");
    if (fptr == NULL)
    {
        printf("CHYBA: Nepodarilo se otevrit soubor 'knihovnici.txt' pro cteni\n");
        return 0;
    }

    int i = 0;
    while (fscanf(fptr, "%d;%49[^;];%49[^;];%49[^;];%d\n",
        &pole_knihovniku[i].ID,
        pole_knihovniku[i].jmeno,
        pole_knihovniku[i].prijmeni,
        pole_knihovniku[i].email,
        &pole_knihovniku[i].telefon) == 5)
    {
        i++;
    }

    fclose(fptr);
    printf("Ze souboru bylo nacteno %d knihovniku.\n", i);
    return i;
}




//ctenari
int uloz_ctenare(Ctenar pole_ctenaru[], int pocet_ctenaru, int pocet_knih, const char* nazev_souboru)
{
    FILE* fptr = fopen(nazev_souboru, "w");
    if (fptr == NULL)
    {
        printf("CHYBA: Nepodarilo se otevrit soubor 'ctenari.txt' pro zapis\n");
        return 0;
    }


    fclose(fptr);

    printf("INFO: Uspesne ulozeno %d knih do souboru '%s'.\n", pocet_knih, nazev_souboru);
    return 1;
}

int nacti_knihy(Kniha pole_knih[], int max_knih, const char* nazev_souboru) {
    // ahoj, prosím vracej počet knih zpět a jen data ukladej do pole knih :)
}