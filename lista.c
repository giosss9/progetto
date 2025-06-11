//Implementazione del modulo: file lista.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attivita.h"
#include "lista.h"

struct nodo {
    attivita valore;
    struct nodo *successivo;
};

/*
 * Funzione: nuova_lista
 * ---------------------
 * Crea e restituisce una nuova lista vuota.
 */

lista nuova_lista(void) {
    return NULL;
}


/*
 * Funzione: lista_vuota
 * ---------------------
 * Verifica se una lista è vuota.
 */

int lista_vuota(lista l) {
    if (l == NULL)
      return 1;
    else
      return 0;
}

/*
 * Funzione: cons_lista
 * --------------------
 * Costruisce una nuova lista aggiungendo un elemento in testa.
 */

lista cons_lista(attivita val, lista l) {
    if(val == NULLATTIVITA)
      return NULL;

    lista nuovo_nodo = (lista)malloc(sizeof(struct nodo));
    if (nuovo_nodo == NULL) {
        printf("Errore: impossibile allocare memoria per il nuovo nodo.\n");
        return NULL;
    }
    nuovo_nodo->valore = val;
    nuovo_nodo->successivo = l;
    return nuovo_nodo;
}


/*
 * Funzione: coda_lista
 * --------------------
 * Restituisce la coda della lista, ovvero tutti gli elementi tranne il primo.
 */

lista coda_lista(lista l) {
    if (lista_vuota(l)) {
        printf("Errore: impossibile ottenere la coda di una lista vuota.\n");
        return NULL;
    }
    lista coda=l->successivo;
    return coda;
}


/*
 * Funzione: prendi_primo
 * ----------------------
 * Restituisce il primo elemento della lista.
 */

attivita prendi_primo(lista l) {
    if (lista_vuota(l)) {
        printf("Errore: impossibile ottenere il primo elemento di una lista vuota.\n");
        return NULL;
    }
    return l->valore;
}


/*
 * Funzione: dimensione_lista
 * --------------------------
 * Calcola e restituisce il numero di elementi della lista.
 */

int dimensione_lista(lista l) {
    int conta = 0;
    while (!lista_vuota(l)) {
        conta++;
        l = l->successivo;
    }
    return conta;
}


/*
 * Funzione: stampa_lista
 * ----------------------
 * Stampa tutti gli elementi della lista.
 */

void stampa_lista(lista l){
	while (!lista_vuota(l)){
		stampa_attivita(l->valore);
		l=l->successivo;
	}
}


/*
 * Funzione: stampa_lista_per_stato
 * --------------------------------
 * Stampa gli elementi della lista che corrispondono a uno stato specifico.
 */

void stampa_lista_per_stato(lista l, int stato_richiesto) {
    if (lista_vuota(l)) {
        printf("La lista è vuota.\n");
        return;
    }

    int trovata = 0;
    while (!lista_vuota(l)) {
        if (rit_stato(l->valore) == stato_richiesto) {
            stampa_attivita(l->valore);
            trovata = 1;
        }
        l = l->successivo;
    }

    if (!trovata) {
        printf("Nessuna attività trovata con lo stato richiesto (%d).\n", stato_richiesto);
    }

    printf("\n");
}


/*
 * Funzione: cerca_attivita_per_id
 * -------------------------------
 * Cerca un'attività nella lista tramite il suo identificatore.
 */

attivita cerca_attivita_per_id(lista l, const int id) {
    if (lista_vuota(l)) {
        printf("La lista è vuota.\n");
        return NULLATTIVITA;
    }

    while (!lista_vuota(l)) {
        if (l->valore != NULL) {
            if(id==rit_id(l->valore))
                return l->valore;
        }
        l = l->successivo;
    }

    return NULLATTIVITA;
}


/*
 * Funzione: rimuovi_attivita_per_id
 * ---------------------------------
 * Rimuove un’attività dalla lista in base al suo identificatore.
 */

lista rimuovi_attivita_per_id(lista l, int id) {
    if (lista_vuota(l)) return NULL;

    // Caso 1: primo nodo è quello da rimuovere
    if (rit_id(l->valore) == id) {
        lista nuovo_inizio = l->successivo;
        libera_attivita(l->valore);
        free(l);
        return nuovo_inizio;
    }

    // Caso 2: nodo da rimuovere è in mezzo o in fondo
    lista corrente = l;
    while (corrente->successivo != NULL) {
        if (rit_id(corrente->successivo->valore) == id) {
            lista da_rimuovere = corrente->successivo;
            corrente->successivo = da_rimuovere->successivo;
            libera_attivita(da_rimuovere->valore);
            free(da_rimuovere);
            break;
        }
        corrente = corrente->successivo;
    }

    return l;
}

/*
 * Funzione: libera_lista
 * ----------------------
 * Libera tutta la memoria occupata da una lista e dalle attività in essa contenute.
 */

void libera_lista(lista l) {
    while (!lista_vuota(l)) {
        lista temp = l;
        l = l->successivo;

        attivita a = temp->valore;

        // Libera i campi dinamici dell'attività
        libera_attivita(a);

        // Libera il nodo della lista
        free(temp);
    }
}