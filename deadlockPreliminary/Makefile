all: RAG.o deadlock.c
	gcc -Wall -o deadlock deadlock.c RAG.o

RAG.o: RAG.h RAG.c
	gcc -Wall -c RAG.c

clean: 
	rm -f *.o deadlock