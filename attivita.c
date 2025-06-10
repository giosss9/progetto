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
 * ---------------------------------------
 *
 * Parametri:
 *    d stringa che equivale alla descrizione
 *    c stringa che equivale al nome del corso
 *     g intero equivale al giorno della scadenza
 *     m intero equivale al mese della scadenza
 *     a intero equivale all anno della scadenza
 *     tempo intero equivale al tempo stimato per il completamento di un attivita
 *     pr intero equivale alla priorita di un' attivita
 *     ore intero equivale alle ore della data di scadenza
 *     id intero che rappresenta univocamente un'attivita
 *
 * Precondizioni:
 *      d e c
 *	      Devono puntare ad una stringa
 *	    tempo >0
 *	    0<=pr<=2
 *	    g dipende dal mese e dall anno
 *      1<=m<=12
 *      2025<=a<=2030
 *
 * Postcondizioni:
 *     Se tuttti i parametri sono validi e la memoria è stata allocata correttamente,
 *     restituisce una struttura di tipo attivita inizializzata con i parametri forniti,
 *     altrimenti restituisce NULLATTIVITA
 *
 * Effetti collaterali:
 *     Stampa su stdout eventuali messaggi di errore
 *
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
    nuova->stato = 0;
	nuova->id = id;
    nuova->tempo_inizio = NULL;
    return nuova;
}

/*
 * Funzione: stampa_attivita
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivita
 *
 * Precondizioni:
 *    a deve puntare ad una struttura attivita
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *     Stampa su stdout i campi di a
 *
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
 * Funzione: rit_campo (descrizione corso scadenza priorita stato id tempo_inizio)
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivita
 *
 * Precondizioni:
 *    a deve puntare ad una struttura valida
 *
 * Postcondizioni:
 *    Ritorna il campo corrispondente della struttura a il tipo dipende dal campo
 *     puo essere char*, data_ora e intero
 *     Ritorna NULLATTIVITA se a non punta ad una struttura attivita
 *
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
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivia
 *    stato di tipo intero
 *
 * Precondizioni:
 *    a deve puntare ad una struttura valida
 *    stato deve essere compreso tra 0 e 3
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *     Cambia il campo stato di a se è valido tutto
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
 * ---------------------------------------
 *
 * Parametri:
 *    a di tip attivita
 *
 * Precondizioni:
 *    a deve puntare ad una struttura attivita valida
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *    Assegna il tempo corrente al campo tempo_inizio di a.
 *     Se esiste una struttura data_ora gia inizializzata,
 *     nel campo tempo_inizio, la sovrascrive.
 *
 */

void imposta_tempo_inizio(attivita a) {
    if (a == NULLATTIVITA){
		printf("Stato non aggiornato");
	 	return;
	}
    if (a->tempo_inizio != NULL) {
        free(a->tempo_inizio);
        a->tempo_inizio = NULL;
    }

    a->tempo_inizio = ottieni_data_ora();
}

/*
 * Funzione: libera_attivita
 * ---------------------------------------
 *
 * Parametri:
 *    a di tipo attivita
 *
 * Precondizioni:
 *    a deve puntare ad una struttura attivita
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *     Se i campi data_ora all'interno di a sono inizializzati li libera prima,
 *     successivamente libera il puntatore a alla struttura
 *
 */

void libera_attivita(attivita a) {
    if (a == NULLATTIVITA) return;

    if (a->scadenza != NULL)
		libera_data_ora(a->scadenza);

    if (a->tempo_inizio != NULL)
		libera_data_ora(a->tempo_inizio);

    free(a);
}
