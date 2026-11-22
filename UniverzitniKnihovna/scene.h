#define SCENE_H
#ifdef SCENE_H

#include "ui.h"

// Knihy
void scenaPridaniKnihy(Session* s);                  // přidání knihy / knihy 1
void scenaHledaniKnihy(Session* s);                  // hledání knihy / 1 / 2
void scenaUpravaKnihy(Session* s, int idKnihy);      // UPRAVIŤ dáta
int  scenaVymazaniKnihy(Session* s, int idKnihy);    // VYMAZANIE knihy
void scenaPrehledKnih(Session* s);                   // Správca: všetky knihy
void scenaPrehledDostupnosti(Session* s);            // Vedúci: podľa dostupnosti

// Čtenáři
void scenaPridaniCtenare(Session* s);                // Přidájí čtenáře
void scenaPrehledCtenaru(Session* s);                // chybí v UML
void scenaHledaniCtenare(Session* s);                // chybí v UML
void scenaUpravaCtenare(Session* s, int idCtenare);  // chybí v UML
int  scenaVymazaniCtenare(Session* s, int idCtenare);// chybí v UML

// Výpůjčky
void scenaPridaniVypujcky(Session* s, int idKnihy);  // PRIDANIE půjčky
void scenaVymazaniVypujcky(Session* s, int idKnihy); // VYMAZANIE půjčky
void scenaVypujckyCtenare(Session* s);               // chybí v UML
void scenaMojeVypujcky(Session* s);                  // čtenař

#endif