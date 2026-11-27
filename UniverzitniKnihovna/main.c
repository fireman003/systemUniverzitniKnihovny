#include <stdio.h>

#include "ui.h"
#include "obrazovky.h"

static const MenuItem HLAVNI_MENU[] = {
    { "Přehled všech knih", prehledKnih, ROLE_SPRAVCE },
    { "Přidat knihu",       pridatKnihu, ROLE_SPRAVCE | ROLE_KNIHOVNIK },
    { "Hledat knihu",       hledatKnihu, ROLE_SPRAVCE | ROLE_KNIHOVNIK | ROLE_VEDOUCI }
};

int main(void)
{
    Session s;

    uiInit();

    while (uiLogin(&s))
        uiRunMenu("Hlavní menu", HLAVNI_MENU, sizeof HLAVNI_MENU / sizeof HLAVNI_MENU[0],
            "Odhlásit se", &s);

    return 0;
}