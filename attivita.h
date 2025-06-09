#ifndef ATTIVITA_H
#define ATTIVITA_H

#include "data.h"
#define NULLATTIVITA NULL

typedef struct attivita_studio *attivita;

attivita crea_attivita(char *d, char *c, int g, int m, int a, int tempo, int pr, int ore, int id);
void stampa_attivita(attivita a);

char* rit_descrizione(attivita a);
char* rit_corso(attivita a);
data_ora rit_scadenza(attivita a);
int rit_tempo_stimato(attivita a);
int rit_priorita(attivita a);
int rit_stato(attivita a);
data_ora rit_tempo_inizio(attivita a);

int rit_id(attivita a);

void imposta_stato(attivita a, int stato);
void imposta_tempo_inizio(attivita a);

void libera_attivita(attivita a);

#endif
