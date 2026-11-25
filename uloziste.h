#ifndef Defstruct
#define Defstruct


typedef struct
{
	int den;
	int mesic;
	int rok;
} Datum;
/*
typedef enum
{
	Rezervovano,
	Nerezervovano
} Rezervace;*/

typedef struct
{
	int ID;
	Datum Rezervovano_pro_den;
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
} Kniha;

typedef struct 
{
	int ID;
	Datum vypujceno_od;
	Datum vypujceno_do;
} Vypujcka;

typedef struct
{
	int ID;
	char jmeno[50];
	char prijmeni[50];
	char email[50];
	int telefon;
	Vypujcka vypujcene_knihy;
	Rezervace rezervovane_knihy;
} Ctenar;

typedef struct
{
	int ID;
	char jmeno[50];
	char prijmeni[50];
	char email[50];
	int telefon;
} Knihovnik;



int uloz_knihy(Kniha pole_knih[], int pocet_knih, const char* nazev_souboru);
int nacti_knihovniky(Knihovnik pole_knihovniku[]);
int uloz_ctenare(Ctenar pole_ctenaru[], int pocet_ctenaru, int pocet_knih, const char* nazev_souboru);
int nacti_knihy(Kniha pole_knih[], int max_knih, const char* nazev_souboru);

#endif // Defstruct