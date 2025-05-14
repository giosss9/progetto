main.out: main.o attivita.o lista.o
	gcc main.o attivita.o lista.o -o main.out

attivita.o: attivita.c attivita.h
	gcc -c ./attivita.c

lista.o: lista.c lista.h attivita.o
	gcc -c ./lista.c

main.o: main.c attivita.o lista.o
	gcc -c ./main.c -std=c99

clean:
	rm -f *.o *.out