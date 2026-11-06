#ifndef ULOZISTE_H
#define ULOZISTE_H

#define MAX_TEXT 100

typedef struct 
{
    int id;
    char nazev[MAX_TEXT];
    char autor[MAX_TEXT];
    int rok_vydani;
    int je_vypujcena;
} Kniha;


typedef struct 
{
    int id; 
    char jmeno[MAX_TEXT];
    char kontakt[MAX_TEXT];
} Ctenar;


typedef struct 
{
    int id_vypujcky;
    int id_ctenare;
    int id_knihy;
    int je_aktivni; 
} Vypujcka;

int uloz_knihy(Kniha pole_knih[], int pocet_knih, const char* nazev_souboru);

#endif