#include <stdio.h>
#include "attivita.h"

#define MAX 100
//Definizione tipo data che servirà per la scadenza dell'attività
typedef struct data{
    int giorno;
    int mese;
    int anno;
}

typedef struct attivita{
    char descrizione[MAX];
    char corso[MAX];
    data scadenza;
    int tempo_stimato;
    int priorita;        //0=bassa 1=media 2=alta
    int stato;          //0=non iniziata 1=in corso 2=completata
}

