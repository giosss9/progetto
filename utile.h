//Interfaccia del modulo: file utile.h

#ifndef UTILE_H
#define UTILE_H

#include "lista.h"
#include "attivita.h"

/*
 * Funzione: carica_attivita_da_file
 * ---------------------------------------
 *
 * Parametri:
 *    ultimo_id puntatore ad una variabile intera
 *    nome_file puntatore ad una stringa
 *
 * Precondizioni:
 *    ultimo_id deve puntare ad una variabile
 *    nome_file deve puntare ad un array di caratteri
 *
 * Postcondizioni:
 *    Se almeno un attivita è valida all'interno del file,
 *    restituisce una lista, altrimenti restiuisce NULL
 *
 * Effetti collaterali:
 *    Stampa su stdout eventuali messaggi di errore
 *     Al valore della variabile a cui punta ultimo_id viene assegnato l’id successivo da utilizzare
 *
 */
lista carica_attivita_da_file(const char *nome_file, int *ultimo_id);

/*
 * Funzione: inserisci_attivita_da_input
 * ---------------------------------------
 *
 * Parametri:
 *    ultimo_id puntatore ad una variabile intera
 *
 * Precondizioni:
 *    ultimo_id deve puntare ad una variabile
 *
 * Postcondizioni:
 *    Se tutti i dati sono validi,
 *    restituisce una nuova attività, altrimenti restiuisce NULLATTIVITA
 *
 * Effetti collaterali:
 *    Stampa su stdout messaggi e permette all'utente di inserire.
 *     Inolte se tutti i dati sono validi incremente il valore della variabile puntata da ultimo_id
 *
 */
attivita inserisci_attivita_da_input(int *ultimo_id);

/*
 * Funzione: calcolo_progresso
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivita
 *    trascorso di tipo data_ora
 *
 * Precondizioni:
 *    a non deve essere nulla
 *    trascorso non deve essere nullo
 *
 * Postcondizioni:
 *    Non restituisce niente
 *
 * Effetti collaterali:
 *    Stampa su stdout eventuali messaggi di errore
 *    Se va a buon fine stampa i dati di a e una barra
 *     che rappresenta il progresso di a
 *
 */
void calcolo_progresso(attivita a, data_ora trascorso);

/*
 * Funzione: aggiorna_stato
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivita
 *    scelta di tipo intero
 *
 * Precondizioni:
 *    a non deve essere nulla
 *    scelta deve essere 1 o 2
 *
 * Postcondizioni:
 *    Restituisce 0 se l'aggiornamento è riuscito
 *        1 se scelta non è valido
 *        2 se l'attivita è completata in ritardo, non è valida o è già in corso
 *
 * Effetti collaterali
 *     	Modifica lo stato di a, da 0(non iniziata) a 1(in corso)
 *         o da 1 a 2(completata)
 *     Nel primo caso imposta tempo_inizio di a alla data_ora corrente
 *
 */
int aggiorna_stato(attivita a, int scelta);

/*
 * Funzione: menu
 * ---------------------------------------
 *
 * Parametri:
 *    ultimo_id puntatore a intero
 *
 * Precondizioni:
 *    ultimo_id deve puntare a una variabile
 *
 * Postcondizioni:
 *    Non restituisce niente
 *
 * Effetti collaterali
 *     Stampa il menu del programma in cui l’utente può scegliere cosa fare,
 *     finché non sceglie 0 che equivale alla chiusura del programma.
 *
 */
void menu(lista l, int *ultimo_id);

/*
 * Funzione: dati_validi
 * ---------------------------------------
 *
 * Parametri:
 *    giorno, mese, anno ore, tempo_stimato interi
 *
 * Precondizioni:
 *    La combinazione giorno mese e anno deve essere valida
 *
 * Postcondizioni:
 *    Restituisce 1 se tutti i dati sono validi, altrimenti 0
 *
 */
int dati_validi(int giorno, int mese, int anno, int ore, int tempo_stimato, int priorita);

/*
 * Funzione: giorno_valido
 * ---------------------------------------
 *
 * Parametri:
 *    giorno mese e anno interi.
 *
 * Precondizioni:
 *    Nessuna.
 *
 * Postcondizioni:
 *    Restituisce 1 se la combinazione giorno/mese/anno rappresenta una data valida,
 *     altrimenti 0.
 *
 */
int giorno_valido(int giorno, int mese, int anno);

/*
 * Funzione: mostra_progresso
 * ---------------------------------------
 *
 * Parametri:
 *     l di tipolista
 *
 * Precondizioni:
 *    la lista l non deve essere vuota
 *
 * Postcondizioni:
 *    Non restituisce niente
*
 * Effetti collaterali
 *     Stamap su stdout il progresso di tutte le attivita presenti in l
 *
 */
void mostra_progresso(lista l);

/*
 * Funzione: genera_report_settimanale
 * ---------------------------------------
 *
 * Parametri:
 *    l di tipo lista
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 *    Non restituisce niente
 *
 * Effetti collaterali
 *     Stampa le attivita classificate in base alla settimana di scadenza
 *
 */
void genera_report_settimanale(lista l);

#endif