#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyseur_syntaxique.h"
#include "analyseur_lexical.h"
#include "premiers.h"
#include "suivants.h"
#include "symboles.h"

char yytext[100];
FILE *yyin;

int main(int argc, char **argv) {
	if(argc != 3){
		printf("Syntaxe : ./compilateur -l|-s|-a|-t ./tests/.l");
		exit(-1);
	}
  	yyin = fopen(argv[2], "r");
  	if(yyin == NULL){
    	fprintf(stderr, "impossible d'ouvrir le fichier %s\n", argv[2]);
    	exit(1);
  	}	
	if(strcmp(argv[1], "-l") == 0)
		analyseur_lexical(yyin);
	else if(strcmp(argv[1], "-s") == 0)
		analyseur_syntaxique(yyin, 1, 0, 0);
	else if(strcmp(argv[1], "-a") == 0)
		analyseur_syntaxique(yyin, 0, 1, 0);
	else if(strcmp(argv[1], "-t") == 0)
		analyseur_syntaxique(yyin, 0, 0, 1);
	else if(strcmp(argv[1], "-n") == 0){
		analyseur_syntaxique(yyin, 0, 0, 0);
		printf("Fichier assembleur.txt créée\n");
	}
	
  	return 0;
}
