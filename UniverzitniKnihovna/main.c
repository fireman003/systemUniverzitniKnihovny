#include <stdio.h>
#include "ui.h"

static const MenuItem HLAVNI_MENU[] = {
    { "Přehled všech knih",        NULL,        ROLE_SPRAVCE },
    { "Přidat knihu",              pridatKnihu, ROLE_SPRAVCE | ROLE_KNIHOVNIK },
    { "Hledat knihu",              hledatKnihu, ROLE_SPRAVCE | ROLE_KNIHOVNIK | ROLE_VEDOUCI },
    { "Přidat čtenáře",            NULL,        ROLE_KNIHOVNIK },
    { "Výpůjčky čtenáře",          NULL,        ROLE_KNIHOVNIK },
    { "Přehled podle dostupnosti", NULL,        ROLE_VEDOUCI },
    { "Moje výpůjčky",             NULL,        ROLE_CTENAR }
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