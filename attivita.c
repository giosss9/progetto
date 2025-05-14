#include <stdio.h>
#include <string.h>
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

//Funzione che stampa l'attivita passata come argomento e non restituisce niente
void stampa_attivita(attivita a) {
    if (a == NULL) {
        printf("Attività non valida (NULL)\n");
        return;
    }

    printf("Corso: %s\n", a->corso);
    printf("Descrizione: %s\n", a->descrizione);
    printf("Scadenza: %02d/%02d/%04d\n", a->scadenza.giorno, a->scadenza.mese, a->scadenza.anno);
    printf("Tempo stimato: %d ore\n", a->tempo_stimato);

    // Stampa priorità in formato leggibile
    printf("Priorità: ");
    switch (a->priorita) {
        case 0: printf("bassa\n"); break;
        case 1: printf("media\n"); break;
        case 2: printf("alta\n"); break;
        default: printf("sconosciuta\n"); break;
    }

    // Stampa stato in formato leggibile
    printf("Stato: ");
    switch (a->stato) {
        case 0: printf("non iniziata\n"); break;
        case 1: printf("in corso\n"); break;
        case 2: printf("completata\n"); break;
        default: printf("sconosciuto\n"); break;
    }
}


