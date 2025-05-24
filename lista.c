#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "attivita.h"
#include "lista.h"

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
    while (l != NULL) {
        conta++;
        l = l->successivo;
    }
    return conta;
}

void stampa_lista(lista l) {
	if (lista_vuota(l)) {
    	printf("La lista è vuota.\n");
	}
    while (l != NULL) {
        stampa_attivita(l->valore);
        printf(" ");
        l = l->successivo;
    }
    printf("\n");
}
attivita cerca_attivita_per_descrizione(lista l, const char *descrizione) {
    if (lista_vuota(l)) {
        printf("La lista è vuota.\n");
        return NULLITEM;
    }

    int verifica;
    while (l != NULL) {
        if (l->valore != NULL) {
            verifica = confronta_descrizione(l->valore, descrizione);
            if (verifica == 1)
                return l->valore;
        }
        l = l->successivo;
    }

    return NULLITEM;
}

