CC = gcc

LIBS = -lm 
CCFLAGS = -Wall -ggdb

OBJ = util.o analyseur_lexical.o analyseur_syntaxique.o premiers.o suivants.o affiche_arbre_abstrait.o syntabs.o tabsymboles.o

all: compilateur

compilateur: compilateur.c $(OBJ)
	$(CC) $(CCFLAGS) -o compilateur compilateur.c $(OBJ)

analyseur_syntaxique.o: analyseur_syntaxique.c
	$(CC) $(CCFLAGS) -c analyseur_syntaxique.c
analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c analyseur_lexical.c
util.o: util.c
	$(CC) $(CCFLAGS) -c util.c
	
premiers.o: premiers.c
	$(CC) $(CCFLAGS) -c premiers.c
	
suivants.o: suivants.c
	$(CC) $(CCFLAGS) -c suivants.c
	
affiche_arbre_abstrait.o: affiche_arbre_abstrait.c
	$(CC) $(CCFLAGS) -c affiche_arbre_abstrait.c
	
syntabs.o: syntabs.c
	$(CC) $(CCFLAGS) -c syntabs.c
	
tabsymboles.o: tabsymboles.c
	$(CC) $(CCFLAGS) -c tabsymboles.c

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
