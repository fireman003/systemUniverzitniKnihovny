#include "ui.h"
#include "evidence.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define C_RESET          "\x1b[0m"
#define C_BOLD           "\x1b[1m"
#define C_TITLE          "\x1b[1;36m"
#define C_DIM            "\x1b[90m"
#define C_ERROR          "\x1b[1;31m"
#define C_OK             "\x1b[1;32m"
#define C_SELECT         "\x1b[30;46m"
#define C_FIELD          "\x1b[30;47m"
#define C_FIELD_INACTIVE "\x1b[37;100m"
#define CURSOR_HIDE      "\x1b[?25l"
#define CURSOR_SHOW      "\x1b[?25h"

#define APP_TITLE "EVIDENCE UNIVERZITNÍ KNIHOVNY"

typedef struct {
    char username[UI_FIELD_MAX];
    char password[UI_FIELD_MAX];
    Role role;
    int  readerId;
} User;

typedef struct {
    int x, y, w, h;
} Box;


static int  gUserCount = 4;
static int  gNextReaderId = 2;
static char gStatus[160] = "";
static int  gEof = 0;

static void termSize(int* cols, int* rows)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
        *cols = info.srWindow.Right - info.srWindow.Left + 1;
        *rows = info.srWindow.Bottom - info.srWindow.Top + 1;
        return;
    }

    * cols = 80;
    *rows = 24;
}

static int textWidth(const char* s)
{
    int n = 0;
    for (; *s; s++)
        if (((unsigned char)*s & 0xC0) != 0x80)
            n++;
    return n;
}

static void gotoXY(int x, int y)
{
    printf("\x1b[%d;%dH", y, x);
}

static void repeat(const char* s, int n)
{
    while (n-- > 0)
        fputs(s, stdout);
}

void uiRestore(void)
{
    printf(C_RESET CURSOR_SHOW "\x1b[2J\x1b[H");
    fflush(stdout);
}

void uiInit(void)
{

    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (GetConsoleMode(out, &mode))
        SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    atexit(uiRestore);
}

int uiEof(void)
{
    return gEof;
}

int uiGetKey(void)
{
    fflush(stdout);
    if (gEof)
        return KEY_ESC;

    int c = _getch();
    if (c == 0 || c == 224) {
        switch (_getch()) {
        case 72: return KEY_UP;
        case 80: return KEY_DOWN;
        case 75: return KEY_LEFT;
        case 77: return KEY_RIGHT;
        default: return KEY_NONE;
        }
    }
    switch (c) {
    case 13: return KEY_ENTER;
    case 27: return KEY_ESC;
    case 8:  return KEY_BACKSPACE;
    case 9:  return KEY_TAB;
    }
    return c;
}

void uiClear(void)
{
    int cols, rows;
    termSize(&cols, &rows);

    printf(C_RESET CURSOR_HIDE "\x1b[2J");
    gotoXY(1, 1);
    printf(C_SELECT);
    repeat(" ", cols);
    gotoXY((cols - textWidth(APP_TITLE)) / 2 + 1, 1);
    printf(C_SELECT C_BOLD "%s" C_RESET, APP_TITLE);

    if (gStatus[0]) {
        gotoXY(2, rows);
        printf(C_DIM "%s" C_RESET, gStatus);
    }
    gotoXY(1, 3);
}

void uiPause(void)
{
    int cols, rows;
    termSize(&cols, &rows);
    gotoXY(2, rows - 1);
    printf(C_DIM "Stiskni libovolnou klávesu..." C_RESET);
    uiGetKey();
}

static Box drawBox(int w, int h, const char* title)
{
    int cols, rows;
    Box b;
    termSize(&cols, &rows);

    b.w = w;
    b.h = h;
    b.x = (cols - w) / 2 + 1;
    b.y = (rows - h) / 2 + 1;
    if (b.x < 1) b.x = 1;
    if (b.y < 3) b.y = 3;

    uiClear();
    gotoXY(b.x, b.y);
    fputs("┌", stdout); repeat("─", w - 2); fputs("┐", stdout);
    for (int i = 1; i < h - 1; i++) {
        gotoXY(b.x, b.y + i);
        fputs("│", stdout); repeat(" ", w - 2); fputs("│", stdout);
    }
    gotoXY(b.x, b.y + h - 1);
    fputs("└", stdout); repeat("─", w - 2); fputs("┘", stdout);

    if (title) {
        int tw = textWidth(title) + 2;
        gotoXY(b.x + (w - tw) / 2, b.y);
        printf(C_TITLE " %s " C_RESET, title);
    }
    return b;
}

static void boxText(Box b, int row, const char* color, const char* text)
{
    gotoXY(b.x + (b.w - textWidth(text)) / 2, b.y + row);
    printf("%s%s" C_RESET, color, text);
}

static void boxHint(Box b, const char* text)
{
    gotoXY(b.x + (b.w - textWidth(text)) / 2, b.y + b.h + 1);
    printf(C_DIM "%s" C_RESET, text);
}

int uiSelect(const char* title, const char** options, int count, int start)
{
    int sel = (start >= 0 && start < count) ? start : 0;
    int w = textWidth(title) + 8;

    for (int i = 0; i < count; i++)
        if (textWidth(options[i]) + 12 > w)
            w = textWidth(options[i]) + 12;
    if (w < 36)
        w = 36;

    Box b = drawBox(w, count + 4, title);
    boxHint(b, "↑↓ pohyb   Enter výběr   Esc zpět");

    for (;;) {
        for (int i = 0; i < count; i++) {
            gotoXY(b.x + 3, b.y + 2 + i);
            if (i == sel) {
                printf(C_SELECT " ▸ %s", options[i]);
                repeat(" ", w - 9 - textWidth(options[i]));
                printf(C_RESET);
            }
            else {
                printf("   %s", options[i]);
                repeat(" ", w - 9 - textWidth(options[i]));
            }
        }

        int k = uiGetKey();
        if (k == KEY_UP)
            sel = (sel + count - 1) % count;
        else if (k == KEY_DOWN || k == KEY_TAB)
            sel = (sel + 1) % count;
        else if (k == KEY_ENTER)
            return sel;
        else if (k == KEY_ESC)
            return -1;
        else if (k >= '1' && k <= '9' && k - '1' < count)
            return k - '1';
    }
}

static int isAllowed(FieldType type, int c)
{
    if (c < 32 || c > 126)
        return 0;
    if (type == FIELD_NUMBER)
        return (c >= '0' && c <= '9') || c == '-';
    return 1;
}

static int validateForm(FormField* f, int count, char* err, size_t errSize)
{
    for (int i = 0; i < count; i++) {
        if (!f[i].optional && f[i].buf[0] == '\0') {
            snprintf(err, errSize, "Vyplňte pole: %s", f[i].label);
            return i;
        }
        if (strpbrk(f[i].buf, UI_FORBIDDEN_CHARS) != NULL) {
            snprintf(err, errSize, "Zakázaný znak \"%s\" v poli: %s",
                UI_FORBIDDEN_CHARS, f[i].label);
            return i;
        }
        if (f[i].type == FIELD_NUMBER && f[i].buf[0] != '\0') {
            char* end;
            strtol(f[i].buf, &end, 10);
            if (end == f[i].buf || *end != '\0') {
                snprintf(err, errSize, "Pole %s musí být číslo", f[i].label);
                return i;
            }
        }
    }
    return -1;
}

int uiForm(const char* title, FormField* f, int count, const char* error)
{
    int w = 52, h = count * 3 + 4, fw = w - 8, active = 0;
    char err[128];
    snprintf(err, sizeof err, "%s", error ? error : "");
    for (int i = count - 1; i >= 0; i--)
        if (f[i].buf[0] == '\0')
            active = i;

    Box b = drawBox(w, h, title);
    boxHint(b, "↑↓/Tab pole   Enter potvrdit   Esc zpět");

    for (;;) {
        for (int i = 0; i < count; i++) {
            int row = b.y + 2 + i * 3;
            size_t len = strlen(f[i].buf);
            size_t from = len > (size_t)(fw - 2) ? len - (fw - 2) : 0;
            int shown = 0;

            gotoXY(b.x + 4, row);
            printf("%s%s%s" C_RESET, i == active ? C_BOLD : C_DIM,
                f[i].label, f[i].optional ? " (nepovinné)" : "");

            gotoXY(b.x + 4, row + 1);
            printf("%s ", i == active ? C_FIELD : C_FIELD_INACTIVE);
            for (size_t j = from; j < len; j++, shown++)
                putchar(f[i].type == FIELD_PASSWORD ? '*' : f[i].buf[j]);
            repeat(" ", fw - 1 - shown);
            printf(C_RESET);
        }

        gotoXY(b.x + 1, b.y + h - 2);
        repeat(" ", w - 2);
        if (err[0])
            boxText(b, h - 2, C_ERROR, err);

        size_t alen = strlen(f[active].buf);
        int cx = (int)(alen > (size_t)(fw - 2) ? (size_t)(fw - 2) : alen);
        gotoXY(b.x + 5 + cx, b.y + 3 + active * 3);
        printf(CURSOR_SHOW);

        int k = uiGetKey();
        printf(CURSOR_HIDE);

        FormField* a = &f[active];
        size_t len = strlen(a->buf);

        if (k == KEY_ESC) {
            return 0;
        }
        else if (k == KEY_UP) {
            active = (active + count - 1) % count;
        }
        else if (k == KEY_DOWN || k == KEY_TAB) {
            active = (active + 1) % count;
        }
        else if (k == KEY_BACKSPACE) {
            if (len > 0)
                a->buf[len - 1] = '\0';
        }
        else if (k == KEY_ENTER) {
            if (active < count - 1) {
                active++;
            }
            else {
                int bad = validateForm(f, count, err, sizeof err);
                if (bad < 0)
                    return 1;
                active = bad;
            }
        }
        else if (k > 0 && k < 256 && isAllowed(a->type, k)) {
            if (len + 1 < a->size) {
                a->buf[len] = (char)k;
                a->buf[len + 1] = '\0';
                err[0] = '\0';
            }
            else {
                snprintf(err, sizeof err, "Maximální délka je %d znaků", (int)a->size - 1);
            }
        }
    }
}

static void messageBox(const char* title, const char* text, const char* color)
{
    int lines = 1, maxW = 0, cur = 0;
    for (const char* p = text; *p; p++) {
        if (*p == '\n') {
            lines++;
            cur = 0;
        }
        else if (((unsigned char)*p & 0xC0) != 0x80 && ++cur > maxW) {
            maxW = cur;
        }
    }

    int w = maxW + 8 < 36 ? 36 : maxW + 8;
    int h = lines + 6;
    Box b = drawBox(w, h, title);

    const char* p = text;
    for (int i = 0; i < lines; i++) {
        char line[256];
        const char* end = strchr(p, '\n');
        size_t n = end ? (size_t)(end - p) : strlen(p);
        if (n >= sizeof line)
            n = sizeof line - 1;
        memcpy(line, p, n);
        line[n] = '\0';
        boxText(b, 2 + i, color, line);
        p = end ? end + 1 : p + n;
    }
    boxText(b, h - 2, C_SELECT, "   OK   ");

    int k;
    do {
        k = uiGetKey();
    } while (k != KEY_ENTER && k != KEY_ESC && k != ' ');
}

void uiMessage(const char* title, const char* text)
{
    messageBox(title, text, C_RESET);
}

void uiError(const char* text)
{
    messageBox("Chyba", text, C_ERROR);
}

void uiSuccess(const char* text)
{
    messageBox("Hotovo", text, C_OK);
}

int uiConfirm(const char* question)
{
    int w = textWidth(question) + 8 < 36 ? 36 : textWidth(question) + 8;
    int sel = 1;
    Box b = drawBox(w, 7, "Potvrzení");
    boxText(b, 2, C_RESET, question);
    boxHint(b, "←→ výběr   Enter potvrdit");

    for (;;) {
        int mid = b.x + b.w / 2;
        gotoXY(mid - 10, b.y + 4);
        printf("%s   Ano   " C_RESET, sel == 0 ? C_SELECT : C_DIM);
        gotoXY(mid + 2, b.y + 4);
        printf("%s   Ne    " C_RESET, sel == 1 ? C_SELECT : C_DIM);

        int k = uiGetKey();
        if (k == KEY_LEFT || k == KEY_RIGHT || k == KEY_TAB)
            sel = !sel;
        else if (k == KEY_ENTER)
            return sel == 0;
        else if (k == KEY_ESC || k == 'n' || k == 'N')
            return 0;
        else if (k == 'a' || k == 'A')
            return 1;
    }
}

int uiReadText(const char* prompt, char* buf, size_t size, int allowEmpty)
{
    FormField f = { prompt, buf, size, FIELD_TEXT, allowEmpty };
    buf[0] = '\0';
    return uiForm("Zadání", &f, 1, NULL);
}

int uiReadInt(const char* prompt, int min, int max, int* out)
{
    char buf[12] = "";
    char err[96] = "";
    FormField f = { prompt, buf, sizeof buf, FIELD_NUMBER, 0 };

    for (;;) {
        if (!uiForm("Zadání", &f, 1, err[0] ? err : NULL))
            return 0;
        long v = strtol(buf, NULL, 10);
        if (v >= min && v <= max) {
            *out = (int)v;
            return 1;
        }
        buf[0] = '\0';
        snprintf(err, sizeof err, "Číslo musí být v rozsahu %d až %d", min, max);
    }
}

const char* uiRoleName(Role r)
{
    switch (r) {
    case ROLE_KNIHOVNIK: return "Knihovník";
    case ROLE_CTENAR:    return "Čtenář";
    case ROLE_VEDOUCI:   return "Vedoucí";
    case ROLE_SPRAVCE:   return "Správce";
    default:             return "Nepřihlášen";
    }
}

static void nastavSession(Session* s, Role role, int id, const char* jmeno)
{
    s->role = role;
    s->id = id;
    snprintf(s->jmeno, sizeof s->jmeno, "%s", jmeno);

    if (role == ROLE_CTENAR)
        snprintf(gStatus, sizeof gStatus, "Přihlášen: %s  |  Čtenář  |  ID %d", s->jmeno, id);
    else
        snprintf(gStatus, sizeof gStatus, "Přihlášen: %s  |  ID %d", uiRoleName(role), id);
}

static int overId(Session* s, int id)
{
    char celeJmeno[110];
    Ctenar* c;

    switch (id) {
    case ID_SPRAVCE:   nastavSession(s, ROLE_SPRAVCE, id, "Správce");   return 1;
    case ID_VEDOUCI:   nastavSession(s, ROLE_VEDOUCI, id, "Vedoucí");   return 1;
    case ID_KNIHOVNIK: nastavSession(s, ROLE_KNIHOVNIK, id, "Knihovník"); return 1;
    }

    c = najdi_ctenare(id);
    if (c == NULL)
        return 0;

    snprintf(celeJmeno, sizeof celeJmeno, "%s %s", c->jmeno, c->prijmeni);
    nastavSession(s, ROLE_CTENAR, id, celeJmeno);
    return 1;
}

static int loginScreen(Session* s)
{
    char idText[10] = "";
    char err[96] = "";
    FormField f = { "Vaše ID", idText, sizeof idText, FIELD_NUMBER, 0 };

    for (int pokus = 1; ; pokus++) {
        if (!uiForm("Přihlášení", &f, 1, err[0] ? err : NULL))
            return 0;

        if (overId(s, atoi(idText)))
            return 1;

        if (pokus >= UI_MAX_ATTEMPTS) {
            uiError("Příliš mnoho neúspěšných pokusů.");
            return 0;
        }
        idText[0] = '\0';
        snprintf(err, sizeof err, "Neznámé ID (pokus %d/%d)", pokus, UI_MAX_ATTEMPTS);
    }
}

static int registerScreen(Session* s)
{
    Ctenar c = { 0 };
    char telefon[10] = "";
    char zprava[160];
    FormField f[4] = {
        { "Jméno",    c.jmeno,    sizeof c.jmeno,    FIELD_TEXT,   0 },
        { "Příjmení", c.prijmeni, sizeof c.prijmeni, FIELD_TEXT,   0 },
        { "E-mail",   c.email,    sizeof c.email,    FIELD_TEXT,   1 },
        { "Telefon",  telefon,    sizeof telefon,    FIELD_NUMBER, 1 }
    };

    if (!uiForm("Registrace čtenáře", f, 4, NULL))
        return 0;
    c.telefon = atoi(telefon);

    int id = pridej_ctenare(&c);
    if (id <= 0) {
        uiError("Registrace se nezdařila, evidence čtenářů je plná.");
        return 0;
    }

    overId(s, id);
    snprintf(zprava, sizeof zprava,
        "Registrace proběhla úspěšně.\n\nVaše ID: %d\nTímto ID se budete přihlašovat.", id);
    uiSuccess(zprava);
    return 1;
}

int uiLogin(Session* s)
{
    static const char* options[] = { "Přihlásit se", "Registrovat se", "Ukončit program" };
    int sel = 0;

    s->role = ROLE_NONE;
    s->id = 0;
    s->jmeno[0] = '\0';
    gStatus[0] = '\0';

    for (;;) {
        sel = uiSelect("Vítejte", options, 3, sel);

        if (sel == 0 && loginScreen(s))
            return 1;
        if (sel == 1 && registerScreen(s))
            return 1;
        if (gEof)
            return 0;
        if (sel == 2 || sel < 0) {
            if (uiConfirm("Opravdu ukončit program?"))
                return 0;
            sel = 2;
        }
    }
}

void uiRunMenu(const char* title, const MenuItem* items, size_t count,
    const char* backLabel, Session* s)
{
    const MenuItem* visible[UI_MENU_MAX];
    const char* labels[UI_MENU_MAX + 1];
    int sel = 0;

    for (;;) {
        int n = 0;
        for (size_t i = 0; i < count && n < UI_MENU_MAX; i++) {
            if (items[i].roles & s->role) {
                visible[n] = &items[i];
                labels[n] = items[i].label;
                n++;
            }
        }
        labels[n] = backLabel;

        sel = uiSelect(title, labels, n + 1, sel);
        if (sel < 0 || sel == n || gEof)
            return;

        if (visible[sel]->action != NULL)
            visible[sel]->action(s);
        else
            uiMessage("Info", "Tato funkce zatím není hotová.");

        if (gEof)
            return;
    }
}

void pridatKnihu(Session* s)
{
    Kniha k = { 0 };
    char rok[8] = "";
    char zprava[100];
    FormField pole[3] = {
        { "Název",      k.nazev_knihy,  sizeof k.nazev_knihy,  FIELD_TEXT,   0 },
        { "Autor",      k.nazev_autora, sizeof k.nazev_autora, FIELD_TEXT,   0 },
        { "Rok vydání", rok,            sizeof rok,            FIELD_NUMBER, 0 }
    };
    (void)s;

    if (!uiForm("Nová kniha", pole, 3, NULL))
        return;
    k.datum_vydani.rok = atoi(rok);

    int id = pridej_knihu(&k);
    if (id <= 0) {
        uiError("Evidence knih je plná.");
        return;
    }

    snprintf(zprava, sizeof zprava, "Kniha uložena pod ID %d.", id);
    uiSuccess(zprava);
}

void hledatKnihu(Session* s)
{
    char text[50];
    Kniha* nalez[20];
    (void)s;

    if (!uiReadText("Název nebo autor", text, sizeof text, 0))
        return;

    int n = hledej_knihy(text, nalez, 20);
    if (n == 0) {
        uiMessage("Hledání", "Kniha nebyla nalezena.");
        return;
    }

    uiClear();
    printf("  Nalezeno: %d\n\n", n);
    for (int i = 0; i < n; i++)
        printf("  %4d  %-30s %-20s %d  %s\n", nalez[i]->ID, nalez[i]->nazev_knihy,
            nalez[i]->nazev_autora, nalez[i]->datum_vydani.rok,
            nalez[i]->status == Vypujceno ? "vypůjčená" : "v knihovně");
    uiPause();
}