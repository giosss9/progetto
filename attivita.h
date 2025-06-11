//Interfaccia del modulo: file attivita.h

#ifndef ATTIVITA_H
#define ATTIVITA_H

#include "data.h"
#define NULLATTIVITA NULL

//Dichiarazione del tipo attivita che è un puntatore alla struttura attivita_studio
typedef struct attivita_studio *attivita;

// Dichiarazione delle funzioni

/*
 * Funzione: crea_attivita
 * ---------------------------------------
 *
 * Parametri:
 *    d stringa che equivale alla descrizione
 *    c stringa che equivale al nome del corso
 *     g intero equivale al giorno della scadenza
 *     m intero equivale al mese della scadenza
 *     a intero equivale all anno della scadenza
 *     tempo intero equivale al tempo stimato per il completamento di un attivita
 *     pr intero equivale alla priorita di un' attivita
 *     ore intero equivale alle ore della data di scadenza
 *     id intero che rappresenta univocamente un'attivita
 *
 * Precondizioni:
 *      d e c
 *	      Devono puntare ad una stringa
 *	    tempo >0
 *	    0<=pr<=2
 *	    g dipende dal mese e dall anno
 *      1<=m<=12
 *      2025<=a<=2030
 *
 * Postcondizioni:
 *     Se tuttti i parametri sono validi e la memoria è stata allocata correttamente,
 *     restituisce una struttura di tipo attivita inizializzata con i parametri forniti,
 *     altrimenti restituisce NULLATTIVITA
 *
 * Effetti collaterali:
 *     Stampa su stdout eventuali messaggi di errore
 *
 */
attivita crea_attivita(char *d, char *c, int g, int m, int a, int tempo, int pr, int ore, int id);

/*
 * Funzione: stampa_attivita
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivita
 *
 * Precondizioni:
 *    a deve puntare ad una struttura attivita
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *     Stampa su stdout i campi di a
 *
 */
void stampa_attivita(attivita a);

/*
 * Funzioni: rit_campo (descrizione corso scadenza priorita stato id tempo_inizio)
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivita
 *
 * Precondizioni:
 *    a deve puntare ad una struttura valida
 *
 * Postcondizioni:
 *    Ritorna il campo corrispondente della struttura a il tipo dipende dal campo
 *     puo essere char*, data_ora e intero
 *     Ritorna NULLATTIVITA se a non punta ad una struttura attivita
 *
 */
char* rit_descrizione(attivita a);
char* rit_corso(attivita a);
data_ora rit_scadenza(attivita a);
int rit_tempo_stimato(attivita a);
int rit_priorita(attivita a);
int rit_stato(attivita a);
data_ora rit_tempo_inizio(attivita a);
int rit_id(attivita a);

/*
 * Funzione: imposta_stato
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivia
 *    stato di tipo intero
 *
 * Precondizioni:
 *    a deve puntare ad una struttura valida
 *    stato deve essere compreso tra 0 e 3
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *     Cambia il campo stato di a se è valido tutto
 *
 */
void imposta_stato(attivita a, int stato);

/*
 * Funzione: imposta_tempo_inizio
 * ---------------------------------------
 *
 * Parametri:
 *    a di tip attivita
 *
 * Precondizioni:
 *    a deve puntare ad una struttura attivita valida
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *    Assegna il tempo corrente al campo tempo_inizio di a.
 *     Se esiste una struttura data_ora gia inizializzata,
 *     nel campo tempo_inizio, la sovrascrive.
 *
 */
void imposta_tempo_inizio(attivita a);

/*
 * Funzione: libera_attivita
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivita
 *
 * Precondizioni:
 *    a deve puntare ad una struttura attivita
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *     Se i campi data_ora all'interno di a sono inizializzati li libera prima,
 *     successivamente libera il puntatore a alla struttura
 *
 */
void libera_attivita(attivita a);

#endif
