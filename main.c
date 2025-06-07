#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "utile.h"

#define MAX_LINE 512
#define MAX 100


int main(int argc,char *argv[]){
     if (argc != 2) {
        printf("Uso corretto: %s <nome_file.txt>\n", argv[0]);
        return 1;
    }

	int ultimo_id;
    lista l = carica_attivita_da_file(argv[1], &ultimo_id);

    if (lista_vuota(l)) {
		printf("Errore: la lista è vuota");
        return 1;
    }

	menu(l, &ultimo_id);

}