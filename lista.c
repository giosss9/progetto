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
lista nuova_lista(void) {
    return NULL;
}


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
int lista_vuota(lista l) {
    if (l == NULL)
      return 1;
    else
      return 0;
}

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

attivita prendi_primo(lista l) {
    if (lista_vuota(l)) {
        printf("Errore: impossibile ottenere il primo elemento di una lista vuota.\n");
        return NULL;
    }
    return l->valore;
}


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

void stampa_lista(lista l){
	while (!lista_vuota(l)){
		stampa_attivita(l->valore);
		l=l->successivo;
	}
}


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