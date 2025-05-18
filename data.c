#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "data.h"


//Funzione per estrapolare la data e l'ora corrente restituisce la struttura data_ora definita sopra
data_ora ottieni_data_ora() {
    time_t ora_corrente = time(NULL); //dà il tempo attuale in secondi dal 1/1/1970 (formato "timestamp")
    struct tm *tempo_locale = localtime(&ora_corrente); //converte quel valore grezzo in una struttura tm leggibile

    //Copia i valori dalla struttura tm alla struttura data_ora, convertendo mese e anno nel formato corretto.
    data_ora data_ora;
    data_ora->giorno = tempo_locale->tm_mday;
    data_ora->mese = tempo_locale->tm_mon + 1;  // tm_mon va da 0 a 11
    data_ora->anno = tempo_locale->tm_year + 1900;  // tm_year è anni dal 1900
    data_ora->ore = tempo_locale->tm_hour;
    data_ora->minuti = tempo_locale->tm_min;
    data_ora->secondi = tempo_locale->tm_sec;

    return data_ora;
}
// Calcola la differenza tra la data/ora passata e quella attuale
data_ora calcolo_tempo_trascorso(data_ora inizio) {
    // Crea struttura tm per la data/ora di inizio
    struct tm t_inizio = {0};
    t_inizio.tm_year = inizio->anno - 1900;
    t_inizio.tm_mon  = inizio->mese - 1;
    t_inizio.tm_mday = inizio->giorno;
    t_inizio.tm_hour = inizio->ore;
    t_inizio.tm_min  = inizio->minuti;
    t_inizio.tm_sec  = inizio->secondi;

    time_t tempo_inizio = mktime(&t_inizio);

    // Ottiene la data/ora attuale
    data_ora adesso = ottieni_data_ora();
    struct tm t_attuale = {0};
    t_attuale.tm_year = adesso->anno - 1900;
    t_attuale.tm_mon  = adesso->mese - 1;
    t_attuale.tm_mday = adesso->giorno;
    t_attuale.tm_hour = adesso->ore;
    t_attuale.tm_min  = adesso->minuti;
    t_attuale.tm_sec  = adesso->secondi;

    time_t tempo_adesso = mktime(&t_attuale);

    if (tempo_inizio == -1 || tempo_adesso == -1) {
        fprintf(stderr, "Errore nella conversione del tempo\n");
        exit(EXIT_FAILURE);
    }

    double diff_sec = difftime(tempo_adesso, tempo_inizio);

    // Alloca memoria per il risultato
    data_ora risultato = malloc(sizeof(data_ora));
    if (risultato == NULL) {
        fprintf(stderr, "Errore allocazione memoria\n");
        exit(EXIT_FAILURE);
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
