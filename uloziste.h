#ifndef Defstruct
#define Defstruct


typedef struct
{
	int den;
	int mesic;
	int rok;
} Datum;

typedef enum
{
	Rezervovano,
	Nerezervovano
} Rezervace;

typedef enum
{
	Vypujceno,
	V_knihovne
} Status;

typedef struct
{
	int ID;
	char nazev_knihy[50];
	char nazev_autora[50];
	char zanr[50];
	int pocet_stran;
	Datum datum_vydani;
	Status status;
	char Rezervovano_pro[50];
	Datum Rezervovano_pro_den;
	int vypujceno;
	Datum vypujceno_od;
	Datum vypujceno_do;
} Kniha;

typedef struct
{
	int ID;
	char jmeno[50];
	char prijmeni[50];
	char email[50];
	int telefon;
	Kniha vypujcene_knihy[100];
	Kniha rezervovane_knihy[100];
} Ctenar;

typedef struct
{
	int ID;
	char jmeno[50];
	char prijmeni[50];
	char email[50];
	int telefon;
} Knihovnik;

typedef struct {
	int ID;
} Vypujcka;

int uloz_knihy(Kniha pole_knih[], int pocet_knih, const char* nazev_souboru);
int nacti_knihovniky(Knihovnik pole_knihovniku[]);
int uloz_ctenare(Ctenar pole_ctenaru[], int pocet_ctenaru, int pocet_knih, const char* nazev_souboru);
int nacti_knihy(Kniha pole_knih[], int max_knih, const char* nazev_souboru);

#endif // Defstruct