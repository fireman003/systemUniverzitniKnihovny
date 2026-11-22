#include <stdio.h>
#include <stdlib.h>
#include "ui.h"

int main()
{
    Session s;
    uiInit();

    if (!evidence_nacti()) {
        uiError("Nepodařilo se načíst data.");
    }
        
    //hlavni cyklus programu
    while () {
        
    }
        

    evidence_uloz();
    return 0;
}