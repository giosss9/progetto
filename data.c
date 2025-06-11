//Implementazione del modulo: file data.c

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
 * ------------------------
 * Crea e inizializza una struttura data_ora con i valori forniti.
 * Viene effettuata la validazione dei parametri prima di allocare memoria.
 * Se i dati non sono validi o l’allocazione fallisce, restituisce NULL.
 */
data_ora crea_data_ora(int giorno, int mese, int anno, int ore, int minuti, int secondi) {
    // Controllo dati
    if (!giorno_valido(giorno, mese, anno)) {
        printf("Errore validazione campi\n");
        return NULL;
    }
    if (anno < 2025 || anno > 2030 || ore < 0 || ore > 23 || minuti < 0 || minuti > 59 || secondi < 0 || secondi > 59) {
        return NULL;
    }

    // Allocazione struttura e inizializzazione campi
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
 * --------------------------
 * Raccoglie data e ora correnti dal sistema e crea una struttura data_ora corrispondente.
 * Se l’allocazione fallisce, restituisce NULL.
 */
data_ora ottieni_data_ora() {
    time_t tempo_grezzo = time(NULL); //Ottiene il tempo corrente espresso in secondi
    struct tm *tempo_locale = localtime(&tempo_grezzo); //Converte il valore tempo_grezzo in componenti leggibili

    // Alloca struttura per contenere data e ora
    data_ora data_corrente = malloc(sizeof(struct data_ora));
    if (data_corrente == NULL) {
        printf("Errore nell'allocazione della memoria\n");
        return NULL;
    }

    // Copia i campi dalla struct tm del sistema
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
 * ---------------------------------
 * Calcola la differenza tra una data data_ora e la data/ora attuale.
 * Restituisce una nuova struttura con i giorni, ore, minuti e secondi trascorsi.
 */
data_ora calcolo_tempo_trascorso(data_ora inizio) {
    if (inizio == NULL) {
        printf("Errore: data di inizio NULL\n");
        return NULL;
    }

    // Validazione dei campi essenziali
    int giorno = rit_giorno(inizio);
    int mese = rit_mese(inizio);
    int anno = rit_anno(inizio);
    int ore = rit_ore(inizio);

    if (!giorno_valido(giorno, mese, anno) || anno < 2025 || anno > 2030 || ore < 0 || ore > 23) {
        printf("Errore validazione campi\n");
        return NULL;
    }

    // Conversione in time_t per il calcolo della differenza
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

    // Ottiene data e ora correnti
    data_ora adesso = ottieni_data_ora();
    if (adesso == NULL) {
        fprintf(stderr, "Errore: impossibile ottenere la data attuale\n");
        return NULL;
    }

	//Li converte in secondi
    struct tm t_adesso = {0};
    t_adesso.tm_year = adesso->anno - 1900;
    t_adesso.tm_mon  = adesso->mese - 1;
    t_adesso.tm_mday = adesso->giorno;
    t_adesso.tm_hour = adesso->ore;
    t_adesso.tm_min  = adesso->minuti;
    t_adesso.tm_sec  = adesso->secondi;

    time_t tempo_attuale = mktime(&t_adesso);
    free(adesso);

    if (tempo_attuale == -1) {
        fprintf(stderr, "Errore: mktime fallito per data attuale\n");
        return NULL;
    }

    // Calcola differenza in secondi
    double diff_sec = difftime(tempo_attuale, tempo_inizio);
    if (diff_sec < 0) {
        fprintf(stderr, "Attenzione: data di inizio è nel futuro\n");
        diff_sec = 0;
    }

    // Costruzione risultato con giorni, ore, minuti, secondi
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

    risultato->anno = 0;
    risultato->mese = 0;

    return risultato;
}

/*
 * Funzione: confronta_data_ora
 * ----------------------------
 * Confronta due strutture data_ora campo per campo in ordine di significatività.
 * Restituisce:
 *  -1 se a < b
 *   0 se a == b
 *   1 se a > b
 *   2 se uno dei due puntatori è NULL
 */
int confronta_data_ora(data_ora a, data_ora b) {
    if (a == NULL || b == NULL) {
        printf("Errore: data_ora non valida (NULL)\n");
        return 2;
    }

    // Confronto sequenziale per anno, mese, giorno, ora, minuto, secondo
    if (a->anno != b->anno) return (a->anno > b->anno) ? 1 : -1;
    if (a->mese != b->mese) return (a->mese > b->mese) ? 1 : -1;
    if (a->giorno != b->giorno) return (a->giorno > b->giorno) ? 1 : -1;
    if (a->ore != b->ore) return (a->ore > b->ore) ? 1 : -1;
    if (a->minuti != b->minuti) return (a->minuti > b->minuti) ? 1 : -1;
    if (a->secondi != b->secondi) return (a->secondi > b->secondi) ? 1 : -1;

    return 0;
}

/*
 * Funzione: numero_settimana
 * --------------------------
 * Calcola il numero della settimana dell’anno per una data.
 * Usa la funzione `strftime` con il formato %W (settimana con lunedì come inizio).
 */
int numero_settimana(data_ora d) {
    if (d == NULL) return -1;

    struct tm tm_date = {
        .tm_year = d->anno - 1900,
        .tm_mon  = d->mese - 1,
        .tm_mday = d->giorno,
        .tm_hour = d->ore,
        .tm_min  = d->minuti,
        .tm_sec  = d->secondi
    };

    mktime(&tm_date); // Normalizza la struttura tm

    char buffer[3];
    strftime(buffer, sizeof(buffer), "%W", &tm_date); // Ottiene la settimana
    return atoi(buffer);
}

/*
 * Funzioni di ritorno: rit_giorno, rit_mese, rit_anno, rit_ore, rit_minuti, rit_secondi
 * -----------------------------------------------------------------------------------------------
 * Permettono di accedere in modo sicuro ai singoli campi della struttura data_ora.
 * Ogni funzione restituisce il valore del campo richiesto (es. giorno, mese, anno, ecc.),
 * oppure -1 se il puntatore fornito è NULL.
 *
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
 * Funzione di modifica: imposta_ore
 * ------------------------------------------
 * Scopo: Imposta il campo ore nella struttura data_ora.
 * Controlla che il valore sia nell’intervallo valido [0, 23].
 * Se il valore non è valido, non modifica nulla.
 */
void imposta_ore(data_ora d, int ore){
    if (ore < 0 || ore > 23)
        return;
    d->ore = ore;
}

/*
 * Funzione di modifica: imposta_giorno
 * ---------------------------------------------
 * Scopo: Aggiorna il giorno nella struttura data_ora, solo se è valido
 * rispetto a mese e anno. Evita inconsistenze nelle date (es. 30 febbraio).
 */
void imposta_giorno(data_ora d, int giorno){
    if (!giorno_valido(giorno, rit_mese(d), rit_anno(d)))
        return;
    d->giorno = giorno;
}

/*
 * Funzione: libera_data_ora
 * -------------------------
 * Scopo: Dealloca la memoria associata a una struttura data_ora.
 * Protegge da memory leak controllando che il puntatore non sia NULL.
 */

void libera_data_ora(data_ora d) {
    if (d != NULL) {
        free(d);
    }
}

