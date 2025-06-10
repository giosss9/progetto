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
    if (anno < 2025 || anno > 2030) {
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
	// Aggiungi alla fine della funzione dati_validi
	data_ora attivita_data = crea_data_ora(giorno, mese, anno, ore, 0, 0);
	data_ora ora_corrente = ottieni_data_ora();

	if (attivita_data == NULL || ora_corrente == NULL) {
    	return 0; // errore nella creazione delle date
	}

	// Confronta la data: se è nel passato, non è valida
	if (confronta_data_ora(attivita_data, ora_corrente) < 0) {
    	libera_data_ora(attivita_data);
    	libera_data_ora(ora_corrente);
    	return 0;
	}

	libera_data_ora(attivita_data);
	libera_data_ora(ora_corrente);
    // Se tutte le condizioni sono valide
    return 1;
}


attivita inserisci_attivita_da_input(int *ultimo_id) {
	if(ultimo_id==NULL){
		printf("Attenzione ultimo_id non punta ad una variabile");
		return NULL;
	}

    char descrizione[MAX], corso[MAX];
    char buffer[100];
    int giorno, mese, anno, ore, tempo_stimato, priorita;
    int id = *ultimo_id;

    printf("Inserisci descrizione: ");
    fgets(descrizione, sizeof(descrizione), stdin);
    descrizione[strcspn(descrizione, "\n")] = 0;

    printf("Inserisci nome corso: ");
    fgets(corso, sizeof(corso), stdin);
    corso[strcspn(corso, "\n")] = 0;

    printf("Inserisci giorno di scadenza (1-31): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &giorno) != 1) {
        printf("Input non valido per il giorno.\n");
        return NULLATTIVITA;
    }

    printf("Inserisci mese di scadenza (1-12): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &mese) != 1) {
        printf("Input non valido per il mese.\n");
        return NULLATTIVITA;
    }

    printf("Inserisci anno di scadenza (2024-2030): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &anno) != 1) {
        printf("Input non valido per l'anno.\n");
        return NULLATTIVITA;
    }

    printf("Inserisci ora di scadenza (0-23): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &ore) != 1) {
        printf("Input non valido per l'ora.\n");
        return NULLATTIVITA;
    }

    printf("Inserisci tempo stimato in ore (>0): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &tempo_stimato) != 1) {
        printf("Input non valido per il tempo stimato.\n");
        return NULLATTIVITA;
    }

    printf("Inserisci priorità (0=bassa, 1=media, 2=alta): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &priorita) != 1) {
        printf("Input non valido per la priorità.\n");
        return NULLATTIVITA;
    }

    if (!dati_validi(giorno, mese, anno, ore, tempo_stimato, priorita)) {
        printf("Errore nei dati inseriti. Operazione annullata.\n");
        return NULLATTIVITA;
    }

    attivita a = crea_attivita(descrizione, corso, giorno, mese, anno,
                                tempo_stimato, priorita, ore, id);

    *ultimo_id = id + 1;
    return a;
}

lista carica_attivita_da_file(const char *nome_file, int *ultimo_id) {
    if (ultimo_id == NULL) {
        printf("Attenzione ultimo_id non punta ad una variabile\n");
        return NULL;
    }

    FILE *input = fopen(nome_file, "r");
    if (input == NULL) {
        printf("Impossibile aprire il file %s\n", nome_file);
        return NULL;
    }

    char riga[MAX_LINE];
    int riga_num = 0;
    lista l = nuova_lista();
    int id = 0;

    while (fgets(riga, MAX_LINE, input) != NULL) {
        riga_num++;

        // Rimuovi newline
        riga[strcspn(riga, "\r\n")] = 0;

        // Ignora righe vuote o che iniziano con '#'
        if (riga[0] == '\0' || riga[0] == '#') {
            continue;
        }

        char descrizione[MAX], corso[MAX];
        int giorno, mese, anno, tempo_stimato, priorita, ore;

        if (sscanf(riga, "%99[^;];%99[^;];%d;%d;%d;%d;%d;%d",
                   descrizione, corso,
                   &giorno, &mese, &anno, &ore,
                   &tempo_stimato, &priorita) != 8) {
            printf("Riga %d: formato riga non valido: %s\n", riga_num, riga);
            continue;
        }

        if (dati_validi(giorno, mese, anno, ore, tempo_stimato, priorita)) {
            attivita nuova = crea_attivita(descrizione, corso, giorno, mese, anno,
                                           tempo_stimato, priorita, ore, id);
            l = cons_lista(nuova, l);
            id++;
        } else {
            printf("Errore nei dati inseriti nella riga %d. Attivita non inserita.\n", riga_num);
        }
    }

    fclose(input);
    *ultimo_id = id;
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
//Calcola il progresso di un attivita, ritorna la percentuale
void calcolo_progresso(attivita a, data_ora trascorso){
	int stimato = rit_tempo_stimato(a);

    if (trascorso == NULL) {
    	printf("Errore: tempo trascorso non calcolabile\n");
        return;
    }

	int ore_trascorse=rit_ore(trascorso);

	//Calcolo percentuale
    int minuti_trascorsi = ore_trascorse * 60 + rit_minuti(trascorso);
	int minuti_stimati = stimato * 60;
	int percentuale = (minuti_stimati > 0) ? (minuti_trascorsi * 100 / minuti_stimati) : 0;

	if (minuti_trascorsi < 60) {
    	printf("Tempo trascorso: %d minuti\n", minuti_trascorsi);
	} else {
   		printf("Tempo trascorso: %.1f ore\n", minuti_trascorsi / 60.0);
	}

	if (ore_trascorse >= stimato) {
		printf("Stato: POSSIBILE RITARDO (tempo stimato superato)\n");
	} else {
		printf("Stato: IN CORSO\n");
    }

	if (percentuale > 100) {
    	percentuale = 100;
	}

	if(percentuale==-1)
		printf("Errore nel calcolo del progresso");

		// Barra di progresso
 		printf("Progresso: [");
        int lunghezza_barra = 20;
		int riempimento = percentuale * lunghezza_barra / 100;
        for (int i = 0; i < lunghezza_barra; ++i) {
            printf(i < riempimento ? "#" : "-");
        }

   	printf("] %d%%\n", percentuale);
	//return percentuale;
}

void mostra_progresso(lista l) {
    const char* intestazioni[] = {
        "\nAttività NON INIZIATE:",
        "\nAttività IN CORSO:",
        "\nAttività COMPLETATE:",
        "\nAttività IN RITARDO:"
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

                   calcolo_progresso(a,trascorso);

                } else {
					data_ora scad=rit_scadenza(a);
					printf("Data di scadenza:%d/%d/%d\n",rit_giorno(scad),rit_mese(scad),rit_anno(scad));
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
        return 2;
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

            corr = coda_lista(corr);
        }

        if (!trovata) {
            printf("Nessuna attività trovata.\n");
        }
    }

    printf("\n========================================\n");
}

attivita chiedi_attivita_per_id(lista l) {
	stampa_lista(l);
    int id;
    attivita a;

    while (1) {
        printf("Inserisci l'id dell'attività: \n");
        scanf("%d", &id);

        a = cerca_attivita_per_id(l, id);
        if (a == NULLATTIVITA) {
            printf("Attività non trovata.\n");
        } else {
            return a;
        }
    }
}

void menu(lista l, int *ultimo_id) {
	if(ultimo_id==NULL){
		printf("Attenzione ultimo_id non punta ad una variabile");
		return;
	}

    int scelta;
    do {
        printf("\n=================MENU===================\n");
        printf("1. Report settimanale\n");
        printf("2. Mostra stato avanzamento\n");
        printf("3. Aggiorna stato\n");
        printf("4. Inserisci nuova attivita\n");
		printf("5. Rimuovi un attivita\n");
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
				int nuovo, rit;
				attivita a;
				int v = 1;

				while (v == 1) {
    				a = chiedi_attivita_per_id(l);

    				printf("Inserisci il nuovo stato dell'attività [1 = In Corso, 2 = Completata]: \n");
    				scanf("%d", &nuovo);

    				rit = aggiorna_stato(a, nuovo);

    				if (rit == 0) {
        				v = 0; // tutto ok
    				} else if (rit == 1) {
        				printf("Riprova con un valore valido (1 o 2).\n");
    				} else if (rit == 2) {
        				printf("Non è possibile aggiornare questa attività.\n");
        			v = 0;
    				}
				}
				break;
            case 4:
                attivita nuova = inserisci_attivita_da_input(ultimo_id);
                if (nuova != NULLATTIVITA) {
                    l = cons_lista(nuova, l);
                }
				break;
			case 5:
				printf("Quale attivita desideri rimuovere?\n");
				attivita a_da_rimuovere;
				a_da_rimuovere = chiedi_attivita_per_id(l);
				int id = rit_id(a_da_rimuovere);
				l = rimuovi_attivita_per_id(l,id);
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




