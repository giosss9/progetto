#ifndef UTILE_H
#define UTILE_H

#include "lista.h"
#include "attivita.h"

lista carica_attivita_da_file(const char *nome_file, int *ultimo_id);
attivita inserisci_attivita_da_input(int *ultimo_id);

void menu(lista l, int *ultimo_id);

int dati_validi(int giorno, int mese, int anno, int ore, int tempo_stimato, int priorita);

#endif