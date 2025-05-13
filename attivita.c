#include <stdio.h>
#include "attivita.h"

#define MAX 100
//Definizione tipo data che servirà per la scadenza dell'attività
typedef struct {
    int giorno;
    int mese;
    int anno;
}data;

struct attivita_studio {
    char descrizione[MAX];
    char corso[MAX];
    data scadenza;
    int tempo_stimato;
    int priorita;        //0=bassa 1=media 2=alta
    int stato;          //0=non iniziata 1=in corso 2=completata
};

// Funzione che crea e restituisce una nuova attività.
// I parametri includono descrizione, corso, data di scadenza, tempo stimato,
// priorità e stato. Restituisce una struttura `attivita` inizializzata.
attivita crea_attivita(char *d, char *c, int g, int m, int a, int tempo, int pr, int st){
    attivita nuova = (attivita)malloc(sizeof(struct attivita_studio));
    if (nuova == NULL) {
        printf("Errore di allocazione memoria\n");
        return NULL;
    }

    strncpy(nuova->descrizione, d, MAX - 1);
    nuova->descrizione[MAX - 1] = '\0';
    strncpy(nuova->corso, c, MAX - 1);
    nuova->corso[MAX - 1] = '\0';
    nuova->scadenza.giorno = g;
    nuova->scadenza.mese = m;
    nuova->scadenza.anno = a;
    nuova->tempo_stimato = tempo;
    nuova->priorita = pr;
    nuova->stato = st;

    return nuova;
}

