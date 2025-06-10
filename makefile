main.out: main.o attivita.o lista.o data.o utile.o
	gcc main.o attivita.o lista.o data.o utile.o -o main.out

attivita.o: attivita.c attivita.h data.h utile.h
	gcc -c attivita.c

lista.o: lista.c lista.h attivita.h
	gcc -c lista.c

data.o: data.c data.h
	gcc -c data.c

utile.o: utile.c utile.h attivita.h lista.h data.h
	gcc -c utile.c

main.o: main.c attivita.h lista.h data.h utile.h
	gcc -c main.c -std=c99

run: main.out
	./main.out input.txt

clean:
	rm -f *.o main.out
