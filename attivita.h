#include "data.h"
#define NULLITEM NULL

typedef struct attivita_studio *attivita;

attivita crea_attivita(char *d, char *c, int g, int m, int a, int tempo, int pr, int st);
void stampa_attivita(attivita a);

int confronta_descrizione(attivita a, const char *descrizione);

char* rit_descrizione(attivita a);
char* rit_corso(attivita a);
data_ora rit_scadenza(attivita a);
int rit_tempo_stimato(attivita a);
int rit_priorita(attivita a);
int rit_stato(attivita a);
data_ora rit_tempo_creazione(attivita a);