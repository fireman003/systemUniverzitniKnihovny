#ifndef Defstruct
#define Defstruct


typedef struct
{
	int den;
	int mesic;
	int rok
} Datum;

typedef enum
{
	Rezervovano,
	Nerezervváno
} Rezervace;

typedef enum
{
	Výpujčeno,
	V_knihovně
} Status;

typedef struct
{
	int ID;
	char název_knihy[50];
	char název_autora[50];
	char žánr[50];
	int počet_stran;
	Datum datum_vydání;
	Status status;
	char Rezrevováno[50];
	Datum Rezervovano_pro_den;
	char výpůjčeno[50];
	Datum vypůjčeno_od;
	Datum vypůjčeno_do;
} Kniha;

typedef struct
{
	int ID;
	char jméno[50];
	char příjmení[50];
	char email[50];
	int telefon;
	Kniha vypůjčené_knihy[100];
	Kniha rezervované_knihy[100];
}Čtenář;

typedef struct
{
	int ID;
	char jméno[50];
	char příjmení[50];
	char email[50];
	int telefon;
} Knihovník;

#endif // Defsturct