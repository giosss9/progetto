//Implementazione del modulo: file attivita.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include "attivita.h"
#include "data.h"
#include "utile.h"

#define MAX 100
/*
 *	La struttura  rappresenta un’attività di studio che ha questi campi:
 *	    Id:
 *	        Un intero che va da 0 a n, dove n è il numero di attività inserite
 *	    Descrizione e corso
 *	        Due stringhe di lunghezza MAX(100), una rappresenta una breve descrizione dell’attività, l’altra il nome del corso
 *	    Scadenza:
 *	        Rappresenta la data e ora entro cui l’attività deve essere completata, è di tipo data_ora
 *	    Tempo_Stimato:
 *	        Rappresenta il tempo stimato per completare l’attività espresso in ore, dev’essere un valore positivo
 *	    Priorita:
 *	        Rappresenta il livello di priorità assegnato all’attività
 *	        Valori ammessi:
 *	            0 = bassa
 *	            1 = media
 *	            2 = alta
 *	    Stato:
 *	        Rappresenta lo stato attuale dell’attività
 *	        Valori ammessi:
 *	            0 = non iniziata (di default appena viene inserita un’attività)
 *	            1 = in corso
 *	            2 = completata
 *	            3 = in ritardo (se supera la data di scadenza)
 *	    Tempo_inizio
 *	        Rappresenta il momento in cui l’attività passa da non iniziata a in corso
 */

struct attivita_studio {
	int id;
    char descrizione[MAX];
    char corso[MAX];
    data_ora scadenza;
    int tempo_stimato;
    int priorita;
    int stato;
    data_ora tempo_inizio;
};

/*
 * Funzione: crea_attivita
 * -----------------------
 * Alloca e inizializza dinamicamente una struttura "attivita" con i parametri forniti.
 * Effettua controlli su input e crea la data di scadenza.
 *
 * Nota: Se la data non è valida o la memoria non viene allocata correttamente,
 *       la funzione restituisce NULLATTIVITA e stampa un messaggio di errore.
 */
attivita crea_attivita(char *d, char *c, int g, int m, int a, int tempo, int pr, int ore, int id){
	if (d == NULL || c == NULL) {
    	printf("Errore: descrizione o corso null\n");
    	return NULLATTIVITA;
	}
	if (!dati_validi(g,m,a,ore,tempo,pr)) {
    	printf("Errore: valori non validi\n");
    	return NULLATTIVITA;
	}

    attivita nuova = (attivita)malloc(sizeof(struct attivita_studio));
    if (nuova == NULL) {
        printf("Errore di allocazione memoria\n");
        return NULLATTIVITA;
    }

    strncpy(nuova->descrizione, d, MAX - 1);
    nuova->descrizione[MAX - 1] = '\0';
    strncpy(nuova->corso, c, MAX - 1);
    nuova->corso[MAX - 1] = '\0';

	nuova->scadenza = crea_data_ora(g, m, a, ore, 0, 0);
	if (nuova->scadenza == NULL) {
    	printf("Errore nella creazione della data\n");
    	free(nuova);
    	return NULLATTIVITA;
	}

    nuova->tempo_stimato = tempo;
    nuova->priorita = pr;
    nuova->stato = 0; 				// Stato iniziale: non iniziata
	nuova->id = id;
    nuova->tempo_inizio = NULL;
    return nuova;
}

/*
 * Funzione: stampa_attivita
 * -------------------------
 * Stampa a video tutti i campi rilevanti dell’attività.
 * Rende leggibile lo stato e la priorità con etichette testuali.
 */
void stampa_attivita(attivita a) {
    if (a == NULLATTIVITA) {
        printf("Attività non valida (NULL)\n");
        return;
    }

	printf("ID: %d\n", a->id);
    printf("Corso: %s\n", a->corso);
    printf("Descrizione: %s\n", a->descrizione);
    printf("Scadenza: %02d/%02d/%04d\n", rit_giorno(a->scadenza), rit_mese(a->scadenza), rit_anno(a->scadenza));
    printf("Tempo stimato: %d ore\n", a->tempo_stimato);

    printf("Priorità: ");
    switch (a->priorita) {
        case 0: printf("bassa\n"); break;
        case 1: printf("media\n"); break;
        case 2: printf("alta\n"); break;
        default: printf("sconosciuta\n"); break;
    }

    printf("Stato: ");
    switch (a->stato) {
        case 0: printf("non iniziata\n"); break;
        case 1: printf("in corso\n"); break;
        case 2: printf("completata\n"); break;
		case 3: printf("in ritardo\n"); break;
        default: printf("sconosciuto\n"); break;
    }
    printf("----------------------------------------\n");
}

/*
 * Funzioni di ritorno: rit_* (descrizione, corso, scadenza, tempo_stimato,
 *								priorita, stato, id, tempo_inizio)
 * --------------------------------------------------
 * Restituiscono i singoli campi della struttura.
 * In caso di puntatore NULL, restituiscono NULL o -1.
 */
char* rit_descrizione(attivita a) {
    if (a == NULLATTIVITA) return NULL;
    return a->descrizione;
}

char* rit_corso(attivita a) {
    if (a == NULLATTIVITA) return NULL;
    return a->corso;
}

data_ora rit_scadenza(attivita a) {
    if (a == NULLATTIVITA) return NULL;
    return a->scadenza;
}

int rit_tempo_stimato(attivita a) {
    if (a == NULLATTIVITA) return -1;
    return a->tempo_stimato;
}

int rit_priorita(attivita a) {
    if (a == NULLATTIVITA) return -1;
    return a->priorita;
}

int rit_stato(attivita a) {
    if (a == NULLATTIVITA) return -1;
    return a->stato;
}

int rit_id(attivita a) {
    if (a == NULLATTIVITA) return -1;
    return a->id;
}

data_ora rit_tempo_inizio(attivita a) {
    if (a == NULLATTIVITA) return NULL;
    return a->tempo_inizio;
}

/*
 * Funzione: imposta_stato
 * -----------------------
 * Imposta il campo "stato" con un valore tra 0 e 3.
 * Se lo stato è fuori range, stampa un errore e non effettua modifiche.
 *
 */
void imposta_stato(attivita a, int stato) {
	if (a == NULLATTIVITA) return;
    if (stato < 0 || stato > 3) {
        printf("Stato non valido\n");
        return;
    }
    a->stato = stato;
}

/*
 * Funzione: imposta_tempo_inizio
 * ------------------------------
 * Imposta il tempo di inizio dell’attività con l’orario corrente.
 * Libera un eventuale valore precedente per evitare perdite di memoria.
 */
void imposta_tempo_inizio(attivita a) {
    if (a == NULLATTIVITA){
		printf("Stato non aggiornato");
	 	return;
	}
    if (a->tempo_inizio != NULL) {
        free(a->tempo_inizio); // importante: libera la data precedente
        a->tempo_inizio = NULL;
    }

    a->tempo_inizio = ottieni_data_ora(); // crea nuova data corrente
}

/*
 * Funzione: libera_attivita
 * -------------------------
 * Libera memoria della struttura attività, inclusi i campi dinamici scadenza e tempo_inizio.
 *
 * Ordine corretto: prima si rilasciano i campi dinamici, poi la struttura principale.
 */
void libera_attivita(attivita a) {
    if (a == NULLATTIVITA) return;

    if (a->scadenza != NULL)
		libera_data_ora(a->scadenza);

    if (a->tempo_inizio != NULL)
		libera_data_ora(a->tempo_inizio);

    free(a);
}
