#pragma once

#ifndef UI_H
#define UI_H

#include <stddef.h>

typedef enum {
    ROLE_NONE = 0,
    ROLE_KNIHOVNIK = 1 << 0,
    ROLE_CTENAR = 1 << 1,
    ROLE_VEDOUCI = 1 << 2,
    ROLE_SPRAVCE = 1 << 3
} Role;

#define ROLE_ALL (ROLE_KNIHOVNIK | ROLE_CTENAR | ROLE_VEDOUCI | ROLE_SPRAVCE)

typedef struct {
    Role role;
    int  reader_id;          
} Session;


//inicializace
void ui_init(void);

#endif