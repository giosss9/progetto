#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attivita.h"
#include "lista.h"

struct nodo {
    attivita valore;
    struct nodo *successivo;
};

lista nuova_lista(void) {
    return NULL;
}

int lista_vuota(lista l) {
    return l == NULL;
}

// Funzione per aggiungere un nuovo elemento in testa alla lista
lista cons_lista(attivita val, lista l) {
    lista nuovo_nodo = (lista)malloc(sizeof(struct nodo));
    if (nuovo_nodo == NULL) {
        fprintf(stderr, "Errore: impossibile allocare memoria per il nuovo nodo.\n");
        exit(EXIT_FAILURE);
    }
    nuovo_nodo->valore = val;
    nuovo_nodo->successivo = l;
    return nuovo_nodo;
}

lista coda_lista(lista l) {
    if (lista_vuota(l)) {
        fprintf(stderr, "Errore: impossibile ottenere la coda di una lista vuota.\n");
        exit(EXIT_FAILURE);
    }
    return l->successivo;
}

attivita prendi_primo(lista l) {
    if (lista_vuota(l)) {
        fprintf(stderr, "Errore: impossibile ottenere il primo elemento di una lista vuota.\n");
        exit(EXIT_FAILURE);
    }
    return l->valore;
}

int dimensione_lista(lista l) {
    int conta = 0;
    while (!lista_vuota(l)) {
        conta++;
        l = l->successivo;
    }
    return conta;
}

void stampa_lista(lista l){
	while (!lista_vuota(l)){
		stampa_attivita(l->valore);
		l=l->successivo;
	}
}

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