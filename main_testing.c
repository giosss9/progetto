#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "utile.h"

# define M 20

int confronta_file(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "r");
    FILE* f2 = fopen(file2, "r");

    if (!f1 || !f2) {
        fclose(f1);
        fclose(f2);
        return 0; // errore apertura file
    }

    int c1, c2;
    do {
        c1 = fgetc(f1);
        c2 = fgetc(f2);
        if (c1 != c2) {
            fclose(f1);
            fclose(f2);
            return 0; // file diversi
        }
    } while (c1 != EOF && c2 != EOF);

    fclose(f1);
    fclose(f2);
    return 1; // file uguali
}


int test_case_inserimento(char *tc_id, int n){
   char input_fnome[M], output_fnome[M], oracolo_fnome[M];
   int ris;

   // costruiamo i nomi dei file
   sprintf(input_fnome, "%s_input.txt", tc_id);
   sprintf(output_fnome, "%s_output.txt", tc_id);
   sprintf(oracolo_fnome, "%s_oracolo.txt", tc_id);

   // allochiamo memoria per la lista
   lista test_lista=nuova_lista();

	int id=0;


	//Redirigo stdout nel file di output
	FILE* out = freopen(output_fnome, "w", stdout);
	if (!out) {
    	fprintf(stderr, "Errore apertura output file: %s\n", output_fnome);
    	return 0;
	}

	// carica file di input
	test_lista=carica_attivita_da_file(input_fnome,&id);

	stampa_lista(test_lista);
	printf("%d",id);

	//Mi assicuro che sia tutto scritto
	fflush(stdout);

	fseek(out,0,SEEK_SET);

    // Ora confronta l'output generato con l'oracolo
    ris = confronta_file(oracolo_fnome, output_fnome);

    libera_lista(test_lista);

   return ris;
}


int main(int argc, char *argv[])
{
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

   // Scansione del file di input nel ciclo while.

   while(fscanf(test_suite, "%s%d", tc_id, &n) == 2){
        pass = test_case_inserimento(tc_id, n);

        fprintf(risultati,"%s ", tc_id);
        if(pass == 1)
              fprintf(risultati, "PASS \n");
        else
              fprintf(risultati, "FAIL \n");
   }

   fclose(test_suite);  // chiusura file di input
   fclose(risultati);         // chiusura file di output
}
