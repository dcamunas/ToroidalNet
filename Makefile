DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRFIL := files/

CFLAGS := -I $(DIRHEA)
LDLIBS := -lm
CC := mpicc
RUN := mpirun

all : dirs toroidalNet solution

dirs:
	mkdir -p $(DIREXE)

toroidalNet:
	$(CC) $(DIRSRC)toroidalNet.c $(CFLAGS) $(LDLIBS) -o $(DIREXE)toroidalNet

solution:
	$(RUN) -n 9 ./$(DIREXE)toroidalNet 

clean : 
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRHEA)*~ $(DIRSRC)*~ 
