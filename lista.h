#ifndef LISTA_H
#define LISTA_H

#include "attivita.h"

// Definizione del tipo lista (puntatore a nodo)
typedef struct nodo* lista;

// Dichiarazione delle funzioni

/*
 * Funzione: nuova_lista
 * ---------------------------------------
 *
 * Parametri:
 *    Nessuno
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 *    Restituisce una lista vuota
 *
 */
lista nuova_lista(void);

/*
 * Funzione: lista_vuota
 * ---------------------------------------
 *
 * Parametri:
 *     l di tipo lista
 *
 * Precondizioni:
 *    Non ci sono
 *
 * Postcondizioni:
 *    Restituisce 1 se la lista è vuota, altrimenti 0
 *
 */
int lista_vuota(lista l);

/*
 * Funzione: cons_lista
 * ---------------------------------------
 *
 * Parametri:
 *    val di tipo attivita
 *     l di tipo lista
 *
 * Precondizioni:
 *    val deve puntare ad una struttura attivita valida
 *
 * Postcondizioni:
 *    Ritorna il puntatore al nuovo nodo, cioè al nuovo inizio della lista l
 *
 * Effetti collaterali
 *    Aggiunge in testa alla lista il nuovo elemento val
 *
 *
 */
lista cons_lista(attivita val, lista l);

/*
 * Funzione: coda_lista
 * ---------------------------------------
 *
 * Parametri:
 *    l di tipo lista
 *
 * Precondizioni:
 *    l non dev'essere vuota
 *
 * Postcondizioni:
 *    Restituisce la lista senza il primo elemento se esiste, altrimenti NULL
 *
 */
lista coda_lista(lista l);

/*
 * Funzione: prendi_primo
 * ---------------------------------------
 *
 * Parametri:
 *    l di tipo lista
 *
 * Precondizioni:
 *    l non deve essere vuota
 *
 * Postcondizioni:
 *    Restituisce il primo elemento della lista l se esiste, altrimenti NULL
 *
 */
attivita prendi_primo(lista l);

/*
 * Funzione: dimensione_lista
 * ---------------------------------------
 *
 * Parametri:
 *    l di tipo lista
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 *    Restituisce il numero di elementi della lista, se è vuota restituisce 0
 *
 */
int dimensione_lista(lista l);

/*
 * Funzione: stampa_lista
 * ---------------------------------------
 *
 * Parametri:
 *    l di tipo lista
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 *    Nessuna
 *
 * Effetti collaterali:
 *     Stampa a schermo la lista, se è vuota non stampa niente
 *
 */
void stampa_lista(lista l);


/*
 * Funzione: cerca_attivita_per_id
 * ---------------------------------------
 *
 * Parametri:
 *    l di tipo lista
 *    id di tipo intero costante
 *
 * Precondizioni:
 *    l non deve essere vuota
 *
 * Postcondizioni:
 *    Se id corrisponde all'id di uno degli elementi della lista
 *     ritorna il corrispettivo elemento, altrimenti ritorna NULLATTIVITA
 *
 */
attivita cerca_attivita_per_id(lista l, const int id);

/*
 * Funzione: rimuovi_attivita_per_id
 * ---------------------------------------
 *
 * Parametri:
 *    l di tipo lista
 *    id di tipo intero
 *
 * Precondizioni:
 *    l non deve essere vuota
 *
 * Postcondizioni:
 *    Restituisce la lista con, eventualmente, un elemento rimosso,
 *     se lo trova tramite id
 *
 */
lista rimuovi_attivita_per_id(lista l, int id);

/*
 * Funzione: stampa_lista_per_stato
 * ---------------------------------------
 *
 * Parametri:
 *    l di tipo lista
 *    stato_richiesto di tipo intero
 *
 * Precondizioni:
 *    l non dev'esssere vuota
 *
 * Postcondizioni:
 *    Nessuna
 *
 * Efetti collaterali
 *    Stampa su stdout gli elementi della lista che hanno lo stato_richiesto
 *
 */
void stampa_lista_per_stato(lista l, int stato_richiesto);


/*
 * Funzione: libera_lista
 * ---------------------------------------
 *
 * Parametri:
 *    l di tipo lista
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 *    Nessuna
 *
 * Effetti collaterali
 *     Libera la memoria occupata dalla lista l
 *
 */
void libera_lista(lista l);

#endif