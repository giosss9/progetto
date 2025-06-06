#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "data.h"

struct data_ora {
    int giorno;
    int mese;
    int anno;
    int ore;
    int minuti;
    int secondi;
};

// Crea e restituisce una nuova struttura data_ora
data_ora crea_data_ora(int giorno, int mese, int anno, int ore, int minuti, int secondi) {
    data_ora nuova = malloc(sizeof(struct data_ora));
    if (nuova == NULL) {
        fprintf(stderr, "Errore allocazione memoria data_ora\n");
        exit(EXIT_FAILURE);
    }

    nuova->giorno = giorno;
    nuova->mese = mese;
    nuova->anno = anno;
    nuova->ore = ore;
    nuova->minuti = minuti;
    nuova->secondi = secondi;

    return nuova;
}

// Ottiene data e ora correnti
data_ora ottieni_data_ora() {
    time_t tempo_grezzo = time(NULL);
    struct tm *tempo_locale = localtime(&tempo_grezzo);

    data_ora data_corrente = malloc(sizeof(struct data_ora));
    if (data_corrente == NULL) {
        fprintf(stderr, "Errore nell'allocazione della memoria\n");
        exit(EXIT_FAILURE);
    }

    data_corrente->giorno = tempo_locale->tm_mday;
    data_corrente->mese = tempo_locale->tm_mon + 1;
    data_corrente->anno = tempo_locale->tm_year + 1900;
    data_corrente->ore = tempo_locale->tm_hour;
    data_corrente->minuti = tempo_locale->tm_min;
    data_corrente->secondi = tempo_locale->tm_sec;

    return data_corrente;
}

// Calcola il tempo trascorso tra la data passata e quella attuale (giorni minuti e ore)
data_ora calcolo_tempo_trascorso(data_ora inizio) {
    if (inizio == NULL) return NULL;

    struct tm t_inizio = {
        .tm_year = inizio->anno - 1900,
        .tm_mon  = inizio->mese - 1,
        .tm_mday = inizio->giorno,
        .tm_hour = inizio->ore,
        .tm_min  = inizio->minuti,
        .tm_sec  = inizio->secondi
    };

    time_t tempo_inizio = mktime(&t_inizio);
    if (tempo_inizio == -1) {
        fprintf(stderr, "Errore conversione mktime (inizio)\n");
        return NULL;
    }

    data_ora adesso = ottieni_data_ora();
    if (adesso == NULL) return NULL;

    struct tm t_attuale = {
        .tm_year = adesso->anno - 1900,
        .tm_mon  = adesso->mese - 1,
        .tm_mday = adesso->giorno,
        .tm_hour = adesso->ore,
        .tm_min  = adesso->minuti,
        .tm_sec  = adesso->secondi
    };

    time_t tempo_adesso = mktime(&t_attuale);
    if (tempo_adesso == -1) {
        fprintf(stderr, "Errore conversione mktime (attuale)\n");
        free(adesso);
        return NULL;
    }

    double diff_sec = difftime(tempo_adesso, tempo_inizio);
    free(adesso);

    data_ora risultato = malloc(sizeof(struct data_ora));
    if (risultato == NULL) {
        fprintf(stderr, "Errore allocazione memoria per il risultato\n");
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

// Confronta due date
//-1 → la prima data (a) è prima della seconda (b)
//0 → le due date sono uguali
//1 → la prima data (a) è dopo la seconda (b)
// 2 se
int confronta_data_ora(data_ora a, data_ora b) {
  	if (a == NULL || b == NULL) {
        fprintf(stderr, "Errore: data_ora non valida (NULL)\n");
        exit(EXIT_FAILURE);
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


// Restituisce il numero della settimana dell'anno
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

    mktime(&tm_date); // Normalizzazione

    char buffer[3];
    strftime(buffer, sizeof(buffer), "%W", &tm_date); // %W: settimana (lunedì come inizio)
    return atoi(buffer);
}


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

void libera_data_ora(data_ora d) {
    if (d != NULL) {
        free(d);
    }
}

