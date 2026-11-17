#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "Header1.h"


int uloz_knihy(Kniha pole_knih[], int pocet_knih) 
{

    FILE *fptr = fopen("knihy.txt", "w");
    if (fptr ==NULL) 
    {
        printf("CHYBA: Nepodarilo se otevrit soubor '%s' pro zapis\n", nazev_souboru);
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






int nacti_knihy(Kniha pole_knih[])
{
    FILE *fptr = fopen(nazev_souboru, "r");
    if (fptr == NULL) 
    {
        printf("CHYBA: Nepodarilo se otevrit soubor '%s' pro cteni\n", nazev_souboru);
        return 0;
    }

    int i = 0;
    int navic;



    while (fscanf(fptr, "%d;%49[^;];%49[^;];%49[^;];%d;%d;%d;%d;%d;%49[^;];%d;%d;%d;%49[^;];%d;%d;%d;%d;%d;%d\n",
        &pole_knih[i].ID,
        pole_knih[i].název_knihy,
        pole_knih[i].název_autora,
        pole_knih[i].žánr,
        &pole_knih[i].počet_stran,
        &pole_knih[i].datum_vydání.den,
        &pole_knih[i].datum_vydání.mesic,
        &pole_knih[i].datum_vydání.rok,
        &navic
        pole_knih[i].Rezrevováno,
        &pole_knih[i].Rezervovano_pro_den.den,
        &pole_knih[i].Rezervovano_pro_den.mesic,
        &pole_knih[i].Rezervovano_pro_den.rok,
        pole_knih[i].výpůjčeno,
        &pole_knih[i].vypůjčeno_od.den,
        &pole_knih[i].vypůjčeno_od.mesic,
        &pole_knih[i].vypůjčeno_od.rok,
        &pole_knih[i].vypůjčeno_do.den,
        &pole_knih[i].vypůjčeno_do.mesic,
        &pole_knih[i].vypůjčeno_do.rok) == 20)


    {
        pole_knih[i].status = (Status)navic;
        i++;
    }

    fclose(fptr);
    printf("Ze souboru bylo nacteno %d knih.\n", i);
    return i;
}