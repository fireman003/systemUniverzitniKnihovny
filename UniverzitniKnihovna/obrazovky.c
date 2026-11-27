#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "obrazovky.h"
#include "evidence.h"

#define MAX_VYSLEDKU    15
#define RADKU_NA_STRANU 15

/* ======================== Pomocné ======================== */

static void cisloNaText(int cislo, char* buf, size_t size)
{
    if (cislo > 0)
        snprintf(buf, size, "%d", cislo);
    else
        buf[0] = '\0';
}

static void stavKnihy(const Kniha* k, char* buf, size_t size)
{
    snprintf(buf, size, "%s", k->status == Vypujceno ? "vypůjčená" : "dostupná");
}

static void hlavickaTabulkyKnih(const char* nadpis, int pocet)
{
    uiClear();
    printf("  %s   (celkem %d)\n\n", nadpis, pocet);
    printf("    ID  Název                          Autor                Rok   Stav\n");
    printf("  ------------------------------------------------------------------------------\n");
}

static void vypisTabulkuKnih(const char* nadpis, Kniha* seznam[], int pocet)
{
    char stav[48];

    hlavickaTabulkyKnih(nadpis, pocet);
    for (int i = 0; i < pocet; i++) {
        if (i > 0 && i % RADKU_NA_STRANU == 0) {
            uiPause();
            hlavickaTabulkyKnih(nadpis, pocet);
        }
        stavKnihy(seznam[i], stav, sizeof stav);
        printf("  %4d  %-30.30s %-20.20s %-5d %s\n", seznam[i]->ID, seznam[i]->nazev_knihy,
            seznam[i]->nazev_autora, seznam[i]->datum_vydani.rok, stav);
    }
    uiPause();
}

/* ======================== Formulář knihy ======================== */

typedef struct {
    char      rok[6];
    char      stran[6];
    FormField pole[5];
} FormularKnihy;

static void pripravFormularKnihy(FormularKnihy* f, Kniha* k)
{
    cisloNaText(k->datum_vydani.rok, f->rok, sizeof f->rok);
    cisloNaText(k->pocet_stran, f->stran, sizeof f->stran);

    FormField p[5] = {
        { "Název",       k->nazev_knihy,  sizeof k->nazev_knihy,  FIELD_TEXT,   0 },
        { "Autor",       k->nazev_autora, sizeof k->nazev_autora, FIELD_TEXT,   0 },
        { "Žánr",        k->zanr,         sizeof k->zanr,         FIELD_TEXT,   1 },
        { "Počet stran", f->stran,        sizeof f->stran,        FIELD_NUMBER, 1 },
        { "Rok vydání",  f->rok,          sizeof f->rok,          FIELD_NUMBER, 0 }
    };
    memcpy(f->pole, p, sizeof p);
}

/* vrací 1 = vyplněno správně, 0 = Esc */
static int vyplnFormularKnihy(const char* titulek, Kniha* k)
{
    FormularKnihy f;
    char err[100] = "";

    pripravFormularKnihy(&f, k);

    for (;;) {
        if (!uiForm(titulek, f.pole, 5, err[0] ? err : NULL))
            return 0;

        k->datum_vydani.rok = atoi(f.rok);
        k->pocet_stran = atoi(f.stran);

        const char* chyba = kontrola_knihy(k);
        if (chyba == NULL)
            return 1;
        snprintf(err, sizeof err, "%s", chyba);

        if (k->datum_vydani.rok < 0 || k->datum_vydani.rok > 2100)
            f.rok[0] = '\0';
        else
            f.stran[0] = '\0';
    }
}

/* ======================== Knihy: akce ======================== */

static void upravitKnihu(int idKnihy)
{
    Kniha* k = najdi_knihu(idKnihy);
    Kniha kopie;

    if (k == NULL)
        return;

    kopie = *k;
    if (!vyplnFormularKnihy("Upravit knihu", &kopie))
        return;
    if (!uiConfirm("Uložit změny?"))
        return;

    if (uprav_knihu(&kopie) == EV_OK)
        uiSuccess("Změny byly uloženy.");
    else
        uiError("Knihu se nepodařilo upravit.");
}

/* vrací 1 = kniha smazána */
static int smazatKnihu(int idKnihy)
{
    if (!uiConfirm("Opravdu smazat knihu?"))
        return 0;

    switch (smaz_knihu(idKnihy)) {
    case EV_OK:
        uiSuccess("Kniha byla smazána.");
        return 1;
    case EV_VYPUJCENA:
        uiError("Knihu nelze smazat, je vypůjčená.");
        return 0;
    default:
        uiError("Kniha nebyla nalezena.");
        return 0;
    }
}

/* ======================== Knihy: detail ======================== */

static void popisKnihy(const Kniha* k, char* buf, size_t size)
{
    char stav[48];

    stavKnihy(k, stav, sizeof stav);
    snprintf(buf, size,
        "ID:           %d\n"
        "Název:        %s\n"
        "Autor:        %s\n"
        "Žánr:         %s\n"
        "Počet stran:  %d\n"
        "Rok vydání:   %d\n"
        "\n"
        "Stav:         %s",
        k->ID, k->nazev_knihy, k->nazev_autora, k->zanr[0] ? k->zanr : "-",
        k->pocet_stran, k->datum_vydani.rok, stav);
}

static void detailKnihy(Session* s, int idKnihy)
{
    enum { UPRAVIT, SMAZAT, ZPET };
    const char* volby[3];
    int akce[3];
    char popis[512];

    for (;;) {
        Kniha* k = najdi_knihu(idKnihy);
        int n = 0;

        if (k == NULL)
            return;
        popisKnihy(k, popis, sizeof popis);

        if (s->role == ROLE_SPRAVCE) {
            volby[n] = "Upravit knihu"; akce[n++] = UPRAVIT;
            volby[n] = "Smazat knihu";  akce[n++] = SMAZAT;
        }
        volby[n] = "Zpět"; akce[n++] = ZPET;

        int sel = uiChoice("Detail knihy", popis, volby, n);
        if (sel < 0 || akce[sel] == ZPET)
            return;

        switch (akce[sel]) {
        case UPRAVIT: upravitKnihu(idKnihy); break;
        case SMAZAT:  if (smazatKnihu(idKnihy)) return; break;
        }
    }
}

/* ======================== Knihy: menu ======================== */

void prehledKnih(Session* s)
{
    static Kniha* seznam[MAX_KNIH];
    (void)s;

    int n = vsechny_knihy(seznam, MAX_KNIH);
    if (n == 0) {
        uiMessage("Přehled knih", "V evidenci nejsou žádné knihy.");
        return;
    }
    vypisTabulkuKnih("Všechny knihy", seznam, n);
}

void pridatKnihu(Session* s)
{
    Kniha k = { 0 };
    char zprava[100];
    (void)s;

    if (!vyplnFormularKnihy("Nová kniha", &k))
        return;

    int id = pridej_knihu(&k);
    if (id <= 0) {
        uiError("Evidence knih je plná.");
        return;
    }

    snprintf(zprava, sizeof zprava, "Kniha přidána pod ID %d.", id);
    uiSuccess(zprava);
}

void hledatKnihu(Session* s)
{
    char text[50];
    char radky[MAX_VYSLEDKU][120];
    const char* volby[MAX_VYSLEDKU];
    Kniha* nalez[MAX_VYSLEDKU];
    int sel = 0;

    if (!uiReadText("Název nebo autor (i část)", text, sizeof text, 0))
        return;

    for (;;) {
        int n = hledej_knihy(text, nalez, MAX_VYSLEDKU);
        if (n == 0) {
            uiMessage("Hledání", "Kniha nebyla nalezena.");
            return;
        }

        for (int i = 0; i < n; i++) {
            snprintf(radky[i], sizeof radky[i], "%4d  %.25s - %.18s  [%s]", nalez[i]->ID,
                nalez[i]->nazev_knihy, nalez[i]->nazev_autora,
                nalez[i]->status == Vypujceno ? "vypůjčená" : "dostupná");
            volby[i] = radky[i];
        }

        sel = uiSelect("Výsledky hledání", volby, n, sel);
        if (sel < 0)
            return;
        detailKnihy(s, nalez[sel]->ID);
    }
}