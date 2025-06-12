main_testing.out: main_testing.o ../attivita.o ../lista.o ../data.o ../utile.o
	gcc main_testing.o ../attivita.o ../lista.o ../data.o ../utile.o -o main_testing.out

../attivita.o: ../attivita.c ../attivita.h ../data.h ../utile.h
	gcc -c ../attivita.c -o ../attivita.o

../lista.o: ../lista.c ../lista.h ../attivita.h
	gcc -c ../lista.c -o ../lista.o

../data.o: ../data.c ../data.h
	gcc -c ../data.c -o ../data.o

../utile.o: ../utile.c ../utile.h ../attivita.h ../lista.h ../data.h
	gcc -c ../utile.c -o ../utile.o

main_testing.o: main_testing.c ../attivita.h ../lista.h ../data.h ../utile.h
	gcc -c main_testing.c -std=c99 -o main_testing.o

run: main_testing.out
	./main_testing.out test_suite.txt risultati.txt

clean:
	rm -f ../*.o main_testing.o main_testing.out
