#ifndef OBRAZOVKY_H
#define OBRAZOVKY_H

#include "ui.h"

/* Knihy */
void prehledKnih(Session* s);
void prehledDostupnosti(Session* s);
void pridatKnihu(Session* s);
void hledatKnihu(Session* s);
void pujcitKnihu(Session* s);
void vratitKnihu(Session* s);

/* Čtenáři */
void pridatCtenare(Session* s);
void prehledCtenaru(Session* s);
void hledatCtenare(Session* s);
void vypujckyCtenare(Session* s);
void mojeVypujcky(Session* s);

#endif