#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "data.h"

// Funzione per ottenere data e ora corrente
data_ora ottieni_data_ora() {
    time_t ora_corrente = time(NULL);
    struct tm *tempo_locale = localtime(&ora_corrente);

    data_ora d;
    d.giorno = tempo_locale->tm_mday;
    d.mese = tempo_locale->tm_mon + 1;
    d.anno = tempo_locale->tm_year + 1900;
    d.ore = tempo_locale->tm_hour;
    d.minuti = tempo_locale->tm_min;
    d.secondi = tempo_locale->tm_sec;

    return d;
}

// Calcola il tempo trascorso tra la data passata e quella attuale
data_ora calcolo_tempo_trascorso(data_ora inizio) {
    struct tm t_inizio = {0};
    t_inizio.tm_year = inizio.anno - 1900;
    t_inizio.tm_mon = inizio.mese - 1;
    t_inizio.tm_mday = inizio.giorno;
    t_inizio.tm_hour = inizio.ore;
    t_inizio.tm_min = inizio.minuti;
    t_inizio.tm_sec = inizio.secondi;

    time_t tempo_inizio = mktime(&t_inizio);

    data_ora adesso = ottieni_data_ora();
    struct tm t_attuale = {0};
    t_attuale.tm_year = adesso.anno - 1900;
    t_attuale.tm_mon = adesso.mese - 1;
    t_attuale.tm_mday = adesso.giorno;
    t_attuale.tm_hour = adesso.ore;
    t_attuale.tm_min = adesso.minuti;
    t_attuale.tm_sec = adesso.secondi;

    time_t tempo_adesso = mktime(&t_attuale);

    if (tempo_inizio == -1 || tempo_adesso == -1) {
        fprintf(stderr, "Errore nella conversione del tempo\n");
        exit(EXIT_FAILURE);
    }

    double diff_sec = difftime(tempo_adesso, tempo_inizio);

    data_ora risultato = {0};
    risultato.giorno = (int)(diff_sec / (60 * 60 * 24));
    diff_sec -= risultato.giorno * 60 * 60 * 24;

    risultato.ore = (int)(diff_sec / (60 * 60));
    diff_sec -= risultato.ore * 60 * 60;

    risultato.minuti = (int)(diff_sec / 60);
    risultato.secondi = (int)(diff_sec - risultato.minuti * 60);

    risultato.anno = 0;
    risultato.mese = 0;

    return risultato;
}
