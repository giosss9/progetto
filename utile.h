#ifndef UTILE_H
#define UTILE_H

lista carica_attivita_da_file(const char *nome_file);
attivita inserisci_attivita_da_input();

void mostra_progresso(lista l);

void controlla_ritardo(attivita a);
int aggiorna_stato(attivita a, int scelta);

void genera_report_settimanale(lista l);

#endif