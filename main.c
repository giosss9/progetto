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
        printf("Hai specificato il file '%s'. Vuoi caricare le attività da questo file? (s/n): ", argv[1]);
        char risposta = getchar();
        getchar(); // per consumare il newline

        if (risposta == 's' || risposta == 'S') {
            l = carica_attivita_da_file(argv[1], &ultimo_id);
        }
        // Se risponde 'n', non caricare nulla, vai direttamente al menu
    }

    // Se non c’è file o risposta è 'n', parte comunque il menu con lista (vuota o caricata)
    menu(l, &ultimo_id);

    return 0;
}
