#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "attivita.h"
#include "data.h"

#define MAX_LINE 512
#define MAX 100

attivita inserisci_attivita_da_input() {
    char descrizione[MAX], corso[MAX];
    int giorno, mese, anno, ore, tempo_stimato, priorita;

    printf("Inserisci descrizione: ");
    fgets(descrizione, sizeof(descrizione), stdin);
    descrizione[strcspn(descrizione, "\n")] = 0;

    printf("Inserisci nome corso: ");
    fgets(corso, sizeof(corso), stdin);
    corso[strcspn(corso, "\n")] = 0;

    printf("Inserisci giorno di scadenza (1-31): ");
    scanf("%d", &giorno);
    printf("Inserisci mese di scadenza (1-12): ");
    scanf("%d", &mese);
    printf("Inserisci anno di scadenza (>0): ");
    scanf("%d", &anno);

    printf("Inserisci ora di scadenza (0-23): ");
    scanf("%d", &ore);

    printf("Inserisci tempo stimato (ore > 0): ");
    scanf("%d", &tempo_stimato);

    printf("Inserisci priorità (0=bassa, 1=media, 2=alta): ");
    scanf("%d", &priorita);

    // Stato iniziale: 0 = non iniziata
    int stato = 0;

    // Svuota il buffer di input
    while (getchar() != '\n');

    // Validazioni base
     if (tempo_stimato < 0 ||
			priorita < 0 || priorita > 2 ||
			 ore < 0 || ore > 23 ||
			 giorno<0 || giorno>31||
			 mese<1 || mese>12 ||
			 anno<2024 || anno>2030) {
        printf("Errore nei dati inseriti. Operazione annullata.\n");
        return NULL;
        }

    attivita a=crea_attivita(descrizione, corso, giorno, mese, anno,
                         tempo_stimato, priorita, ore);
    return a;
}

lista carica_attivita_da_file(const char *nome_file) {
    FILE *input = fopen(nome_file, "r");
    if (input == NULL) {
        printf("Impossibile aprire il file %s\n", nome_file);
        return NULL;
    }

    char riga[MAX_LINE];
    int riga_num = 0;
    lista l = nuova_lista();

    while (fgets(riga, MAX_LINE, input) != NULL) {
        riga_num++;

        char descrizione[MAX], corso[MAX];
        int giorno, mese, anno, tempo_stimato, priorita, ore;

        riga[strcspn(riga, "\n")] = 0;

        if (sscanf(riga, "%99[^;];%99[^;];%d;%d;%d;%d;%d;%d;",
                   descrizione, corso,
                   &giorno, &mese, &anno, &ore,
                   &tempo_stimato, &priorita) != 8) {
            fprintf(stderr, "Riga %d: formato riga non valido: %s\n", riga_num, riga);
            continue;
                   }

        if (tempo_stimato < 0 ||
			priorita < 0 || priorita > 2 ||
			 ore < 0 || ore > 23 ||
			 giorno<0 || giorno>31||
			 mese<1 || mese>12 ||
			 anno<2024 || anno>2030) {

            fprintf(stderr, "Riga %d: dati non validi:\n", riga_num);
            fprintf(stderr, "  Giorno: %d (1-31)\n", giorno);
            fprintf(stderr, "  Mese: %d (1-12)\n", mese);
            fprintf(stderr, "  Ore: %d (0-24)\n", ore);
            fprintf(stderr, "  Anno: %d (>0)\n", anno);
            fprintf(stderr, "  Tempo stimato: %d (>0)\n", tempo_stimato);
            fprintf(stderr, "  Priorità: %d (0=bassa, 1=media, 2=alta)\n", priorita);

            continue;
        }

        attivita nuova = crea_attivita(descrizione, corso, giorno, mese, anno,
                                       tempo_stimato, priorita, ore);
        l = cons_lista(nuova, l);
    }

    fclose(input);
    return l;
}
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

        while (!lista_vuota(corrente)) {
            attivita a = prendi_primo(corrente);
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
                    if (trascorso == NULL) {
                        printf("Errore: tempo trascorso non calcolabile\n");
                        continue;
                    }
                    int ore_trascorse = rit_ore(trascorso) ;
                    int percentuale = (stimato > 0) ? (ore_trascorse * 100 / stimato) : 0;

                    if (ore_trascorse < 1.0) {
                        printf("Tempo trascorso: %d minuti\n", rit_minuti(trascorso) );
                    } else {
                        printf("Tempo trascorso: %d ore\n", rit_ore(trascorso));
                    }

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

            corrente = coda_lista(corrente);
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

    if(scelta==1){
        imposta_tempo_inizio(a);
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

    const char* categorie[] = {
        "📅 Settimana corrente:",
        "📅 Settimana prossima:",
        "📅 Settimane future:",
        "🕒 Attività scadute:"
    };

    for (int categoria = 0; categoria < 4; categoria++) {
        printf("\n%s\n", categorie[categoria]);

        lista corr = l;
        int trovata = 0;

        while (!lista_vuota(corr)) {
            attivita a = prendi_primo(corr);
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

            corr = coda_lista(corr);  // ✅ uso incapsulato
        }

        if (!trovata) {
            printf("Nessuna attività trovata.\n");
        }
    }

    printf("\n========================================\n");
}




