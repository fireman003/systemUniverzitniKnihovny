#define EVIDENCE_H
#ifdef EVIDENCE_H

#include "../Header1.h"

int evidence_nacti(void);
int evidence_uloz(void);

// Knihy
const char* kontrola_knihy(const Kniha* k);     // NULL = OK, jinak text chyby
int    pridej_knihu(Kniha* k);                  // vrací nové ID
Kniha* najdi_knihu(int id);                     // NULL = neexistuje
int    hledej_knihy(const char* text, Kniha* vysledky[], int max);
int    vsechny_knihy(Kniha* vysledky[], int max);
int    uprav_knihu(const Kniha* k);
int    smaz_knihu(int id);                      // nelze smazat vypůjčenou

// Čtenáři
const char* kontrola_ctenare(const Ctenar* c);
int     pridej_ctenare(Ctenar* c);
Ctenar* najdi_ctenare(int id);
int     hledej_ctenare(const char* text, Ctenar* vysledky[], int max);
int     vsichni_ctenari(Ctenar* vysledky[], int max);
int     uprav_ctenare(const Ctenar* c);
int     smaz_ctenare(int id);                   // nelze smazat s aktivní výpůjčkou

// Výpůjčky
int       vytvor_vypujcku(int id_ctenare, int id_knihy);  // kontrola dostupnosti
int       ukonci_vypujcku(int id_knihy);                  // kontrola, že je půjčená
Vypujcka* aktivni_vypujcka_knihy(int id_knihy);
int       vypujcky_ctenare(int id_ctenare, Vypujcka* vysledky[], int max);

#endif