#ifndef __ANALYSEUR_SYNTAXIQUE__
#define __ANALYSEUR_SYNTAXIQUE__

#include <stdio.h>
#include "stdlib.h"
#include "syntabs.h"
#include "affiche_arbre_abstrait.h"

void analyseur_syntaxique(FILE *yyin, int arbreSynt, int arbreAsynt, int tabSymboles);
void parcours_dec(char *nom, int taille);
void parcours_exp(n_exp* exp);
void parcours_instr(n_instr* instr);
void operationGeneration(n_exp* exp);
void traitementVariableAffect(n_var* var);
void traitementVariableExp(n_var* var);

n_prog *programme();
n_l_dec *optDecVariables();
n_l_dec *listeDecVariables();
n_l_dec *listeDecVariablesBis(n_l_dec *herite);
n_dec *declarationVariable();
int optTailleTableau();
n_l_dec *listeDecFonctions();
n_dec *declarationFonction();
n_l_dec *listeParam();
n_l_dec *optListeDecVariables();
n_instr *instruction();
n_instr *instructionFaire();
n_instr *instructionAffect();
n_instr *instructionBloc();
n_l_instr *listeInstructions();
n_instr *instructionSi();
n_instr *optSinon();
n_instr *instructionTantque();
n_instr *instructionAppel();
n_instr *instructionRetour();
n_instr *instructionEcriture();
n_instr *instructionIncr();
n_instr *instructionVide();
n_exp *expression();
n_exp *expressionBis(n_exp *herite);
n_exp *conjonction();
n_exp *conjonctionBis(n_exp *herite);
n_exp *comparaison();
n_exp *comparaisonBis(n_exp *herite);
n_exp *expArith();
n_exp *expArithBis(n_exp *herite);
n_exp *terme();
n_exp *termeBis(n_exp *herite);
n_exp *negation();
n_exp *facteur();
n_var *var();
n_exp *optIndice();
n_appel *appelFct();
n_l_exp *listeExpressions();
n_l_exp *listeExpressionsBis(n_l_exp *herite);

#endif
