main.out: main.o attivita.o lista.o data.o
	gcc main.o attivita.o lista.o data.o -o main.out

attivita.o: attivita.c attivita.h data.o
	gcc -c ./attivita.c

lista.o: lista.c lista.h attivita.o
	gcc -c ./lista.c

data.o: data.c data.h
	gcc -c data.c

utile.o:utile.c utile.h attivita.o lista.o data.o
	gcc -c utile.c

main.o: main.c attivita.o lista.o data.o utile.o
	gcc -c ./main.c -std=c99

