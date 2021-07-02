all: main.o dataset.o models.o
	gcc main.o dataset.o models.o -o main -Wall -ggdb3

main.o: main.c dataset.h models.h
	gcc -I . -c main.c -Wall -ggdb3

dataset.o: dataset.c dataset.h
	gcc -I . -c dataset.c -Wall -ggdb3
	
models.o: models.c models.h
	gcc -I . -c models.c -Wall -ggdb3

clean:
	rm -rf *.o *.csv *.txt
	rm main
