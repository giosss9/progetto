#ifndef DATA_H
#define DATA_H

typedef struct data_ora *data_ora;

data_ora crea_data_ora(int giorno, int mese, int anno, int ore, int minuti, int secondi);

data_ora ottieni_data_ora();
data_ora calcolo_tempo_trascorso(data_ora inizio);

//Funzione che confronta due strutture data_ora passate come argomento
// Restituisce:
//  -1 se a < b
//   0 se a == b
//   1 se a > b
int confronta_data_ora(data_ora a, data_ora b);

// Restituisce il numero di settimana dell'anno di una data_ora
int numero_settimana(data_ora d);

int rit_giorno(data_ora d);
int rit_mese(data_ora d);
int rit_anno(data_ora d);
int rit_ore(data_ora d);
int rit_minuti(data_ora d);
int rit_secondi(data_ora d);

void imposta_ore(data_ora d, int ore);

void libera_data_ora(data_ora d);
#endif // DATA_H
