#ifndef DATA_H
#define DATA_H

//Dichiarazione del tipo data_ora che è un puntatore alla struttura data_ora definita in data.c
typedef struct data_ora *data_ora;

// Dichiarazione delle funzioni

/*
 * Funzione: crea_data_ora
 * ---------------------------------------
 *
 * Parametri:
 *  Tutti interi
 *    giorno mese anno ore minuti secondi
 *
 * Precondizioni:
 *	    giorno ∈ [1, 31] (a seconda del mese e dell'anno)
 *	    mese ∈ [1, 12]
 *	    anno > 0
 *	    ore ∈ [0, 23]
 *	    minuti ∈ [0, 59]
 *	    secondi ∈ [0, 59]
 *
 * Postcondizioni:
 *	    Ritorna una struttura data_ora inizializzata con i parametri forniti
 *	    Se l’allocazione o il controllo per i dati fallisce, ritorna NULL.
 *
 */
data_ora crea_data_ora(int giorno, int mese, int anno, int ore, int minuti, int secondi);

/*
 * Funzione: ottieni_data_ora
 * ---------------------------------------
 *
 * Parametri:
 *    Nessuno
 *
 * Precondizioni:
 *    Nessuna
 *
 * Postcondizioni:
 *     Viene restituita una struttura data_ora contenente data e ora corrente del sistema.
 *     Se l’allocazione di memoria fallisce, ritorna NULL.
 *
 */
data_ora ottieni_data_ora();

/*
 * Funzione: calcolo_tempo_trascorso
 * ---------------------------------------
 *
 * Parametri:
 *    inizio che è un tipo data_ora
 *
 * Precondizioni:
 *    inizio è un puntatore valido ad una struttura data_ora
 *    correttamente inizializzata.
 *
 * Postcondizioni:
 *     Retituisce una nuova data_ora che rappresenta
 *     il tempo trascorso tra la data inizio e quella corrente.
 *
 */
data_ora calcolo_tempo_trascorso(data_ora inizio);

/*
 * Funzione: confronta_data_ora
 * ---------------------------------------
 *
 * Parametri:
 *    a primo tipo data_ora
 *    b secondo tipo data_ora
 *
 * Precondizioni:
 *    a e b devono essere due puntatori validi a strutture data_ora
 *
 * Postcondizioni:
 *     Restituisce:
 *         -1 se a è antecedente a b
 *          0 se sono uguali
 * 	        1 se a è successiva a b
 *          2 se a e b non sono validi
 */
int confronta_data_ora(data_ora a, data_ora b);

/*
 * Funzione: numero_settimana
 * ---------------------------------------
 *
 * Parametri:
 *    d tipo data_ora
 *
 * Precondizioni:
 *    d è un puntatore valido ad una struttura data_ora
 *
 * Postcondizioni:
 *    Restituisce il numero della settimana dell'anno
 *
 */
int numero_settimana(data_ora d);

/*
 * Funzione: rit_campo (giorno mese anno ore minuti secondi)
 * ---------------------------------------
 *
 * Parametri:
 *    d è un tipo data_ora
 *
 * Precondizioni:
 *    d è un puntatore valido ad una struttura data_ora
 *
 * Postcondizioni:
 *    Restituisce il campo della struttura d, se non è nulla.
 *
 */
int rit_giorno(data_ora d);
int rit_mese(data_ora d);
int rit_anno(data_ora d);
int rit_ore(data_ora d);
int rit_minuti(data_ora d);
int rit_secondi(data_ora d);

//Per il testing

/*
 * Funzione: imposta_ore
 * ---------------------------------------
 *
 * Parametri:
 *    d è un tipo data_ora
 *    ore tipo intero
 *
 * Precondizioni:
 *    d è un puntatore valido ad una struttura data_ora
 *    ore deve essere compreso tra 0 e 23
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *    Cambia il campo ore della struttura d
 *
 */
void imposta_ore(data_ora d, int ore);

/*
 * Funzione: imposta_giorno
 * ---------------------------------------
 *
 * Parametri:
 *    d è un tipo data_ora
 *    giorno tipo intero
 *
 * Precondizioni:
 *    d è un puntatore valido ad una struttura data_ora
 *    giorno deve essere valido a seconda del mese e dell'anno
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *    Cambia il campo giorno della struttura d
 *
 */
void imposta_giorno(data_ora d, int giorno);

/*
 * Funzione: libera_data_ora
 * ---------------------------------------
 *
 * Parametri:
 *    d tipo data_ora
 *
 * Precondizioni:
 *    d è un puntatore valido a una struttura data_ora.
 *
 * Postcondizioni:
 *    Non ritorna niente
 *
 * Effetti collaterali:
 *    Libera il puntatore d dalla struttura assegnatagli
 *
 */
void libera_data_ora(data_ora d);
#endif // DATA_H
