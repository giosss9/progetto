#ifndef DATA_H
#define DATA_H

typedef struct data_ora {
    int giorno;
    int mese;
    int anno;
    int ore;
    int minuti;
    int secondi;
} data_ora;


data_ora ottieni_data_ora();
data_ora calcolo_tempo_trascorso(data_ora inizio);

//Funzione che confronta due strutture data_ora passate come argomento
// Restituisce:
//  -1 se a < b
//   0 se a == b
//   1 se a > b
int confronta_data_ora(data_ora a, data_ora b);

#endif // DATA_H
