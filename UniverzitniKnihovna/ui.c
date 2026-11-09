#include "ui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/select.h>
static struct termios gOrigTerm;
static int gRawOn = 0;
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

static User gUsers[UI_MAX_USERS] = {
    { "knihovnik", "knihovna", ROLE_KNIHOVNIK, -1 },
    { "vedouci",   "vedouci",  ROLE_VEDOUCI,   -1 },
    { "admin",     "admin",    ROLE_SPRAVCE,   -1 },
    { "ctenar",    "ctenar",   ROLE_CTENAR,     1 }
};
static int  gUserCount = 4;
static int  gNextReaderId = 2;
static char gStatus[160] = "";
static int  gEof = 0;

static void termSize(int* cols, int* rows)
{
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info)) {
        *cols = info.srWindow.Right - info.srWindow.Left + 1;
        *rows = info.srWindow.Bottom - info.srWindow.Top + 1;
        return;
    }
#else
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return;
    }
#endif
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
#ifndef _WIN32
    if (gRawOn) {
        tcsetattr(STDIN_FILENO, TCSANOW, &gOrigTerm);
        gRawOn = 0;
    }
#endif
}

#ifndef _WIN32
static void onSignal(int sig)
{
    (void)sig;
    uiRestore();
    _exit(1);
}
#endif

void uiInit(void)
{
#ifdef _WIN32
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    if (GetConsoleMode(out, &mode))
        SetConsoleMode(out, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#else
    if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &gOrigTerm) == 0) {
        struct termios raw = gOrigTerm;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN] = 1;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        gRawOn = 1;
    }
    signal(SIGINT, onSignal);
    signal(SIGTERM, onSignal);
#endif
    atexit(uiRestore);
}

int uiEof(void)
{
    return gEof;
}

#ifndef _WIN32
static int readByte(int timeoutMs)
{
    unsigned char c;
    if (timeoutMs >= 0) {
        fd_set set;
        struct timeval tv;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        tv.tv_sec = 0;
        tv.tv_usec = timeoutMs * 1000;
        if (select(STDIN_FILENO + 1, &set, NULL, NULL, &tv) <= 0)
            return -1;
    }
    if (read(STDIN_FILENO, &c, 1) != 1)
        return -2;
    return c;
}
#endif

int uiGetKey(void)
{
    fflush(stdout);
    if (gEof)
        return KEY_ESC;
#ifdef _WIN32
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
#else
    int c = readByte(-1);
    if (c == -2) {
        gEof = 1;
        return KEY_ESC;
    }
    if (c == 27) {
        int c2 = readByte(50);
        if (c2 != '[' && c2 != 'O')
            return KEY_ESC;
        switch (readByte(50)) {
        case 'A': return KEY_UP;
        case 'B': return KEY_DOWN;
        case 'C': return KEY_RIGHT;
        case 'D': return KEY_LEFT;
        default:  return KEY_NONE;
        }
    }
    switch (c) {
    case '\n':
    case '\r': return KEY_ENTER;
    case 127:
    case 8:    return KEY_BACKSPACE;
    case '\t': return KEY_TAB;
    }
    return c;
#endif
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

static User* findUser(const char* username)
{
    for (int i = 0; i < gUserCount; i++)
        if (strcmp(gUsers[i].username, username) == 0)
            return &gUsers[i];
    return NULL;
}

int uiAddUser(const char* username, const char* password, Role role, int readerId)
{
    if (gUserCount >= UI_MAX_USERS || findUser(username) != NULL)
        return 0;

    User* u = &gUsers[gUserCount++];
    snprintf(u->username, sizeof u->username, "%s", username);
    snprintf(u->password, sizeof u->password, "%s", password);
    u->role = role;
    u->readerId = readerId;
    return 1;
}

static void setSession(Session* s, const User* u)
{
    s->role = u->role;
    s->readerId = u->readerId;
    snprintf(s->username, sizeof s->username, "%s", u->username);

    if (u->role == ROLE_CTENAR)
        snprintf(gStatus, sizeof gStatus, "Přihlášen: %s  |  %s č. %d",
            u->username, uiRoleName(u->role), u->readerId);
    else
        snprintf(gStatus, sizeof gStatus, "Přihlášen: %s  |  %s",
            u->username, uiRoleName(u->role));
}

static int loginScreen(Session* s)
{
    char name[UI_FIELD_MAX] = "", pass[UI_FIELD_MAX] = "", err[96] = "";
    FormField f[2] = {
        { "Uživatelské jméno", name, sizeof name, FIELD_TEXT,     0 },
        { "Heslo",             pass, sizeof pass, FIELD_PASSWORD, 0 }
    };

    for (int attempt = 1; ; attempt++) {
        if (!uiForm("Přihlášení", f, 2, err[0] ? err : NULL))
            return 0;

        User* u = findUser(name);
        if (u != NULL && strcmp(u->password, pass) == 0) {
            setSession(s, u);
            return 1;
        }
        if (attempt >= UI_MAX_ATTEMPTS) {
            uiError("Příliš mnoho neúspěšných pokusů.");
            return 0;
        }
        pass[0] = '\0';
        snprintf(err, sizeof err, "Špatné jméno nebo heslo (pokus %d/%d)",
            attempt, UI_MAX_ATTEMPTS);
    }
}

static int registerScreen(Session* s, RegisterReaderFn registerReader)
{
    char full[UI_FIELD_MAX] = "", name[UI_FIELD_MAX] = "";
    char pass[UI_FIELD_MAX] = "", pass2[UI_FIELD_MAX] = "";
    char err[128] = "";
    FormField f[4] = {
        { "Jméno a příjmení",  full,  sizeof full,  FIELD_TEXT,     0 },
        { "Uživatelské jméno", name,  sizeof name,  FIELD_TEXT,     0 },
        { "Heslo",             pass,  sizeof pass,  FIELD_PASSWORD, 0 },
        { "Heslo znovu",       pass2, sizeof pass2, FIELD_PASSWORD, 0 }
    };

    for (;;) {
        if (!uiForm("Registrace čtenáře", f, 4, err[0] ? err : NULL))
            return 0;

        if (findUser(name) != NULL) {
            snprintf(err, sizeof err, "Uživatelské jméno už existuje");
        }
        else if (strlen(pass) < 4) {
            snprintf(err, sizeof err, "Heslo musí mít alespoň 4 znaky");
        }
        else if (strcmp(pass, pass2) != 0) {
            snprintf(err, sizeof err, "Hesla se neshodují");
        }
        else {
            int id = registerReader ? registerReader(full) : gNextReaderId++;
            if (id <= 0 || !uiAddUser(name, pass, ROLE_CTENAR, id)) {
                snprintf(err, sizeof err, "Registrace se nezdařila");
                continue;
            }
            setSession(s, findUser(name));

            char msg[128];
            snprintf(msg, sizeof msg, "Registrace proběhla úspěšně.\nVaše číslo čtenáře: %d", id);
            uiSuccess(msg);
            return 1;
        }
        pass[0] = '\0';
        pass2[0] = '\0';
    }
}

int uiLogin(Session* s, RegisterReaderFn registerReader)
{
    static const char* options[] = { "Přihlásit se", "Registrovat se", "Ukončit program" };
    int sel = 0;

    s->role = ROLE_NONE;
    s->readerId = -1;
    s->username[0] = '\0';
    gStatus[0] = '\0';

    for (;;) {
        sel = uiSelect("Vítejte", options, 3, sel);

        if (sel == 0 && loginScreen(s))
            return 1;
        if (sel == 1 && registerScreen(s, registerReader))
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