#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "Header1.h"



//knihy

int uloz_knihy(Kniha pole_knih[], int pocet_knih) 
{

    FILE *fptr = fopen("knihy.txt", "w");
    if (fptr ==NULL) 
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






int nacti_knihy(Kniha pole_knih[])
{
    FILE *fptr = fopen("knihy.txt", "r");
    if (fptr == NULL) 
    {
        printf("CHYBA: Nepodarilo se otevrit soubor '%s' pro cteni\n", "knihy.txt");
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




//knihovnici

int uloz_knihovniky(Knihovník pole_knihovniku[], int pocet_knihovniku)
{
    FILE* fptr = fopen("knihovnici.txt", "w");
    if (fptr == NULL)
    {
        printf("CHYBA: Nepodarilo se otevrit soubor 'knihovnici.txt' pro zapis\n");
        return 0;
    }

    for (int i = 0; i < pocet_knihovniku; i++)
    {
        fprintf(fptr, "%d;%s;%s;%s;%d\n",
            pole_knihovniku[i].ID,
            pole_knihovniku[i].jméno,
            pole_knihovniku[i].příjmení,
            pole_knihovniku[i].email,
            pole_knihovniku[i].telefon
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

    for (int i = 0; i < pocet_ctenaru; i++)
    {
        
        fprintf(fptr, "%d;%s;%s;%s;%d\n",
            pole_ctenaru[i].ID,
            pole_ctenaru[i].jméno,
            pole_ctenaru[i].příjmení,
            pole_ctenaru[i].email,
            pole_ctenaru[i].telefon
        );

        
        for (int j = 0; j < 100; j++)
        {
            fprintf(fptr, "%d ", pole_ctenaru[i].vypůjčené_knihy[j].ID);
        }
        fprintf(fptr, "\n");

        for (int j = 0; j < 100; j++)
        {
            fprintf(fptr, "%d ", pole_ctenaru[i].rezervované_knihy[j].ID);
        }
        fprintf(fptr, "\n");
    }

    fclose(fptr);
    printf("Ctenari ulozeni\n");
    return 1;
}




int nacti_ctenare(Čtenář pole_ctenaru[])
{
    FILE* fptr = fopen("ctenari.txt", "r");
    if (fptr == NULL)
    {
        printf("CHYBA: Nepodarilo se otevrit soubor 'ctenari.txt' pro cteni\n");
        return 0;
    }

    int i = 0;

    
    while (fscanf(fptr, "%d;%49[^;];%49[^;];%49[^;];%d\n",
        &pole_ctenaru[i].ID,
        pole_ctenaru[i].jméno,
        pole_ctenaru[i].příjmení,
        pole_ctenaru[i].email,
        &pole_ctenaru[i].telefon) == 5)
    {
        
            for (int j = 0; j < 100; j++)
        {
            fscanf(fptr, "%d", &pole_ctenaru[i].vypůjčené_knihy[j].ID);
        }

       
        for (int j = 0; j < 100; j++)
        {
            fscanf(fptr, "%d", &pole_ctenaru[i].rezervované_knihy[j].ID);
        }

        i++;
    }

    fclose(fptr);
    printf("Ze souboru bylo nacteno %d ctenaru.\n", i);
    return i;
}