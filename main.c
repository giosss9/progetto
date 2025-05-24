#include <stdio.h>
#include <string.h>
#include "attivita.h"
#include "lista.h"
#include "utile.h"

#define MAX_LINE 512
#define MAX 100

void menu(lista l) {
    int scelta;
    do {
        printf("\n======== MENU ========\n");
        printf("1. Visualizza attività\n");
        printf("2. Mostra stato avanzamento\n");
        printf("3. Aggiorna stato\n");
        printf("0. Esci\n");
        printf("======================\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        getchar(); // per consumare il newline

        switch (scelta) {
            case 1:
                stampa_lista(l);
                break;
            case 2:
                mostra_progresso(l);
                break;
            case 3:
				char descrizione[MAX];
				int nuovo,v=1, k=1;
				attivita a;
				while(v==1){
					while(k==1){
						printf("Inserisci la descrizione dell'attivita che vuoi aggiornare: \n");
						fgets(descrizione,MAX,stdin);
						descrizione[strcspn(descrizione, "\n")] = 0;
						a=cerca_attivita_per_descrizione(l,descrizione);
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
			case 0:
                printf("Uscita in corso...\n");
                break;
            default:
                printf("Scelta non valida.\n");
        }
    } while (scelta != 0);
}


int main(int argc,char *argv[]){
    if(argc!=2){
        printf("Input invalido\n");
        return 1;
    }

    char *nome_file = argv[1];

    char riga[MAX_LINE];
    int riga_num=0;

    FILE *input = fopen(nome_file, "r");
    if (input == NULL) {
        printf("Impossibile aprire il file %s\n", nome_file);
        return 1;
    }

    lista l = nuova_lista();

    //Inserimento da file input
    while (fgets(riga, MAX_LINE, input) != NULL) {
        riga_num++;

        char descrizione[MAX], corso[MAX];
        int giorno, mese, anno, tempo_stimato, priorita, stato, ore;

        riga[strcspn(riga, "\n")] = 0;

        if (sscanf(riga, "%99[^;];%99[^;];%d;%d;%d;%d;%d;%d;%d",
                   descrizione, corso,
                   &giorno, &mese, &anno, &ore,
                   &tempo_stimato, &priorita, &stato) != 9) {
            fprintf(stderr, "Riga %d: formato riga non valido: %s\n", riga_num, riga);
            break;
                   }

        // Validazione dei campi
        if (giorno < 1 || giorno > 31 ||
            mese < 1 || mese > 12 ||
            anno <= 0 ||
            tempo_stimato <= 0 ||
            priorita < 0 || priorita > 2 ||
            stato < 0 || stato > 1 ||  			//lo stato deve partire da non iniziare o in corso, un'attivita non può
			ore<0 || ore >24) { 				//essere completata o in ritardo dall'inserimento


            fprintf(stderr, "Riga %d: dati non validi:\n", riga_num);
            fprintf(stderr, "  Giorno: %d (1-31)\n", giorno);
            fprintf(stderr, "  Mese: %d (1-12)\n", mese);
			fprintf(stderr, "  Ore: %d (0-24)\n", ore);
            fprintf(stderr, "  Anno: %d (>0)\n", anno);
            fprintf(stderr, "  Tempo stimato: %d (>0)\n", tempo_stimato);
            fprintf(stderr, "  Priorità: %d (0=bassa, 1=media, 2=alta)\n", priorita);
            fprintf(stderr, "  Stato: %d (0=non iniziata, 1=in corso)\n", stato);
            break;
        }

        attivita nuova = crea_attivita(descrizione, corso, giorno, mese, anno,
                                   tempo_stimato, priorita, stato,ore);
        l = cons_lista(nuova, l);
    }

    fclose(input);

	menu(l);


}