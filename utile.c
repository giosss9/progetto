#include <stdio.h>
#include "lista.h"
#include "attivita.h"
#include "data.h"
void mostra_progresso(lista l) {
    lista corrente = l;

    while (corrente != NULL) {
        attivita a = corrente->valore;

        int stimato = rit_tempo_stimato(a); // in ore
        data_ora creazione = rit_tempo_creazione(a);
        data_ora scadenza = rit_scadenza(a);
        data_ora trascorso = calcolo_tempo_trascorso(creazione);

        int ore_trascorse = trascorso.ore + (trascorso.minuti / 60.0);
        int percentuale = (stimato > 0) ? (ore_trascorse * 100 / stimato) : 0;

        // Stampa intestazione attività
        printf("\n--- Attività: %s ---\n", rit_descrizione(a));
        printf("Tempo stimato: %d ore\n", stimato);
        printf("Tempo trascorso: %d ore\n", ore_trascorse);

        // Stampa barra di progresso
        printf("Progresso: [");
        int barLength = 20;
        int filled = percentuale * barLength / 100;
        for (int i = 0; i < barLength; ++i) {
            if (i < filled)
                printf("#");
            else
                printf("-");
        }
        printf("] %d%%\n", percentuale);

        // Controllo stato
        if (rit_stato(a) == 2) {
            printf("Stato: COMPLETATA \n");
        } else {
            // Verifica ritardo rispetto a scadenza
            int v=confronta_data_ora(rit_scadenza(a),ottieni_data_ora());

            if (v > 0) {
                printf("Stato: IN RITARDO (scadenza superata)\n");
            } else if (ore_trascorse >= stimato) {
                printf("Stato: POSSIBILE RITARDO (tempo stimato superato)\n");
            } else {
                printf("Stato: IN CORSO\n");
            }
        }

        corrente = corrente->successivo;
    }
}
