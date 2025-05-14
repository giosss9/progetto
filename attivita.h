typedef struct attivita_studio *attivita;

attivita crea_attivita(char *d, char *c, int g, int m, int a, int tempo, int pr, int st);
void stampa_attivita(attivita a);