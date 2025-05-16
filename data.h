typedef struct data_e_ora *data_ora;

//Funzione per estrapolare la data e l'ora corrente restituisce la struttura data_ora
data_ora ottieni_data_ora();

// Calcola la differenza tra una data e un orario passato come argomento e la data e l'orario attuale, ritorna il risultato
data_ora calcolo_tempo_trascorso(data_ora inizio);