#ifndef UTILE_H
#define UTILE_H

lista carica_attivita_da_file(const char *nome_file, int *ultimo_id);
attivita inserisci_attivita_da_input(int *ultimo_id);

void mostra_progresso(lista l);

void controlla_ritardo(attivita a);
int aggiorna_stato(attivita a, int scelta);

void genera_report_settimanale(lista l);

void menu(lista l, int *ultimo_id);

int dati_validi(int giorno, int mese, int anno, int ore, int tempo_stimato, int priorita);

#endif