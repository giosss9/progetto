#include <stdio.h>
#include "lista.h"
#include "attivita.h"
#include "data.h"

// Verifica se l'attività è in ritardo, se lo è cambia lo stato
void controlla_ritardo(attivita a) {

    data_ora data_corrente = ottieni_data_ora();
    data_ora scadenza = rit_scadenza(a);
    int confronto = confronta_data_ora(data_corrente, scadenza);

    int stato=rit_stato(a);
    if (confronto > 0 && stato != 3) {
        imposta_stato(a,3);
    }
}

void mostra_progresso(lista l) {
    const char* intestazioni[] = {
        "\n🔹 Attività NON INIZIATE:",
        "\n🟡 Attività IN CORSO:",
        "\n✅ Attività COMPLETATE:",
        "\n⏰ Attività IN RITARDO:"
    };

    for (int stato_atteso = 0; stato_atteso <= 3; stato_atteso++) {
        printf("%s\n", intestazioni[stato_atteso]);

        lista corrente = l;
        int trovata = 0;

        while (corrente != NULL) {
            attivita a = corrente->valore;
            controlla_ritardo(a);
            int stato = rit_stato(a);

            if (stato == stato_atteso) {
                trovata = 1;

                int stimato = rit_tempo_stimato(a);
                printf("\n--- Attività: %s ---\n", rit_descrizione(a));
                printf("Tempo stimato: %d ore\n", stimato);

                if (stato == 1) { // Solo per "in corso"
                    data_ora inizio = rit_tempo_inizio(a);
                    data_ora trascorso = calcolo_tempo_trascorso(inizio);
                    int ore_trascorse = rit_ore(trascorso) + (rit_minuti(trascorso) / 60.0);
                    int percentuale = (stimato > 0) ? (ore_trascorse * 100 / stimato) : 0;

                    printf("Tempo trascorso: %d ore\n", ore_trascorse);

                    // Barra di progresso
                    printf("Progresso: [");
                    int barLength = 20;
                    int filled = percentuale * barLength / 100;
                    for (int i = 0; i < barLength; ++i) {
                        printf(i < filled ? "#" : "-");
                    }
                    printf("] %d%%\n", percentuale);

                     if (ore_trascorse >= stimato) {
                        printf("⚠ Stato: POSSIBILE RITARDO (tempo stimato superato)\n");
                    } else {
                        printf("⏳ Stato: IN CORSO\n");
                    }
                } else {
                    // Stato testuale per altri stati
                    switch (stato) {
                        case 0: printf("Stato: NON INIZIATA\n"); break;
                        case 2: printf("Stato: COMPLETATA\n"); break;
                        case 3: printf("Stato: IN RITARDO\n"); break;
                    }
                }
            }

            corrente = corrente->successivo;
        }

        if (!trovata) {
            printf("Nessuna attività trovata in questa categoria.\n");
        }
    }
}


//Funzione che riceve come argomento una struttura di tipo attivita e un intero che deve essere 1 o 2
//Se l'attività è completata o la scelta non è valida restituisce 1, se è in ritardo restituisce 2, altrimenti restituisce 0
int aggiorna_stato(attivita a, int scelta) {
	// Verifica se l'attività è in ritardo prima di tutto
    controlla_ritardo(a);

    // Verifica se l'attività è già completata
    if (rit_stato(a) == 2) {
        printf("Attenzione! Questa attività è già completata.\n");
        return 1;
    }

    // Controllo validità input
    if (scelta < 1 || scelta > 2) {
        printf("Input non valido. Inserisci 1 (in corso) o 2 (completata).\n");
        return 1;
    }

    if (rit_stato(a) == 3 && scelta==1){
        printf("L'attività è già in ritardo! Affrettati a completarla.\n");
        return 2;
    }
    // Aggiorna lo stato
    imposta_stato(a, scelta);
    printf("Stato aggiornato correttamente.\n");
    return 0;
}

void genera_report_settimanale(lista l) {
    data_ora oggi = ottieni_data_ora();
    int settimana_corrente = numero_settimana(oggi);
    int anno_corrente = rit_anno(oggi);

    printf("===== 📆 Report Settimanale =====\n");

    lista corr = l;

    // Stampa intestazioni e cicla per ogni categoria
    const char* categorie[] = {
        "📅 Settimana corrente:",
        "📅 Settimana prossima:",
        "📅 Settimane future:",
        "🕒 Attività scadute:"
    };

    for (int categoria = 0; categoria < 4; categoria++) {
        printf("\n%s\n", categorie[categoria]);
        corr = l;  // Reset puntatore

        int trovata = 0;
        while (corr != NULL) {
            attivita a = corr->valore;
            data_ora scad = rit_scadenza(a);

            int settimana_attivita = numero_settimana(scad);
            int anno_attivita = rit_anno(scad);
            int cmp = confronta_data_ora(scad, oggi);

            int stampa = 0;
            if (categoria == 0 && settimana_attivita == settimana_corrente && anno_attivita == anno_corrente && cmp >= 0)
                stampa = 1;
            else if (categoria == 1 && settimana_attivita == settimana_corrente + 1 && anno_attivita == anno_corrente)
                stampa = 1;
            else if (categoria == 2 && settimana_attivita > settimana_corrente + 1 && anno_attivita >= anno_corrente)
                stampa = 1;
            else if (categoria == 3 && cmp < 0)
                stampa = 1;

            if (stampa) {
                stampa_attivita(a);
                trovata = 1;
            }

            corr = corr->successivo;
        }

        if (!trovata) {
            printf("Nessuna attività trovata.\n");
        }
    }

    printf("\n========================================\n");
}




