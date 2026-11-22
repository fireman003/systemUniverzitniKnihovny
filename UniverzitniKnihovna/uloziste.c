#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "../Header1.h"


int uloz_knihy(Kniha pole_knih[], int pocet_knih)
{

    FILE* fptr = fopen("knihy.txt", "w");
    if (fptr == NULL)
    {
        printf("CHYBA: Nepodarilo se otevrit soubor '%s' pro zapis\n", "knihy.txt");
        return 0;
    }

    for (int i = 0; i < pocet_knih; i++)
    {

        fprintf(fptr, "%d;%s;%s;%s;%d;%d;%d;%d;%d;%s;%d;%d;%d;%s;%d;%d;%d;%d;%d;%d\n",

            pole_knih[i].ID,
            pole_knih[i].název_knihy,
            pole_knih[i].název_autora,
            pole_knih[i].žánr,
            pole_knih[i].počet_stran,
            pole_knih[i].datum_vydání.den,
            pole_knih[i].datum_vydání.mesic,
            pole_knih[i].datum_vydání.rok,
            pole_knih[i].status,
            pole_knih[i].Rezrevováno,
            pole_knih[i].Rezervovano_pro_den.den,
            pole_knih[i].Rezervovano_pro_den.mesic,
            pole_knih[i].Rezervovano_pro_den.rok,
            pole_knih[i].výpůjčeno,
            pole_knih[i].vypůjčeno_od.den,
            pole_knih[i].vypůjčeno_od.mesic,
            pole_knih[i].vypůjčeno_od.rok,
            pole_knih[i].vypůjčeno_do.den,
            pole_knih[i].vypůjčeno_do.mesic,
            pole_knih[i].vypůjčeno_do.rok



        );
    }


    fclose(fptr);

    printf("Knihy uloženy\n")



        return 1;
}


int nacti_knihovniky(Knihovník pole_knihovniku[])
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
        pole_knihovniku[i].jméno,
        pole_knihovniku[i].příjmení,
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
static int uloz_ctenare(Čtenář pole_ctenaru[], int pocet_ctenaru)
{
    FILE* fptr = fopen("ctenari.txt", "w");
    if (fptr == NULL)
    {
        printf("CHYBA: Nepodarilo se otevrit soubor 'ctenari.txt' pro zapis\n");
        return 0;
    }


    fclose(soubor);

    printf("INFO: Uspesne ulozeno %d knih do souboru '%s'.\n", pocet_knih, nazev_souboru);
    return 1;
}