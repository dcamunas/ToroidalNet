DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRFIL := files/

CFLAGS := -I $(DIRHEA)
LDLIBS := -lm
CC := mpicc
RUN := mpirun

all : dirs main

dirs:
	mkdir -p $(DIREXE)

main:
	$(CC) $(DIRSRC)main.c $(CFLAGS) $(LDLIBS) -o $(DIREXE)main

solution:
	$(RUN) -n 9 ./$(DIREXE)main 

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~ 
