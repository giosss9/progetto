main_testing.out: main_testing.o attivita.o lista.o data.o utile.o
	gcc main_testing.o attivita.o lista.o data.o utile.o -o main_testing.out

attivita.o: attivita.c attivita.h data.h utile.h
	gcc -c attivita.c

lista.o: lista.c lista.h attivita.h
	gcc -c lista.c

data.o: data.c data.h
	gcc -c data.c

utile.o: utile.c utile.h attivita.h lista.h data.h
	gcc -c utile.c

main_testing.o: main_testing.c attivita.h lista.h data.h utile.h
	gcc -c main_testing.c -std=c99

run: main_testing.out
	./main_testing.out test_suite.txt risultati.txt

clean:
	rm -f *.o main_testing.out
