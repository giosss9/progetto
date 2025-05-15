#include <stdio.h>
#include <time.h>
#include "data.h"


struct data_e_ora{
    int giorno;
    int mese;
    int anno;
    int ore;
    int minuti;
    int secondi;
} ;

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