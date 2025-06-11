//File main.c

#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "utile.h"

#define MAX_LINE 512
#define MAX 100


int main(int argc, char *argv[]) {
    int ultimo_id = 0;
    lista l = nuova_lista();

 	if (argc == 2) {
    	char risposta;
    	do {
        	printf("È presente il file '%s'. Vuoi caricare le attività da questo file? (s/n): ", argv[1]);
        	risposta = getchar();
        	while (getchar() != '\n');
    	} while (risposta != 's' && risposta != 'S' && risposta != 'n' && risposta != 'N');

    	if (risposta == 's' || risposta == 'S') {
        l = carica_attivita_da_file(argv[1], &ultimo_id);
    }
    // Se risponde 'n' o 'N', va direttamente al menu
	}


    menu(l, &ultimo_id);

    return 0;
}
