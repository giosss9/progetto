#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "data.h"
#include "utile.h"

struct data_ora {
    int giorno;
    int mese;
    int anno;
    int ore;
    int minuti;
    int secondi;
};

/*
 * Funzione: crea_data_ora
 * ---------------------------------------
 *
 * Parametri:
 *  Tutti interi
 *    giorno mese anno ore minuti secondi
 *
 * Precondizioni:
 *	    giorno ∈ [1, 31] (a seconda del mese e dell'anno)
 *	    mese ∈ [1, 12]
 *	    anno > 0
 *	    ore ∈ [0, 23]
 *	    minuti ∈ [0, 59]
 *	    secondi ∈ [0, 59]
 *
 * Postcondizioni:
 *	    Ritorna una struttura data_ora inizializzata con i parametri forniti
 *	    Se l’allocazione o il controllo per i dati fallisce, ritorna NULL.
 *
 */

data_ora crea_data_ora(int giorno, int mese, int anno, int ore, int minuti, int secondi) {
    //Controllo dati
  	if(!giorno_valido(giorno, mese, anno) == 1) {
      	printf("Errore validazione campi\n");
      	return NULL;
  	}
    if (anno < 2025 || anno > 2030) {
        return NULL;
    }
    if (ore < 0 || ore > 23) {
        return NULL;
    }
    if (minuti < 0 || minuti > 59) {
        return NULL;
    }
    if (secondi < 0 || secondi > 59) {
      return NULL;
    }


    data_ora nuova = malloc(sizeof(struct data_ora));
    if (nuova == NULL) {
        printf("Errore allocazione memoria data_ora\n");
        return NULL;
    }

    nuova->giorno = giorno;
    nuova->mese = mese;
    nuova->anno = anno;
    nuova->ore = ore;
    nuova->minuti = minuti;
    nuova->secondi = secondi;

    return nuova;
}

/*
 * Funzione: ottieni_data_ora
 * ---------------------------------------
 *
 * Parametri:
 *    Nessuno
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 *     Viene restituita una struttura data_ora contenente data e ora corrente del sistema.
 *     Se l’allocazione di memoria fallisce, ritorna NULL.
 *
 */

data_ora ottieni_data_ora() {
    time_t tempo_grezzo = time(NULL);
    struct tm *tempo_locale = localtime(&tempo_grezzo);

    data_ora data_corrente = malloc(sizeof(struct data_ora));
    if (data_corrente == NULL) {
        printf("Errore nell'allocazione della memoria\n");
        return NULL;
    }

    data_corrente->giorno = tempo_locale->tm_mday;
    data_corrente->mese = tempo_locale->tm_mon + 1;
    data_corrente->anno = tempo_locale->tm_year + 1900;
    data_corrente->ore = tempo_locale->tm_hour;
    data_corrente->minuti = tempo_locale->tm_min;
    data_corrente->secondi = tempo_locale->tm_sec;

    return data_corrente;
}

/*
 * Funzione: calcolo_tempo_trascorso
 * ---------------------------------------
 *
 * Parametri:
 *    inizio che è un tipo data_ora
 *
 * Precondizioni:
 *    inizio è un puntatore valido ad una struttura data_ora
 *    correttamente inizializzata.
 *
 * Postcondizioni:
 *     Retituisce una nuova data_ora che rappresenta
 *     il tempo trascorso tra la data inizio e quella corrente.
 *
 */

data_ora calcolo_tempo_trascorso(data_ora inizio) {
    if (inizio == NULL) {
        printf("Errore: data di inizio NULL\n");
        return NULL;
    }

    //Controllo campi
	int giorno=rit_giorno(inizio);
	int mese=rit_mese(inizio);
	int anno=rit_anno(inizio);
	int ore=rit_ore(inizio);

    if(!giorno_valido(giorno, mese, anno)) {
        printf("Errore validazione campi\n");
        return NULL;
    }
    if (anno < 2025 || anno > 2030) {
        return NULL;
    }
    if (ore < 0 || ore > 23) {
        return NULL;
    }

    // Costruisci struct tm per l'inizio
    struct tm t_inizio = {0};
    t_inizio.tm_year = inizio->anno - 1900;
    t_inizio.tm_mon  = inizio->mese - 1;
    t_inizio.tm_mday = inizio->giorno;
    t_inizio.tm_hour = inizio->ore;
    t_inizio.tm_min  = inizio->minuti;
    t_inizio.tm_sec  = inizio->secondi;

    time_t tempo_inizio = mktime(&t_inizio);
    if (tempo_inizio == -1) {
        fprintf(stderr, "Errore: mktime fallito per data inizio\n");
        return NULL;
    }

    data_ora adesso = ottieni_data_ora();
    if (adesso == NULL) {
        fprintf(stderr, "Errore: impossibile ottenere la data attuale\n");
        return NULL;
    }

    // Costruisci struct tm per ora attuale
    struct tm t_adesso = {0};
    t_adesso.tm_year = adesso->anno - 1900;
    t_adesso.tm_mon  = adesso->mese - 1;
    t_adesso.tm_mday = adesso->giorno;
    t_adesso.tm_hour = adesso->ore;
    t_adesso.tm_min  = adesso->minuti;
    t_adesso.tm_sec  = adesso->secondi;

    time_t tempo_attuale = mktime(&t_adesso);
    free(adesso); // non serve più

    if (tempo_attuale == -1) {
        fprintf(stderr, "Errore: mktime fallito per data attuale\n");
        return NULL;
    }

    double diff_sec = difftime(tempo_attuale, tempo_inizio);

    // Se la differenza è negativa (inizio nel futuro), forza a 0
    if (diff_sec < 0) {
        fprintf(stderr, "Attenzione: data di inizio è nel futuro\n");
        diff_sec = 0;
    }

    // Alloca e riempi la struct risultato
    data_ora risultato = malloc(sizeof(struct data_ora));
    if (!risultato) {
        fprintf(stderr, "Errore allocazione memoria risultato\n");
        return NULL;
    }

    risultato->giorno = (int)(diff_sec / (60 * 60 * 24));
    diff_sec -= risultato->giorno * 60 * 60 * 24;

    risultato->ore = (int)(diff_sec / (60 * 60));
    diff_sec -= risultato->ore * 60 * 60;

    risultato->minuti = (int)(diff_sec / 60);
    risultato->secondi = (int)(diff_sec - risultato->minuti * 60);

    // Campi non rilevanti per il tempo trascorso
    risultato->anno = 0;
    risultato->mese = 0;

    return risultato;
}

/*
 * Funzione: confronta_data_ora
 * ---------------------------------------
 *
 * Parametri:
 *    a primo tipo data_ora
 *    b secondo tipo data_ora
 *
 * Precondizioni:
 *    a e b devono essere due puntatori validi a strutture data_ora
 *
 * Postcondizioni:
 *     Restituisce:
 *         -1 se a è antecedente a b
 *          0 se sono uguali
 * 	        1 se a è successiva a b
 *          2 se a e b non sono validi
 */

int confronta_data_ora(data_ora a, data_ora b) {
  	if (a == NULL || b == NULL) {
        printf("Errore: data_ora non valida (NULL)\n");
        return 2;
    }
    int risultato = 0;

    if (a->anno != b->anno)
        risultato = (a->anno > b->anno) ? 1 : -1;
    else if (a->mese != b->mese)
        risultato = (a->mese > b->mese) ? 1 : -1;
    else if (a->giorno != b->giorno)
        risultato = (a->giorno > b->giorno) ? 1 : -1;
    else if (a->ore != b->ore)
        risultato = (a->ore > b->ore) ? 1 : -1;
    else if (a->minuti != b->minuti)
        risultato = (a->minuti > b->minuti) ? 1 : -1;
    else if (a->secondi != b->secondi)
        risultato = (a->secondi > b->secondi) ? 1 : -1;

    return risultato;
}

/*
 * Funzione: numero_settimana
 * ---------------------------------------
 *
 * Parametri:
 *    d tipo data_ora
 *
 * Precondizioni:
 *    d è un puntatore valido ad una struttura data_ora
 *
 * Postcondizioni:
 *    Restituisce il numero della settimana dell'anno
 *
 */

int numero_settimana(data_ora d) {
    if (d == NULL) return -1;
    int rit;
    struct tm tm_date = {
        .tm_year = d->anno - 1900,
        .tm_mon  = d->mese - 1,
        .tm_mday = d->giorno,
        .tm_hour = d->ore,
        .tm_min  = d->minuti,
        .tm_sec  = d->secondi
    };

    mktime(&tm_date); // Normalizzazione

    char buffer[3];
    strftime(buffer, sizeof(buffer), "%W", &tm_date); // %W: settimana (lunedì come inizio)
    rit=atoi(buffer);
    return rit;
}

/*
 * Funzione: rit_campo (giorno mese anno ore minuti secondi)
 * ---------------------------------------
 *
 * Parametri:
 *    d è un tipo data_ora
 *
 * Precondizioni:
 *    d è un puntatore valido ad una struttura data_ora
 *
 * Postcondizioni:
 *    Restituisce il campo della struttura d, se non è nulla.
 *
 */

int rit_giorno(data_ora d) {
    int giorno = (d != NULL) ? d->giorno : -1;
    return giorno;
}

int rit_mese(data_ora d) {
    int mese = (d != NULL) ? d->mese : -1;
    return mese;
}

int rit_anno(data_ora d) {
    int anno = (d != NULL) ? d->anno : -1;
    return anno;
}

int rit_ore(data_ora d) {
    int ore = (d != NULL) ? d->ore : -1;
    return ore;
}

int rit_minuti(data_ora d) {
    int minuti = (d != NULL) ? d->minuti : -1;
    return minuti;
}

int rit_secondi(data_ora d) {
    int secondi = (d != NULL) ? d->secondi : -1;
    return secondi;

}

/*
 * Funzione: imposta_ore
 * ---------------------------------------
 *
 * Parametri:
 *    d è un tipo data_ora
 *    ore tipo intero
 *
 * Precondizioni:
 *    d è un puntatore valido ad una struttura data_ora
 *    ore deve essere compreso tra 0 e 23
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *    Cambia il campo ore della struttura d
 *
 */

void imposta_ore(data_ora d, int ore){
	if(ore<0||ore>23)
		return;

	d->ore=ore;
}

/*
 * Funzione: imposta_giorno
 * ---------------------------------------
 *
 * Parametri:
 *    d è un tipo data_ora
 *    giorno tipo intero
 *
 * Precondizioni:
 *    d è un puntatore valido ad una struttura data_ora
 *    giorno deve essere valido a seconda del mese e dell'anno
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *    Cambia il campo giorno della struttura d
 *
 */

void imposta_giorno(data_ora d, int giorno){
    if(!giorno_valido(giorno, rit_mese(d), rit_anno(d)))
		return;

	d->giorno=giorno;
}

/*
 * Funzione: libera_data_ora
 * ---------------------------------------
 *
 * Parametri:
 *    d tipo data_ora
 *
 * Precondizioni:
 *    d è un puntatore valido a una struttura data_ora.
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *    Libera il puntatore d dalla struttura assegnatagli
 *
 */

void libera_data_ora(data_ora d) {
    if (d != NULL) {
        free(d);
    }
}

