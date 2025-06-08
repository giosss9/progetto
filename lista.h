#ifndef LISTA_H
#define LISTA_H

#include "attivita.h"

// Definizione del tipo lista (puntatore a nodo)
typedef struct nodo* lista;

// Dichiarazione delle funzioni

// Crea una nuova lista vuota
lista nuova_lista(void);

// Verifica se la lista è vuota
int lista_vuota(lista l);

// Aggiunge un elemento in testa alla lista
lista cons_lista(attivita val, lista l);

// Restituisce la coda della lista (tutti gli elementi tranne il primo)
lista coda_lista(lista l);

// Restituisce il primo elemento della lista
attivita prendi_primo(lista l);

// Restituisce il numero di elementi nella lista
int dimensione_lista(lista l);

// Stampa tutti gli elementi contenuti nella lista
void stampa_lista(lista l);

//Funzione che data una lista cerca un'attivita per la sua descrizione
attivita cerca_attivita_per_descrizione(lista l, const char *descrizione);

attivita cerca_attivita_per_id(lista l, const int id);

void stampa_lista_per_stato(lista l, int stato_richiesto);

lista rimuovi_attivita_per_id(lista l, int id);

void libera_lista(lista l);

#endif