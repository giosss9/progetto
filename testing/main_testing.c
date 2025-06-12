// File main_testing

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../attivita.h"
#include "../data.h"
#include "../lista.h"
#include "../utile.h"

# define M 20 //Costante che determina la lunghezza massima del nome dei file
#define MAX_RIGA 512
#define MAX 100

/*
 * Funzione: confronta_file
 * ---------------------------------------
 *
 * Parametri:
 *    file1 - nome del primo file da confrontare
 *    file2 - nome del secondo file da confrontare
 *
 * Precondizioni:
 *    I file devono esistere ed essere leggibili
 *
 * Postcondizioni:
 *    Restituisce 1 se i file sono uguali, 0 altrimenti o se non sono apribili
 *
 */
int confronta_file(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "r");
    FILE* f2 = fopen(file2, "r");

    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 0;
    }

    char riga1[MAX_RIGA], riga2[MAX_RIGA];

    while (1) {
        // Leggi riga valida da f1
        do {
            if (!fgets(riga1, MAX_RIGA, f1)) riga1[0] = '\0';
            char* p = riga1;
            while (isspace(*p)) p++;  // salta spazi iniziali
            if (*p == '#' || *p == '\0' || *p == '\n') riga1[0] = '\0';  // ignora riga
        } while (riga1[0] == '\0' && !feof(f1));

        // Leggi riga valida da f2
        do {
            if (!fgets(riga2, MAX_RIGA, f2)) riga2[0] = '\0';
            char* p = riga2;
            while (isspace(*p)) p++;
            if (*p == '#' || *p == '\0' || *p == '\n') riga2[0] = '\0';
        } while (riga2[0] == '\0' && !feof(f2));

        // Confronta righe
        if (strcmp(riga1, riga2) != 0) {
            fclose(f1);
            fclose(f2);
            return 0;
        }

        // Se entrambi sono finiti
        if (feof(f1) && feof(f2)) break;
    }

    fclose(f1);
    fclose(f2);
    return 1;
}


/*
 * Funzione: test_case_rimozione
 * ---------------------------------------
 *
 * Parametri:
 *    tc_id - identificatore del test case (nome base dei file di input/output)
 *    n     - numero atteso di attività dopo la rimozione
 *
 * Precondizioni:
 *    tc_id!=NULL
 *
 * Postcondizioni:
 *    Restituisce 1 se l'output coincide con l'oracolo
 * 		e se il numero atteso di attività coincide con il numero di elementi della lista
 * 		0 altrimenti
 *
 * Effetti collaterali
 * 	  Stampa la lista e eventuali messaggi di errore sul file di output
 *
 */

int test_case_rimozione(char *tc_id, int n){
  	if(tc_id==NULL)
          return 0;

	char input_fnome[M], output_fnome[M], oracolo_fnome[M];
	int ris;
	int num_attivita;
	// costruiamo i nomi dei file
	sprintf(input_fnome, "%s_input.txt", tc_id);
	sprintf(output_fnome, "%s_output.txt", tc_id);
	sprintf(oracolo_fnome, "%s_oracolo.txt", tc_id);

	// allochiamo memoria per la lista
	lista test_lista=nuova_lista();

	int id=0;

	// Salva stdout originale
	FILE* original_stdout = stdout;

	// Redirige stdout nel file
	FILE* out = freopen(output_fnome, "w", stdout);
	if (!out) {
		fprintf(stderr, "Errore apertura output file: %s\n", output_fnome);
		return 0;
	}

	// carica file di input
	test_lista=carica_attivita_da_file(input_fnome,&id);

	//Rimuoviamo attivita con id 3
	test_lista=rimuovi_attivita_per_id(test_lista,3);

	stampa_lista(test_lista);

	//Per testare se sono state inserite n attivita
	num_attivita=dimensione_lista(test_lista);
	if(n!=num_attivita)
		return 0;

	printf("%d",num_attivita);

	//Flush per essere sicuri che venga tutto scritto e ripristina stdout
	fflush(stdout);
	fclose(out);
	stdout = original_stdout;

	// Ora confronta l'output generato con l'oracolo
	ris = confronta_file(oracolo_fnome, output_fnome);

	libera_lista(test_lista);

	return ris;
}

/*
 * Funzione: test_case_report
 * ---------------------------------------
 *
 * Parametri:
 *    tc_id - identificatore del test case
 *    n     - numero atteso di attività nella lista
 *
 * Precondizioni:
 *    Il file input deve esistere e contenere attività valide
 *
 * Postcondizioni:
 * 	 Restituisce 1 se output e oracolo corrispondono
 * 	  e se il numero atteso di attività coincide con il numero di elementi della lista
 *    0 altrimenti
 *
 * Effetti collaterali:
 *    Genera un report settimanale nel file di output,
 * 	  confronta l'output con l'oracolo
 *
 */
int test_case_report(char *tc_id, int n){
	char input_fnome[M], output_fnome[M], oracolo_fnome[M];
	int ris;
	int num_attivita;

	// costruiamo i nomi dei file
	sprintf(input_fnome, "%s_input.txt", tc_id);
	sprintf(output_fnome, "%s_output.txt", tc_id);
	sprintf(oracolo_fnome, "%s_oracolo.txt", tc_id);

	// allochiamo memoria per la lista
	lista test_lista=nuova_lista();

	int id=0;

	// Salva stdout originale
	FILE* original_stdout = stdout;

	// Redirige stdout nel file
	FILE* out = freopen(output_fnome, "w", stdout);
	if (!out) {
		fprintf(stderr, "Errore apertura output file: %s\n", output_fnome);
		return 0;
	}


	// carica file di input
	test_lista=carica_attivita_da_file(input_fnome,&id);

	//Modifichiamo la scadenza dell'attivita che ha id 9 per farla apparire nella senzione attivita scadute
	attivita ritardo=cerca_attivita_per_id(test_lista, 9);
	data_ora nuova_scadenza=rit_scadenza(ritardo);
	int nuovo_giorno=rit_giorno(nuova_scadenza);
	imposta_giorno(nuova_scadenza, nuovo_giorno-10);
	//aggiorna_stato(ritardo,1);

	genera_report_settimanale(test_lista);

	//Per testare se sono state inserite n attivita
	num_attivita=dimensione_lista(test_lista);
	if(n!=num_attivita)
		return 0;

	printf("%d",num_attivita);

	//Flush per essere sicuri che venga tutto scritto e ripristina stdout
	fflush(stdout);
	fclose(out);
	stdout = original_stdout;

	// Ora confronta l'output generato con l'oracolo
	ris = confronta_file(oracolo_fnome, output_fnome);

	libera_lista(test_lista);

	return ris;
}

/*
 * Funzione: test_case_mostra_progresso
 * ---------------------------------------
 *
 * Parametri:
 *    tc_id - identificatore del test case
 *    n     - numero atteso di attività nella lista
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 *    Restituisce 1 se output e oracolo corrispondono
 * 	  e se il numero di attivita coincide con n, altrimenti 0.
 *
 * Effetti collaterali:
 *   Simula stati diversi delle attività,
 * 	 mostra il progresso sul file di output e confronta l'output con l'oracolo
 *
 */

int test_case_mostra_progresso(char *tc_id, int n){
	char input_fnome[M], output_fnome[M], oracolo_fnome[M];
	int ris;
	int num_attivita;

	// costruiamo i nomi dei file
	sprintf(input_fnome, "%s_input.txt", tc_id);
	sprintf(output_fnome, "%s_output.txt", tc_id);
	sprintf(oracolo_fnome, "%s_oracolo.txt", tc_id);

	// allochiamo memoria per la lista
	lista test_lista=nuova_lista();

	int id=0;

	// Salva stdout originale
	FILE* original_stdout = stdout;

	// Redirige stdout nel file
	FILE* out = freopen(output_fnome, "w", stdout);
	if (!out) {
		fprintf(stderr, "Errore apertura output file: %s\n", output_fnome);
		return 0;
	}


	// carica file di input
	test_lista=carica_attivita_da_file(input_fnome,&id);

	//Impostiamo l'attivita che ha id 1 in corso
	attivita in_corso=cerca_attivita_per_id(test_lista, 1);
	aggiorna_stato(in_corso,1);

	//Impostiamo l'attivita che ha id 2 completata
	attivita completata=cerca_attivita_per_id(test_lista, 2);
	aggiorna_stato(completata,1); //Perche deve passare prima da in corso
	aggiorna_stato(completata,2);

	//Impostiamo l'attivita che ha id 3 in ritardo, per farlo impostiamo la data di scadenza a 10 giorni prima
	attivita ritardo=cerca_attivita_per_id(test_lista, 3);
	data_ora nuova_scadenza=rit_scadenza(ritardo);
	int nuovo_giorno=rit_giorno(nuova_scadenza);
	imposta_giorno(nuova_scadenza, nuovo_giorno-10);
	//In questo modo controlliamo anche che la funzione controlli bene che un'attivita è in ritardo
	aggiorna_stato(ritardo,1);

	mostra_progresso(test_lista);

	//Per testare se sono state inserite n attivita
	num_attivita=dimensione_lista(test_lista);
	if(n!=num_attivita)
		return 0;

	printf("%d",num_attivita);

	//Flush per essere sicuri che venga tutto scritto e ripristina stdout
	fflush(stdout);
	fclose(out);
	stdout = original_stdout;

	// Ora confronta l'output generato con l'oracolo
	ris = confronta_file(oracolo_fnome, output_fnome);

	libera_lista(test_lista);

	return ris;
}

/*
 * Funzione: test_case_progresso
 * ---------------------------------------
 *
 * Parametri:
 *    tc_id - identificatore del test case
 *    n     - ore da simulare come trascorse per il calcolo del progresso
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 *   Restituisce 1 se output e oracolo corrispondono, altrimenti 0.
 *
 * Effetti collaterali:
 * 	 Calcola il progresso di un’attività, presa dal file di input,
 * 	 dopo n ore, lo stampa sul file di output e confronta l’output con l’oracolo.
 *
 */
int test_case_progresso(char *tc_id, int n) {
    char input_fnome[M], output_fnome[M], oracolo_fnome[M];
    int ris;

    sprintf(input_fnome, "%s_input.txt", tc_id);
    sprintf(output_fnome, "%s_output.txt", tc_id);
    sprintf(oracolo_fnome, "%s_oracolo.txt", tc_id);

    FILE *input = fopen(input_fnome, "r");
    if (input == NULL) {
        fprintf(stderr, "Impossibile aprire il file %s\n", input_fnome);
        return 0;
    }

    char riga[MAX_RIGA];
    int riga_num = 0;
    int id = 0;
    attivita test_att = NULLATTIVITA;

    while (fgets(riga, MAX_RIGA, input) != NULL) {
        riga_num++;
        riga[strcspn(riga, "\r\n")] = 0;
        if (riga[0] == '\0' || riga[0] == '#') continue;

        char descrizione[MAX], corso[MAX];
        int giorno, mese, anno, tempo_stimato, priorita, ore;

        int letti = sscanf(riga, "%99[^;];%99[^;];%d;%d;%d;%d;%d;%d",
                           descrizione, corso,
                           &giorno, &mese, &anno, &ore,
                           &tempo_stimato, &priorita);

        if (letti != 8) {
            fprintf(stderr, "Formato errato alla riga %d: %s\n", riga_num, riga);
            fclose(input);
            return 0;
        }

        test_att = crea_attivita(descrizione, corso, giorno, mese, anno,
                                 tempo_stimato, priorita, ore, id);

        if (test_att == NULLATTIVITA) {
            fprintf(stderr, "Errore: nessuna attività valida trovata.\n");
            fclose(input);
            return 0;
        }

        break;
    }
    fclose(input);

    // Salva stdout originale
    FILE* original_stdout = stdout;

    // Redirige stdout nel file
    FILE* out = freopen(output_fnome, "w", stdout);
    if (!out) {
        fprintf(stderr, "Errore apertura output file: %s\n", output_fnome);
        return 0;
    }

	//aggiorna stato attivita
	aggiorna_stato(test_att, 1);

	//calcolo tempo trascorso
	data_ora inizio = rit_tempo_inizio(test_att);
	data_ora trascorso = calcolo_tempo_trascorso(inizio);

	int ore_attuali = rit_ore(inizio);

	//Per far apparire n ore piu avanti
	imposta_ore(trascorso, n);

    // Scrive su file
    calcolo_progresso(test_att, trascorso);

    //Flush per essere sicuiri che venga tutto scritto e ripristina stdout
    fflush(stdout);
    fclose(out);

	//Ritorno a stdout originale per permettere ad altre funzioni di redirigere ancora stdout
    stdout = original_stdout;

    //Confronta
    ris = confronta_file(oracolo_fnome, output_fnome);

    libera_attivita(test_att);
    return ris;
}

/*
 * Funzione: test_case_inserimento
 * ---------------------------------------
 *
 * Parametri:
 *    tc_id - identificatore del test case
 *    n     - numero atteso di attività caricate dalla lista
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 * 	 Restituisce 1 se i file oracolo e output corrispondono e se
 *   il numero di attività è quello atteso, 0 altrimenti
 *
 *
 * Effetti collaterali:
 * 	 Carica le attività in una lista dal file di input,
 *   stampa la lista e eventuali messaggi di errore sul file tc_id_output.txt,
 * 	 e confronta l’output con l’oracolo
 *
 */
int test_case_inserimento(char *tc_id, int n){
   	char input_fnome[M], output_fnome[M], oracolo_fnome[M];
   	int ris;
	int num_attivita;
   	// costruiamo i nomi dei file
   	sprintf(input_fnome, "%s_input.txt", tc_id);
   	sprintf(output_fnome, "%s_output.txt", tc_id);
   	sprintf(oracolo_fnome, "%s_oracolo.txt", tc_id);

   	// allochiamo memoria per la lista
   	lista test_lista=nuova_lista();

	int id=0;


	// Salva stdout originale
	FILE* original_stdout = stdout;

	// Redirige stdout nel file
	FILE* out = freopen(output_fnome, "w", stdout);
	if (!out) {
		fprintf(stderr, "Errore apertura output file: %s\n", output_fnome);
		return 0;
	}


	// carica file di input
	test_lista=carica_attivita_da_file(input_fnome,&id);

	stampa_lista(test_lista);

	//Per testare se sono state inserite n attivita
	num_attivita=dimensione_lista(test_lista);
	if(n!=num_attivita)
		return 0;

	printf("%d",num_attivita);

	//Flush per essere sicuri che venga tutto scritto e ripristina stdout
	fflush(stdout);
	fclose(out);
	stdout = original_stdout;

    // Ora confronta l'output generato con l'oracolo
    ris = confronta_file(oracolo_fnome, output_fnome);

    libera_lista(test_lista);

   return ris;
}


/*
 * Funzione: main
 * ---------------------------------------
 *
 * Parametri:
 *    argc - numero di argomenti passati da linea di comando
 *    argv - array di stringhe contenente i nomi dei file
 *
 *
 * Precondizioni:
 *    argc deve essere uguale a 3
 *    argv deve contenere: (main_testing.c, test_suite.txt, risultati.txt)
 *
 * Postcondizioni:
 *    Ritorna -1 se argc<3 e se i file non esistono. altrmenti 0
 *
 * Effetti collaterali:
 *     Esegue i vari test_case aprendo il file test_suite per leggere i loro id,
 *     scrive TCID "PASS"/"FAIL" nel file risultati per ogni caso
 *
 */

int main(int argc, char *argv[]) {

   FILE *test_suite, *risultati;
   char tc_id[M];  // stringa contenente l'identificativo del test case
   int n, pass;

   if(argc < 3){
        printf("Nomi dei file mancanti \n");
        return -1;
   }

   test_suite = fopen(argv[1], "r");  // apro file contenente test suite
   risultati = fopen(argv[2], "w+");   // apro file contenente risultati test

   if( test_suite==NULL || risultati == NULL ) {
       printf("Errore in apertura dei file \n");
       return -1;
   }

	int i=1; //per differenziare i tipi di test case nel ciclo

   // Scansione del file di input nel ciclo while.
   while(fscanf(test_suite, "%s%d", tc_id, &n) == 2){
		if(i<=4)
        	pass = test_case_inserimento(tc_id, n);

		else if(i<7)
			pass = test_case_progresso(tc_id, n);

		if(i==7)
			pass = test_case_mostra_progresso(tc_id, n);

		if(i==8)
			pass = test_case_report(tc_id,n);

		if(i==9)
			pass = test_case_rimozione(tc_id,n);

        fprintf(risultati,"%s ", tc_id);
        if(pass == 1)
              fprintf(risultati, "PASS \n");
        else
              fprintf(risultati, "FAIL \n");

		i++;
   }

   fclose(test_suite);  // chiusura file di input
   fclose(risultati);         // chiusura file di output

   return 0;
}