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
        printf("1. Report settimanale\n");
        printf("2. Mostra stato avanzamento\n");
        printf("3. Aggiorna stato\n");
        printf("4. Inserisci nuova attivita\n");
        printf("0. Esci\n");
        printf("\n========================================\n");
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
            case 4:
                attivita nuova = inserisci_attivita_da_input();
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


int main(int argc,char *argv[]){
     if (argc != 2) {
        printf("Uso corretto: %s <nome_file.txt>\n", argv[0]);
        return 1;
    }

    lista l = carica_attivita_da_file(argv[1]);
    if (l == NULL) {
        return 1;
    }

	menu(l);

}