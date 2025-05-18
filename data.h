#ifndef DATA_H
#define DATA_H

typedef struct data_e_ora {
    int giorno;
    int mese;
    int anno;
    int ore;
    int minuti;
    int secondi;
} data_e_ora;

typedef data_e_ora* data_ora;

data_ora ottieni_data_ora();
data_ora calcolo_tempo_trascorso(data_ora inizio);

#endif // DATA_H
