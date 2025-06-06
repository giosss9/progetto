#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include "attivita.h"
#include "data.h"

#define MAX 100

struct attivita_studio {
    char descrizione[MAX];
    char corso[MAX];
    data_ora scadenza;
    int tempo_stimato;
    int priorita;        //0=bassa 1=media 2=alta
    int stato;          //0=non iniziata 1=in corso 2=completata 3=in ritardo
    data_ora tempo_inizio; //per tenere conto del tempo trascorso, viene  memoriizzato quando l'utente imposta che l'attivita è in corso
};

// Funzione che crea e restituisce una nuova attività.
// I parametri includono descrizione, corso, data di scadenza, tempo stimato,
// priorità e stato. Restituisce una struttura `attivita` inizializzata.
attivita crea_attivita(char *d, char *c, int g, int m, int a, int tempo, int pr, int ore){
	if (d == NULL || c == NULL) {
    	printf("Errore: descrizione o corso null\n");
    	return NULL;
	}
	if (tempo < 0 || pr < 0 || pr > 2 || ore < 0 || ore > 23 || g<0 || g>31 || m<0 || m>12 || a<2020 || a>2030) {
    	printf("Errore: valori non validi\n");
    	return NULL;
	}

    attivita nuova = (attivita)malloc(sizeof(struct attivita_studio));
    if (nuova == NULL) {
        printf("Errore di allocazione memoria\n");
        return NULL;
    }

    strncpy(nuova->descrizione, d, MAX - 1);
    nuova->descrizione[MAX - 1] = '\0';
    strncpy(nuova->corso, c, MAX - 1);
    nuova->corso[MAX - 1] = '\0';

	nuova->scadenza = crea_data_ora(g, m, a, ore, 0, 0);
	if (nuova->scadenza == NULL) {
    	printf("Errore nella creazione della data\n");
    	free(nuova);
    	return NULL;
	}

    nuova->tempo_stimato = tempo;
    nuova->priorita = pr;
    nuova->stato = 0;
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
    printf("Scadenza: %02d/%02d/%04d\n", rit_giorno(a->scadenza), rit_mese(a->scadenza), rit_anno(a->scadenza));
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
    printf("-----------------------------------\n");
}


//Funzioni che ritornano le variabili di attivita
char* rit_descrizione(attivita a) {
    char *descr = NULL;
    if (a != NULL)
        descr = a->descrizione;
    return descr;
}

char* rit_corso(attivita a) {
    char *corso = NULL;
    if (a != NULL)
        corso = a->corso;
    return corso;
}

data_ora rit_scadenza(attivita a) {
    data_ora d = NULL;
    if (a != NULL)
        d = a->scadenza;
    return d;
}

int rit_tempo_stimato(attivita a) {
    int tempo = -1;
    if (a != NULL)
        tempo = a->tempo_stimato;
    return tempo;
}

int rit_priorita(attivita a) {
    int priorita = -1;
    if (a != NULL)
        priorita = a->priorita;
    return priorita;
}

int rit_stato(attivita a) {
    int stato = -1;
    if (a != NULL)
        stato = a->stato;
    return stato;
}


data_ora rit_tempo_inizio(attivita a) {
    data_ora t = NULL;
    if (a != NULL)
        t = a->tempo_inizio;
    return t;
}

// Confronta la descrizione dell'attività con quella fornita esattamente (case-sensitive).
// Restituisce 1 se le stringhe coincidono perfettamente, altrimenti 0.
// Gestisce anche eventuali puntatori NULL per evitare errori di segmentazione.
int confronta_descrizione(attivita a, const char *descrizione) {
    int risultato = 0;
    if (a != NULL && descrizione != NULL)
        risultato = strcmp(a->descrizione, descrizione) == 0;
    return risultato;
}


void imposta_stato(attivita a, int stato) {
	if (a == NULL) return;
    if (stato < 0 || stato > 3) {
        printf("Stato non valido\n");
        return;
    }
    a->stato = stato;
}

void libera_attivita(attivita a) {
    if (a == NULL) return;

    if (a->scadenza != NULL)
		libera_data_ora(a->scadenza);

    if (a->tempo_inizio != NULL)
		libera_data_ora(a->tempo_inizio);

    free(a);
}
