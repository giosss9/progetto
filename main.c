#include <stdio.h>
#include <string.h>
#include "attivita.h"
#include "lista.h"
#define MAX_LINE 512

int main(int argc,char *argv[]){
    if(argc!=2){
        printf("Input invalido\n");
        return 1;
    }

    char *nome_file = argv[1];

    FILE *input = fopen(nome_file, "r");
    if (file == NULL) {
        printf("Impossibile aprire il file %s\n", nome_file);
        return 1;
    }

    lista l = nuova_lista();
    char riga[MAX_LINE];

    while (fgets(riga, MAX_LINE, file) != NULL) {
        riga_num++;

        char descrizione[MAX], corso[MAX];
        int giorno, mese, anno, tempo_stimato, priorita, stato;

        riga[strcspn(riga, "\n")] = 0;

        if (sscanf(riga, "%99[^;];%99[^;];%d;%d;%d;%d;%d;%d",
                   descrizione, corso,
                   &giorno, &mese, &anno,
                   &tempo_stimato, &priorita, &stato) != 8) {
            fprintf(stderr, "Riga %d: formato riga non valido: %s\n", riga_num, riga);
            break;
                   }

        // Validazione dei campi
        if (giorno < 1 || giorno > 31 ||
            mese < 1 || mese > 12 ||
            anno <= 0 ||
            tempo_stimato <= 0 ||
            priorita < 0 || priorita > 2 ||
            stato < 0 || stato > 2) {

            fprintf(stderr, "Riga %d: dati non validi:\n", riga_num);
            fprintf(stderr, "  Giorno: %d (1-31)\n", giorno);
            fprintf(stderr, "  Mese: %d (1-12)\n", mese);
            fprintf(stderr, "  Anno: %d (>0)\n", anno);
            fprintf(stderr, "  Tempo stimato: %d (>0)\n", tempo_stimato);
            fprintf(stderr, "  Priorità: %d (0=bassa, 1=media, 2=alta)\n", priorita);
            fprintf(stderr, "  Stato: %d (0=non iniziata, 1=in corso, 2=completata)\n", stato);
            break;
        }

        attivita nuova = crea_attivita(descrizione, corso, giorno, mese, anno,
                                   tempo_stimato, priorita, stato);
        l = cons_lista(nuova, l);
    }

    fclose(file);

    printf("Attività lette dal file:\n");
    stampa_lista(l);



}