//Implementazione del modulo: file utile.c

#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "attivita.h"
#include "data.h"

#define MAX_LINE 512
#define MAX 100


/*
 * Funzione: giorno_valido
 * -----------------------
 * Verifica se una combinazione di giorno, mese e anno rappresenta una data valida
 * secondo il calendario gregoriano.
 *
 * La funzione considera correttamente gli anni bisestili (ad esempio, il 29 febbraio è
 * valido solo se l'anno è bisestile).
 *
 * Parametri:
 *   giorno - Giorno del mese (1-31)
 *   mese   - Mese dell'anno (1-12)
 *   anno   - Anno completo (es. 2025)
 *
 * Ritorna:
 *   1 se la data è valida, 0 altrimenti.
 */

int giorno_valido(int giorno, int mese, int anno) {
    // Giorni standard per ogni mese (indice 0 = gennaio, 1 = febbraio, ..., 11 = dicembre)
    int giorni_per_mese[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Verifica se l'anno è bisestile
    int bisestile = (anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0);

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

/*
 * Funzione: dati_validi
 * -----------------------
 * Verifica la validità complessiva dei dati forniti per la creazione di un'attività,
 * controllando che giorno, mese, anno, ora, tempo stimato e priorità rispettino i limiti previsti.
 *
 * Inoltre, verifica che la data e l'ora specificate non siano nel passato rispetto all'orario corrente.
 *
 * Parametri:
 *   giorno         - Giorno del mese (1-31)
 *   mese           - Mese dell'anno (1-12)
 *   anno           - Anno (accettato tra 2025 e 2030)
 *   ore            - Ora del giorno (0-23)
 *   tempo_stimato  - Tempo stimato per completare l'attività (in ore, > 0)
 *   priorita       - Livello di priorità (0 = bassa, 1 = media, 2 = alta)
 *
 * Ritorna:
 *   1 se tutti i dati sono validi e coerenti, 0 altrimenti.
 */

int dati_validi(int giorno, int mese, int anno, int ore, int tempo_stimato, int priorita) {
    // Controlla se il giorno è valido per il mese e l'anno (
    if (!giorno_valido(giorno, mese, anno)) {
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

    return 1;
}

/*
 * Funzione: inserisci_attivita_da_input
 * -------------------------------------
 * Richiede all'utente i dati necessari per creare una nuova attività,
 * effettuando controlli di validità su tutti gli input ricevuti.
 *
 * I dati richiesti includono descrizione, corso, data e ora di scadenza,
 * tempo stimato e priorità. In caso di input errati o dati non validi,
 * la funzione annulla l'operazione e restituisce NULLATTIVITA.
 *
 * Parametri:
 *   ultimo_id - Puntatore all'intero contenente l'ultimo ID usato.
 *               Se valido, viene incrementato e assegnato alla nuova attività.
 *
 * Ritorna:
 *   Una nuova struttura 'attivita' con i dati inseriti, oppure NULLATTIVITA in caso di errore.
 *
 * Nota:
 *   L'input viene richiesto da tastiera tramite stdin.
 */

attivita inserisci_attivita_da_input(int *ultimo_id) {
	if(ultimo_id==NULL){
		printf("Attenzione ultimo_id non punta ad una variabile");
		return NULLATTIVITA;
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

    printf("Inserisci anno di scadenza (2025-2030): ");
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

/*
 * Funzione: carica_attivita_da_file
 * --------------------------------
 * Legge da file una lista di attività, creando una nuova lista contenente tutte
 * le attività valide lette riga per riga. Ogni riga deve contenere i dati dell'attività
 * separati da punto e virgola nel formato: descrizione;corso;giorno;mese;anno;ora;tempo_stimato;priorità.
 *
 * Vengono ignorate righe vuote o che iniziano con il carattere '#'.
 * Per ogni riga, la funzione verifica la validità dei dati e, in caso di errore,
 * segnala il problema e scarta l'attività.
 *
 * Parametri:
 *   nome_file - Stringa contenente il nome del file da cui leggere le attività.
 *   ultimo_id - Puntatore a un intero che conterrà l'ID da assegnare alla prossima attività.
 *               Deve puntare a una variabile valida; in caso contrario la funzione restituisce NULL.
 *
 * Ritorna:
 *   Una lista contenente tutte le attività lette e valide, oppure NULL in caso di errore
 *   (file non aperto o parametro ultimo_id non valido).
 *
 * Nota:
 *   L'ID delle attività viene assegnato progressivamente a partire da zero,
 *   aggiornando il valore puntato da ultimo_id con l'ID successivo disponibile.
 */

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

/*
 * Funzione: controlla_ritardo
 * ---------------------------
 * Controlla se un'attività è in ritardo rispetto alla data e ora correnti.
 *
 * La funzione confronta la data e ora di scadenza dell'attività con quella corrente.
 * Se la scadenza è passata e lo stato dell'attività non è già impostato a "in ritardo" (3),
 * allora aggiorna lo stato dell'attività a 3 per indicare il ritardo.
 *
 * Parametri:
 *   a - Struttura 'attivita' da controllare. Se NULLATTIVITA, la funzione termina senza operazioni.
 *
 * Ritorna:
 *   Nulla (funzione void). Aggiorna internamente lo stato dell'attività se necessario.
 *
 * Nota:
 *   Utilizza funzioni di sistema per ottenere la data e ora correnti e per confrontare le date.
 */

void controlla_ritardo(attivita a) {
    if(a==NULLATTIVITA) {
      return;
    }
    data_ora data_corrente = ottieni_data_ora();
    data_ora scadenza = rit_scadenza(a);
    int confronto = confronta_data_ora(data_corrente, scadenza);

    int stato=rit_stato(a);
    if (confronto > 0 && stato != 3) {
        imposta_stato(a,3);
    }
}

/*
 * Funzione: calcolo_progresso
 * ---------------------------
 * Calcola e stampa il progresso di un'attività in base al tempo trascorso rispetto al tempo stimato.
 *
 * La funzione riceve un'attività e un intervallo di tempo trascorso, quindi:
 * - Calcola la percentuale di tempo utilizzato rispetto al tempo stimato.
 * - Stampa il tempo trascorso in minuti o ore.
 * - Indica lo stato dell'attività ("IN CORSO" o "POSSIBILE RITARDO" se il tempo stimato è superato).
 * - Mostra una barra di progresso visuale che rappresenta la percentuale completata.
 *
 * Parametri:
 *   a        - Struttura 'attivita' da cui estrarre il tempo stimato. Se NULLATTIVITA, stampa errore e termina.
 *   trascorso - Struttura 'data_ora' che rappresenta il tempo trascorso. Se NULL, stampa errore e termina.
 *
 * Ritorna:
 *   Void. La funzione stampa direttamente i risultati su stdout.
 *
 * Nota:
 *   La percentuale massima è limitata al 100%.
 *   La barra di progresso ha una lunghezza fissa di 20 caratteri.
 */

void calcolo_progresso(attivita a, data_ora trascorso){
	int stimato = rit_tempo_stimato(a);

    if (a == NULLATTIVITA) {
      printf("Attivita non trovata\n");
      return;
    }

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

/*
 * Funzione: mostra_progresso
 * --------------------------
 * Visualizza le attività suddivise per stato, stampando informazioni rilevanti
 * per ciascuna categoria: non iniziate, in corso, completate e in ritardo.
 *
 * Per ogni attività:
 * - Controlla se è in ritardo aggiornando lo stato se necessario.
 * - Stampa la descrizione, il tempo stimato e altre informazioni specifiche in base allo stato.
 * - Per le attività in corso, calcola e mostra il progresso basato sul tempo trascorso.
 *
 * Parametri:
 *   l - Lista di attività da visualizzare. Se vuota, stampa un messaggio e termina.
 *
 * Ritorna:
 *   Void. La funzione stampa direttamente le informazioni su stdout.
 *
 * Nota:
 *   Le attività sono raggruppate e visualizzate in ordine di stato (0-3).
 *   Usa funzioni di supporto per ottenere informazioni e aggiornare lo stato.
 */

void mostra_progresso(lista l) {
    if(lista_vuota(l)){
		printf("La lista delle attività è vuota");
		return;
	}
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

/*
 * Funzione: aggiorna_stato
 * ------------------------
 * Aggiorna lo stato di un'attività in base alla scelta fornita, eseguendo
 * controlli di validità e aggiornando anche lo stato in ritardo se necessario.
 *
 * La funzione verifica:
 * - Se l'attività è valida (diversa da NULLATTIVITA).
 * - Se l'attività non è già completata (stato 2).
 * - Se la scelta è 1 (in corso) o 2 (completata), altrimenti segnala errore.
 * - Se l'attività è già nello stato richiesto, stampa messaggio e non cambia nulla.
 * - Se l'attività è in ritardo e si sceglie "in corso", avvisa l'utente.
 *
 * Se la scelta è "in corso" (1), imposta anche il tempo di inizio dell'attività.
 * Aggiorna infine lo stato con la nuova scelta.
 *
 * Parametri:
 *   a      - Struttura 'attivita' da aggiornare.
 *   scelta - Intero che indica il nuovo stato desiderato:
 *            1 per "in corso", 2 per "completata".
 *
 * Ritorna:
 *   0 se l'aggiornamento è avvenuto con successo,
 *   1 in caso di input non valido,
 *   2 in caso di attività non valida o stato non modificabile.
 *
 * Nota:
 *   La funzione stampa messaggi di errore o di conferma direttamente su stdout.
 */

int aggiorna_stato(attivita a, int scelta) {
	if(a==NULLATTIVITA) {
        printf("Attivita non valida\n");
	    return 2;
	}
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

    if(rit_stato(a)==1 && scelta==1){
        printf("Questa attività è già in corso");
        return 2;
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

/*
 * Funzione: genera_report_settimanale
 * -----------------------------------
 * Genera un report settimanale delle attività suddividendole in quattro categorie:
 * - Attività della settimana corrente non scadute
 * - Attività della settimana prossima
 * - Attività delle settimane future (dopo la prossima)
 * - Attività scadute
 *
 * Per ogni categoria, la funzione scorre la lista delle attività, confronta la data di scadenza
 * con la data odierna, e stampa le attività corrispondenti.
 *
 * Parametri:
 *   l - Lista di attività da analizzare.
 *
 * Ritorna:
 *   Void. Il report viene stampato direttamente su stdout.
 *
 * Nota:
 *   La funzione utilizza funzioni di supporto per ottenere la data corrente,
 *   calcolare la settimana dell'anno e confrontare date.
 */

void genera_report_settimanale(lista l) {

    data_ora oggi = ottieni_data_ora();
    int settimana_corrente = numero_settimana(oggi);
    int anno_corrente = rit_anno(oggi);

    printf("============Report Settimanale==========\n");

    const char* categorie[] = {
        "SETTIMANA CORRENTE:",
        "SETTIMANA PROSSIMA",
        "SETTIMANE FUTURE",
        "ATTIVITA SCADUTE"
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

/*
 * Funzione: chiedi_attivita_per_id
 * --------------------------------
 * Richiede all'utente di inserire l'ID di un'attività presente nella lista,
 * stampando la lista completa e ripetendo la richiesta finché non viene inserito
 * un ID valido corrispondente a un'attività esistente.
 *
 * Parametri:
 *   l - Lista di attività in cui cercare l'attività con l'ID specificato.
 *
 * Ritorna:
 *   L'attività corrispondente all'ID inserito dall'utente, oppure NULLATTIVITA
 *   se la lista è vuota.
 *
 * Nota:
 *   La funzione utilizza la funzione stampa_lista per mostrare tutte le attività
 *   e cerca l'attività tramite cerca_attivita_per_id.
 *   Continua a chiedere l'ID finché non ne viene inserito uno valido.
 */

attivita chiedi_attivita_per_id(lista l) {
    if(lista_vuota(l)) {
       return NULLATTIVITA;
    }
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

/*
 * Funzione: menu
 * --------------
 * Gestisce l'interfaccia a menu per l'utente, consentendo di selezionare
 * diverse operazioni sulle attività
 * Il menu continua a essere mostrato finché l'utente non sceglie di uscire (0).
 *
 * Parametri:
 *   l          - Lista delle attività da gestire.
 *   ultimo_id  - Puntatore all'intero contenente l'ultimo ID usato,
 *                necessario per l'inserimento di nuove attività.
 *
 * Ritorna:
 *   Void. Le operazioni vengono eseguite direttamente e i risultati stampati.
 *
 * Nota:
 *   La funzione verifica che 'ultimo_id' sia un puntatore valido.
 *   La lista delle attività viene liberata al termine (scelta 0).
 *   Usa funzioni di supporto per ogni operazione (report, progresso, aggiornamento, ecc.).
 */

void menu(lista l, int *ultimo_id) {
	if(ultimo_id==NULL){
		printf("Attenzione ultimo_id non punta ad una variabile");
		return;
	}

    int scelta;
    do {
        printf("\n=================MENU===================\n");
        printf("1. Report settimanale\n");
        printf("2. Monitoraggio del progresso\n");
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
				if(lista_vuota(l)){
					printf("La lista delle attività è vuota");
					break;
				}
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
				if(lista_vuota(l)){
					printf("La lista delle attività è vuota");
					break;
				}
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




