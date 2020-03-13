DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRFIL := files/

CFLAGS := -I $(DIRHEA)
LDLIBS :=
CC := mpicc
RUN := mpirun

all : dirs main

dirs:
	mkdir -p $(DIROBJ) $(DIREXE)

main:
	$(CC) $(DIRSRC)main.c $(CFLAGS) $^ -o $(DIREXE)main

solution:
	$(RUN) -n 2 ./$(DIREXE)main 
	# añadir L = 4 al final del comando run

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~ 
