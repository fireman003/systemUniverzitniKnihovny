#ifndef UI_H
#define UI_H

#include <stddef.h>

#define UI_FIELD_MAX       64
#define UI_MAX_USERS       100
#define UI_MAX_ATTEMPTS    3
#define UI_MENU_MAX        32
#define UI_FORBIDDEN_CHARS ";"

enum {
    KEY_NONE = 0,
    KEY_UP = 1000,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ENTER,
    KEY_ESC,
    KEY_BACKSPACE,
    KEY_TAB
};

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
    int  readerId;
    char username[UI_FIELD_MAX];
} Session;

typedef struct {
    const char* label;
    void (*action)(Session* s);
    unsigned roles;
} MenuItem;

typedef enum {
    FIELD_TEXT,
    FIELD_PASSWORD,
    FIELD_NUMBER
} FieldType;

typedef struct {
    const char* label;
    char* buf;
    size_t size;
    FieldType type;
    int optional;
} FormField;

typedef int (*RegisterReaderFn)(const char* fullName);

void uiInit(void);
void uiRestore(void);
int  uiEof(void);

void uiClear(void);
void uiPause(void);
int  uiGetKey(void);

int  uiSelect(const char* title, const char** options, int count, int start);
int  uiForm(const char* title, FormField* fields, int count, const char* error);
void uiMessage(const char* title, const char* text);
void uiError(const char* text);
void uiSuccess(const char* text);
int  uiConfirm(const char* question);

int  uiReadText(const char* prompt, char* buf, size_t size, int allowEmpty);
int  uiReadInt(const char* prompt, int min, int max, int* out);

int         uiLogin(Session* s, RegisterReaderFn registerReader);
int         uiAddUser(const char* username, const char* password, Role role, int readerId);
const char* uiRoleName(Role r);

void uiRunMenu(const char* title, const MenuItem* items, size_t count,
    const char* backLabel, Session* s);

#endif