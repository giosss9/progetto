#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "attivita.h"
#include "data.h"

#define MAX_LINE 512
#define MAX 100

// Restituisce 1 se il giorno è valido per il mese e l'anno forniti, altrimenti 0
int giorno_valido(int giorno, int mese, int anno) {
    // Giorni standard per ogni mese (indice 0 = gennaio, 1 = febbraio, ..., 11 = dicembre)
    int giorni_per_mese[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Verifica se l'anno è bisestile
    int bisestile = (anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0);

    // Se febbraio e anno bisestile, imposta 29 giorni
    if (mese == 2 && bisestile) {
        giorni_per_mese[1] = 29;
    }

    // Verifica che il mese sia valido (1-12)
    if (mese < 1 || mese > 12) {
        return 0;
    }

    // Controlla che il giorno rientri nel numero massimo per quel mese
    if (giorno >= 1 && giorno <= giorni_per_mese[mese - 1]) {
        return 1;
    } else {
        return 0;
    }
}

// Verifica se tutti i dati relativi a un'attività sono validi
int dati_validi(int giorno, int mese, int anno, int ore, int tempo_stimato, int priorita) {
    // Controlla se il giorno è valido per il mese e l'anno (
    if (!giorno_valido(giorno, mese, anno)) {
        return 0;
    }

    if (mese < 1 || mese > 12) {
        return 0;
    }
    if (anno < 2024 || anno > 2030) {
        return 0;
    }
    if (ore < 0 || ore > 23) {
        return 0;
    }
    if (tempo_stimato <= 0) {
        return 0;
    }
    if (priorita < 0 || priorita > 2) {
        return 0;
    }

    // Se tutte le condizioni sono valide
    return 1;
}


attivita inserisci_attivita_da_input(int *ultimo_id) {
    char descrizione[MAX], corso[MAX];
    int giorno, mese, anno, ore, tempo_stimato, priorita;
	int id=*ultimo_id;

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
     if (dati_validi) {
        printf("Errore nei dati inseriti. Operazione annullata.\n");
        return NULL;
     }

    attivita a=crea_attivita(descrizione, corso, giorno, mese, anno,
                         tempo_stimato, priorita, ore, id);
	*ultimo_id=id+1;
    return a;
}

lista carica_attivita_da_file(const char *nome_file, int *ultimo_id) {
    FILE *input = fopen(nome_file, "r");
    if (input == NULL) {
        printf("Impossibile aprire il file %s\n", nome_file);
        return NULL;
    }

    char riga[MAX_LINE];
    int riga_num = 0;
    lista l = nuova_lista();
	int id=0;
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
                                       tempo_stimato, priorita, ore, id);

        l = cons_lista(nuova, l);
		id++;
    }

    fclose(input);
	id--; //perchè fa un'addizione in più all'interno del while
	*ultimo_id=id;
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

    printf("=========📆 Report Settimanale==========\n");

    const char* categorie[] = {
        "Settimana corrente:",
        "Settimana prossima:",
        "Settimane future:",
        "Attività scadute:"
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


void menu(lista l, int *ultimo_id) {
    int scelta;
    do {
        printf("\n=================MENU===================\n");
        printf("1. Report settimanale\n");
        printf("2. Mostra stato avanzamento\n");
        printf("3. Aggiorna stato\n");
        printf("4. Inserisci nuova attivita\n");
        printf("0. Esci\n");
        printf("========================================\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        getchar(); // per consumare il newline

        switch (scelta) {
            case 1:
                genera_report_settimanale(l);
                break;
            case 2:
                mostra_progresso(l);
                break;
            case 3:
				stampa_lista(l);
				int id;
				int nuovo,v=1, k=1;
				attivita a;
				while(v==1){
					while(k==1){
						printf("Inserisci l'id dell'attività che vuoi cambiare: \n");
						scanf("%d", &id);
						a=cerca_attivita_per_id(l,id);
						if(a==NULLITEM)
							printf("Attività non trovata.\n");
						else
							k=0;
					}
					printf("Inserisci il nuovo stato dell'attivita [1=in corso, 2=completata]: \n");
					scanf("%d",&nuovo);
					v=aggiorna_stato(a,nuovo);
				}

				break;
            case 4:
                attivita nuova = inserisci_attivita_da_input(ultimo_id);
                if (nuova != NULL) {
                    l = cons_lista(nuova, l);
                }
				break;
			case 0:
                libera_lista(l);
                printf("Uscita in corso...\n");
                break;
            default:
                printf("Scelta non valida.\n");
        }
    } while (scelta != 0);
}




