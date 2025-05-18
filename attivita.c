#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "attivita.h"
#include "data.h"

#define MAX 100
//Definizione tipo data che servirà per la scadenza dell'attività


struct attivita_studio {
    char descrizione[MAX];
    char corso[MAX];
    data_ora scadenza;
    int tempo_stimato;
    int priorita;        //0=bassa 1=media 2=alta
    int stato;          //0=non iniziata 1=in corso 2=completata
    data_ora tempo_creazione; //per tenere conto del tempo trascorso
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

	nuova->scadenza = (data_ora) malloc(sizeof(data_ora));
    if (nuova->scadenza == NULL) {
        printf("Errore di allocazione memoria per scadenza\n");
        free(nuova);
        return NULL;
    }

    strncpy(nuova->descrizione, d, MAX - 1);
    nuova->descrizione[MAX - 1] = '\0';
    strncpy(nuova->corso, c, MAX - 1);
    nuova->corso[MAX - 1] = '\0';
    nuova->scadenza->giorno = g;
    nuova->scadenza->mese = m;
    nuova->scadenza->anno = a;
	nuova->scadenza->ore=0;
	nuova->scadenza->minuti=0;
	nuova->scadenza->secondi=0;
    nuova->tempo_stimato = tempo;
    nuova->priorita = pr;
    nuova->stato = st;

    nuova->tempo_creazione = ottieni_data_ora();
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
    printf("Scadenza: %02d/%02d/%04d\n", a->scadenza->giorno, a->scadenza->mese, a->scadenza->anno);
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


//Funzioni che ritornano le variabili di attivita
char* rit_descrizione(attivita a) {
    return a->descrizione;
}

char* rit_corso(attivita a) {
    return a->corso;
}

data_ora rit_scadenza(attivita a) {
    return a->scadenza;
}

int rit_tempo_stimato(attivita a) {
    return a->tempo_stimato;
}

int rit_priorita(attivita a) {
    return a->priorita;
}

int rit_stato(attivita a) {
    return a->stato;
}

data_ora rit_tempo_creazione(attivita a) {
    return a->tempo_creazione;
}


int confronta_descrizione(attivita a, const char *descrizione){
    int verifica=0;
    if (strcmp(a->descrizione, descrizione) == 0)
            verifica=1;
    return verifica;
}
