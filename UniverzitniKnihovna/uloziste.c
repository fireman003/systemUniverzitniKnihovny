#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "Header1.h"

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

    fclose(fptr);
    printf("Knihovnici ulozeni\n");
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

int uloz_ctenare(Čtenář pole_ctenaru[], int pocet_ctenaru)
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