#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "evidence.h"

//testovaci UI itemy
static const MenuItem HLAVNI_MENU[] = {
    { "Přidat knihu", pridatKnihu, ROLE_ALL },
    { "Hledat knihu", hledatKnihu, ROLE_ALL }
};

int main()
{
    Session s;
    uiInit();

    if (!evidence_nacti()) {
        uiError("Nepodařilo se načíst data.");
    }
        
    //hlavni cyklus programu
    while (uiLogin(&s, NULL)){
        uiRunMenu("Hlavní menu", HLAVNI_MENU, sizeof HLAVNI_MENU / sizeof HLAVNI_MENU[0],
            "Odhlásit se", &s);
    }
        

    evidence_uloz();
    return 0;
}
