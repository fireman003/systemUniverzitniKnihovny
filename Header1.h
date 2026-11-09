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

#endif // Defsturct