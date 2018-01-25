#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symboles.h"
#include "analyseur_lexical.h"
#include "analyseur_syntaxique.h"
#include "util.h"
#include "premiers.h"
#include "suivants.h"
#include "syntabs.h"
#include "affiche_arbre_abstrait.h"
#include "tabsymboles.h"

int uniteCourante;
char buf[128];
char val[128];
FILE* fichier;
int afficheArbre = 0;
int afficheTabSymboles = 0;

int portee = P_VARIABLE_GLOBALE;

int adresseLocaleCourante = 0;
int adresseGlobaleCourante = 0;
int adresseArgumentCourant = 0;

int nbArg = 0;
int nbArguments = 0;
int ebpValArg = 0;

int sinonGlob = 0;

int nbInst = 0; //pour les sauts conditionnels en assembleur

void affichage_syntaxe(int uniteCourante){
	nom_token(uniteCourante, buf, val);
	affiche_element(buf, val, afficheArbre);
}

n_prog *programme(){
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	n_l_dec *var = NULL;
	n_l_dec *fonc = NULL;
	n_prog *$$;
	if(est_premier(uniteCourante, _optDecVariables_) || est_premier(uniteCourante, _listeDecFonctions_)){
		var = optDecVariables();
		fprintf(fichier, "\n%s\t%s\n%s\n%s\n", "section", ".text", "global _start", "_start:");
		fprintf(fichier, "\t\t%s\t%s\n\t\t%s\t%s\n\t\t%s\t%s\n", "call", "main", "mov", "eax, 1", "int", "0x80");
		fonc = listeDecFonctions();
	}
	else if(est_suivant(uniteCourante, _programme_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Un programme commence soit par une déclaration de variable, soit par une fonction");
	}
	$$ = cree_n_prog(var, fonc);
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_l_dec *optDecVariables(){
	n_l_dec *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _listeDecVariables_)){
		$$ = listeDecVariables();
		if(uniteCourante != POINT_VIRGULE){
			erreur("Point virgule manquant");
		}
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
	}
	else if(est_suivant(uniteCourante, _optDecVariables_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Un programme commence soit par une déclaration de variable, soit par une fonction");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_l_dec *listeDecVariables(){	
	n_dec *$2 = NULL;
	n_l_dec *$3 = NULL;
	n_l_dec *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _declarationVariable_)){
		$2 = declarationVariable();
		$3 = listeDecVariablesBis(NULL);
	}
	else{
		erreur("Mauvaise déclaration de variable");
	}
	$$ = cree_n_l_dec($2, $3);
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_l_dec *listeDecVariablesBis(n_l_dec *herite){
	n_dec *$2;
	n_l_dec *$$ = NULL;
	n_l_dec *herite_fils;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	
	if(uniteCourante == VIRGULE){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = declarationVariable();
		herite_fils = cree_n_l_dec($2, herite);
		$$ = listeDecVariablesBis(herite_fils);
	}
	else if (est_suivant(uniteCourante, _listeDecVariablesBis_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
		$$ = herite;
	}
	else{
		erreur("Mauvaise déclaration de variable");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_dec *declarationVariable(){ 
	char *nom = NULL;
	int taille = 0;
	n_dec *$$ = NULL;
	adresseArgumentCourant++;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante != ENTIER){
		erreur("Mauvaise déclaration de variable");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	if(uniteCourante != ID_VAR){
		erreur("Mauvaise déclaration de variable");
	}
	//nom_token(uniteCourante, buf, val);
	affichage_syntaxe(uniteCourante);
	uniteCourante = yylex();	
	nom = duplique_chaine(val);
    if(rechercheDeclarative(nom) != -1){
        erreur("Impossible de déclarer une variable locale ayant le même nom qu'un argument, ou qu'une autre variable locale");
    }
	taille = optTailleTableau();
	if(taille == 0){
		$$ = cree_n_dec_var(nom);
        ajouteIdentificateur(nom, portee, T_ENTIER, adresseLocaleCourante, 1);
        adresseLocaleCourante+=4;
        if(portee == P_VARIABLE_GLOBALE)
        	parcours_dec(nom, 1);
	}
	else{
		$$ = cree_n_dec_tab(nom, taille);
        if(portee != P_VARIABLE_GLOBALE)
            erreur("Un tableau est toujours une variable globale");
        ajouteIdentificateur(nom, portee, T_TABLEAU_ENTIER, adresseLocaleCourante, taille);
        adresseLocaleCourante += 4*taille;
        parcours_dec(nom, taille);
    }
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

int optTailleTableau(){
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	int taille = 0;
	if(uniteCourante == CROCHET_OUVRANT){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		if(uniteCourante != NOMBRE){
			erreur("Mauvaise déclaration de variable");
		}
		//nom_token(uniteCourante, buf, val);
		affichage_syntaxe(uniteCourante);
		uniteCourante = yylex();	
		taille = atoi(val);
		if(uniteCourante != CROCHET_FERMANT){
			erreur("Mauvaise déclaration de variable");
		}
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
	}
	else if(est_suivant(uniteCourante, _optTailleTableau_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
		taille = 0;
	}
	else{
		erreur("Mauvaise déclaration de variable");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return taille;
}

n_l_dec *listeDecFonctions(){
	n_dec *$2 = NULL;
	n_l_dec *$3 = NULL;
	n_l_dec *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _declarationFonction_)){
		$2 = declarationFonction();
		$3 = listeDecFonctions();
		$$ = cree_n_l_dec($2, $3);
	}
	else if(est_suivant(uniteCourante, _listeDecFonctions_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Mauvaise déclaration de fonction");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_dec *declarationFonction(){
    portee = P_VARIABLE_LOCALE;
	char *nom = NULL;
	n_l_dec *param = NULL;
	n_l_dec *vars = NULL;
	n_instr *corps = NULL;
	n_dec *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante != ID_FCT){
		erreur("Mauvaise déclaration de fonction");
	}
	//nom_token(uniteCourante, buf, val);
	affichage_syntaxe(uniteCourante);
	uniteCourante = yylex();
	nom = duplique_chaine(val);
	if(rechercheDeclarative(nom) != -1){
		exit(-1);
	}
	int i = ajouteIdentificateur(nom, P_VARIABLE_GLOBALE, T_FONCTION, 0, 0);
	entreeFonction();
	param = listeParam();
	int nbParam = adresseArgumentCourant;
	tabsymboles.tab[i].complement = nbParam;
	nbArguments = nbParam;
	ebpValArg = 8 + 4 * nbParam;
	
	uniteCourante = yylex();
	adresseLocaleCourante = 0;
	vars = optDecVariables();
	
	/*******************/
	
	
	fprintf(fichier, "%s:\n", nom);
	fprintf(fichier, "\t\tpush\tebp\n");
	fprintf(fichier, "\t\tmov\t\tebp, esp\n");
	
	/* TRAITEMENT DES Variables */

	int nbLoc = 1;
	if(vars != NULL){
		n_l_dec *tet = vars;
		while(tet->queue != NULL){
			tet = tet->queue;
			++nbLoc;
		}
		fprintf(fichier, "\t\tsub\t\tesp, %d\t;variables locales\n", 4 * nbLoc);
	}

	
	/********************/
	corps = instructionBloc();
    if((strcmp("main", nom) == 0) && nbParam != 0){
    	printf("param %d", nbParam);
    	erreur("La fonction main ne doit pas avoir de parametres");
    }
	if(afficheTabSymboles == 1)
    	afficheTabsymboles();
	sortieFonction();
	$$ = cree_n_dec_fonc(nom, param, vars, corps);
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
    portee = P_VARIABLE_GLOBALE;
    
    /*******************/
    
    if(vars != NULL){
    	fprintf(fichier, "\t\tadd\t\tesp, %d\t;desalloue variables locales\n", 4 * nbLoc);
    }
    if(nbParam != 0){
    	int ebpValArg = 8 + 4 * nbParam;
		fprintf(fichier, "\t\tpop\t\teax;retour\n");
		fprintf(fichier, "\t\tmov\t\t[ebp + %d], eax\t;retour\n", ebpValArg);
    }
    
    fprintf(fichier, "\t\tpop\tebp\n");
    fprintf(fichier, "\t\t%s", "ret\n");
    
    /*******************/
	return $$;
}

n_l_dec *listeParam(){
    portee = P_ARGUMENT;
    adresseArgumentCourant = 0;
	n_l_dec *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante != PARENTHESE_OUVRANTE){
		erreur("Parenthese manquante");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	$$ = optListeDecVariables();
	if(uniteCourante != PARENTHESE_FERMANTE){
		erreur("Parenthese manquante");
	}
	nom_token(uniteCourante, buf, val);
	affiche_element(buf, val, afficheArbre);
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
    portee = P_VARIABLE_LOCALE;
	return $$;
}

n_l_dec *optListeDecVariables(){
	n_l_dec *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _listeDecVariables_)){
		$$ = listeDecVariables();
	}
	else if(est_suivant(uniteCourante, _optListeDecVariables_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Mauvaise déclaration de variable");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_instr *instruction(){
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante == FAIRE){
		$$ = instructionFaire();
	}
	else if(est_premier(uniteCourante, _instructionAffect_)){
		$$ = instructionAffect();
	}
	else if(est_premier(uniteCourante, _instructionBloc_)){
		$$ = instructionBloc();
	}
	else if(est_premier(uniteCourante, _instructionSi_)){
		$$ = instructionSi();
	}
	else if(est_premier(uniteCourante, _instructionTantque_)){
		$$ = instructionTantque();
	}
	else if(est_premier(uniteCourante, _instructionAppel_)){
		$$ = instructionAppel();
	}
	else if(est_premier(uniteCourante, _instructionRetour_)){
		$$ = instructionRetour();
	}
	else if(est_premier(uniteCourante, _instructionEcriture_)){
		$$ = instructionEcriture();
	}
	else if(est_premier(uniteCourante, _instructionVide_)){
		$$ = instructionVide();
	}
	else if(est_premier(uniteCourante, _instructionIncr_)){
		$$ = instructionIncr();
	}
	else{
		erreur("Instruction invalide");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	parcours_instr($$);	
	return $$;
}

n_instr *instructionFaire(){
	n_instr *$2 = NULL;
	n_exp *$3 = NULL;
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	$2 = instructionBloc();
	if(uniteCourante != TANTQUE){
		erreur("tantque manquant");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	$3 = expression();
	if(uniteCourante != POINT_VIRGULE){
		erreur("point virgule manquant");
	}
	$$ = cree_n_instr_faire($2, $3);
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_instr *instructionAffect(){
	n_var *$2 = NULL;
	n_exp *$3 = NULL;
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _var_)){
		$2 = var();
	}
    int i = rechercheExecutable($2->nom);
    if(i == -1){
        erreur("Variable utilisée mais non déclarée");
    }
	if(tabsymboles.tab[i].type == T_TABLEAU_ENTIER && $2->type == 0){
		erreur("Un tableau ne doit jamais être utilisé sans indice");
	}
	else if(tabsymboles.tab[i].type == T_ENTIER && $2->type == 1){
		erreur("Une variable entiere ne doit jamais être indicée");
	}
	if(uniteCourante != EGAL){
		erreur("Symbole '=' manquant");
	}
	affichage_syntaxe(uniteCourante);
	uniteCourante = yylex();	
	$3 = expression();
	if(uniteCourante != POINT_VIRGULE){
		erreur("point virgule manquant");
	}
	$$ = cree_n_instr_affect($2, $3);
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_instr *instructionBloc(){
	n_l_instr *$2 = NULL;
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante != ACCOLADE_OUVRANTE){
		erreur("Symbole '{' manquant");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	$2 = listeInstructions();
	if(uniteCourante != ACCOLADE_FERMANTE){
		erreur("Symbole '}' manquant");
	}
	$$ = cree_n_instr_bloc($2);
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_l_instr *listeInstructions(){
	n_instr *$2 = NULL;
	n_l_instr *$3 = NULL;
	n_l_instr *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _instruction_)){
		$2 = instruction();
		$3 = listeInstructions();
		$$ = cree_n_l_instr($2, $3);
	}
	else if(est_suivant(uniteCourante, _listeInstructions_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Instruction incorrecte");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_instr *instructionSi() {
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	n_instr *$$ = NULL;
	n_exp *$2 = NULL;
	n_instr *$4 = NULL;
	n_instr *$5 = NULL;
	if(uniteCourante != SI){
		erreur("si manquant");
	}
	affichage_syntaxe(uniteCourante);
	uniteCourante = yylex();
	$2 = expression();
	if(uniteCourante != ALORS){
		erreur("alors manquant");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex();
	fprintf(fichier, "inst%d:\n", nbInst);
	++nbInst;
	$4 = instructionBloc();
	fprintf(fichier, "\t\tjmp\t\tinst%d\n", nbInst+2);
	fprintf(fichier, "inst%d:\n", nbInst);
	++nbInst;
	$5 = optSinon();
	fprintf(fichier, "\t\tjmp\t\tinst%d\n", nbInst);
	$$ = cree_n_instr_si($2, $4, $5);
	fprintf(fichier, "inst%d:\n", nbInst);
	++nbInst;
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_instr *optSinon(){
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	n_instr *$$ = NULL;
	if(uniteCourante == SINON){
        affichage_syntaxe(uniteCourante);
    	fprintf(fichier, "\t\tjmp\t\tinst%d;sinon\n", nbInst);
    	fprintf(fichier, "inst%d:\t\t;sinon\n", nbInst);
    	++nbInst;
       	uniteCourante = yylex(); 
		$$ = instructionBloc();
	}
	else if(est_suivant(uniteCourante, _optSinon_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Instruction SI incorrecte");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_instr *instructionTantque(){
	n_exp *$2 = NULL;
	n_instr *$3 = NULL;
	n_instr *$$;
	fprintf(fichier, "inst%d:\n", nbInst+2);
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante == TANTQUE){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = expression();
		if(uniteCourante == FAIRE){
			affichage_syntaxe(uniteCourante);
			uniteCourante = yylex();
			fprintf(fichier, "inst%d:\n", nbInst);
			$3 = instructionBloc();
			fprintf(fichier, "\t\tjmp\t\tinst%d\n", nbInst+2);
			fprintf(fichier, "inst%d:\n", nbInst+1);
		}
		else{
			erreur("faire manquant");
		}
	}
	else{
		erreur("tantque manquant");
	}
	$$ = cree_n_instr_tantque($2, $3);
	affiche_balise_fermante(__FUNCTION__, afficheArbre);	
	return $$;
}

n_instr *instructionAppel(){
	n_appel *$2 = NULL;
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _appelFct_)){
		$2 = appelFct();
		if(uniteCourante != POINT_VIRGULE){
			erreur("point virgule manquant");
		}
		nom_token(uniteCourante, buf, val);
		affiche_element(buf, val, afficheArbre);		
	}
	else{
		erreur("Instruction d'appel incorrecte");
	}
	$$ = cree_n_instr_appel($2);
	uniteCourante = yylex();
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_instr *instructionRetour(){
	n_exp *$2 = NULL;
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante != RETOUR){
		erreur("retour manquant");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	$2 = expression();
	if(uniteCourante != POINT_VIRGULE){
		erreur("point virgule manquant");
	}
	$$ = cree_n_instr_retour($2);
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	
	fprintf(fichier, "\t\tpop\t\teax;retour\n");
	fprintf(fichier, "\t\tmov\t\t[ebp + %d], eax\t;retour\n", ebpValArg);
    fprintf(fichier, "\t\tpop\tebp\n");
    fprintf(fichier, "\t\t%s", "ret\n");	
	return $$;
}

n_instr *instructionEcriture(){
	n_exp *$2 = NULL;
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante != ECRIRE){
		erreur("ecrire manquant");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	if(uniteCourante != PARENTHESE_OUVRANTE){
		erreur("Symbole '(' manquant");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	$2 = expression();
	if(uniteCourante != PARENTHESE_FERMANTE){
		erreur("Symbole ')' manquant");
	}
	affichage_syntaxe(uniteCourante);
	uniteCourante = yylex();	
	if(uniteCourante != POINT_VIRGULE){
		erreur("point virgule manquant");
	}
	$$ = cree_n_instr_ecrire($2);
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_instr *instructionIncr(){	
	n_exp *$2 = NULL;
	n_instr *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante != INCR){
		erreur("incr manquant");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	$2 = expression();
	if(uniteCourante != POINT_VIRGULE){
		erreur("point virgule manquant");
	}
	$$ = cree_n_instr_incr($2);
	fprintf(fichier, "\t\tpop eax\n");
	fprintf(fichier, "\t\tadd eax, 1\n");
	
	int z = rechercheExecutable($$->u.incr->u.var->nom);
	int adr = 4 + tabsymboles.tab[z].adresse;
	switch(tabsymboles.tab[z].portee){
		case 1:
			fprintf(fichier, "\t\tmov\t\t[%s], eax\n", $$->u.incr->u.var->nom);
			break;
		case 2:
			fprintf(fichier, "\t\tmov\t\t[ebp - %d], eax\n", adr);
			break;
		case 3:
			break;
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}


n_instr *instructionVide(){
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	n_instr *$$ = NULL;
	if(uniteCourante != POINT_VIRGULE){
		erreur("point virgule manquant");
	}
    affichage_syntaxe(uniteCourante);
   	uniteCourante = yylex(); 
	$$ = cree_n_instr_vide();
	affiche_balise_fermante(__FUNCTION__, afficheArbre);	
	return $$;
}

n_exp *expression(){
	n_exp *$2 = NULL;
	n_exp *$$ = NULL;
	adresseArgumentCourant++; 
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _conjonction_)){
		$2 = conjonction();
		$$ = expressionBis($2);
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	parcours_exp($$);
	return $$;
}

n_exp *expressionBis(n_exp *herite){
	n_exp *$2;
	n_exp *$$ = NULL;
	n_exp *herite_fils;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante == OU){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = conjonction();
		herite_fils = cree_n_exp_op(ou, herite, $2);
		$$ = expressionBis(herite_fils);
	}
	else if(est_suivant(uniteCourante, _expressionBis_)){
		$$ = herite;
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Expression malformée");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp *conjonction(){
	n_exp *$2 = NULL;
	n_exp *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _comparaison_)){
		$2 = comparaison();
		$$ = conjonctionBis($2);
	}
	else{
		erreur("Expression malformée");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp *conjonctionBis(n_exp *herite){
	n_exp *$2;
	n_exp *$$ = NULL;
	n_exp *herite_fils;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante == ET){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = comparaison();
		herite_fils = cree_n_exp_op(et, herite, $2);
		$$ = conjonctionBis(herite_fils);
	}
	else if(est_suivant(uniteCourante, _conjonctionBis_)){
		$$ = herite;
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Expression malformée");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp *comparaison(){
	n_exp *$2 = NULL;
	n_exp *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _expArith_)){
		$2 = expArith();
		$$ = comparaisonBis($2);
	}
	else{
		erreur("Expression malformée");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp *comparaisonBis(n_exp *herite){
	n_exp *$2;
	n_exp *$$;
	n_exp *herite_fils;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante == EGAL){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = expArith();
		herite_fils = cree_n_exp_op(egal, herite, $2);
		$$ = comparaisonBis(herite_fils);
	}
	else if(uniteCourante == INFERIEUR){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = expArith();
		herite_fils = cree_n_exp_op(inf, herite, $2);
		$$ = comparaisonBis(herite_fils);
	}
	else if(est_suivant(uniteCourante, _comparaisonBis_)){
		$$ = herite;
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Expression malformée");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp *expArith(){
	n_exp *$2 = NULL;
	n_exp *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);

	if(est_premier(uniteCourante, _terme_)){
		$2 = terme();
		$$ = expArithBis($2);
	}
	else{
		erreur("Expression malformée");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp *expArithBis(n_exp *herite){
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	n_exp *$2;
	n_exp *$$;
	n_exp *herite_fils;
	if(uniteCourante == PLUS){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = terme();
		herite_fils = cree_n_exp_op(plus, herite, $2);
		$$ = expArithBis(herite_fils);
	}
	else if(uniteCourante == MOINS){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = terme();
		herite_fils = cree_n_exp_op(moins, herite, $2);
		$$ = expArithBis(herite_fils);
	}
	else if(est_suivant(uniteCourante, _expArithBis_)){
		$$ = herite;
	}
	else{
		erreur("Expression malformée");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;	
}

n_exp *terme(){
	n_exp *$2;
	n_exp *$$;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _negation_)){
		$2 = negation();
		$$ = termeBis($2);
	}
	else{
		erreur("Expression malformée");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp *termeBis(n_exp *herite){
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	n_exp *$2;
	n_exp *$$;
	n_exp *herite_fils;
	if(uniteCourante == FOIS){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = negation();
		herite_fils = cree_n_exp_op(fois, herite, $2);
		$$ = termeBis(herite_fils);
	}
	else if(uniteCourante == DIVISE){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = negation();
		herite_fils = cree_n_exp_op(divise, herite, $2);
		$$ = termeBis(herite_fils);
	}
	/*else if(uniteCourante == MODULO){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = negation();
		herite_fils = cree_n_exp_op(modulo, herite, $2);
		$$ = termeBis(herite_fils);
	}*/
	else if(est_suivant(uniteCourante, _termeBis_)){
		$$ = herite;
	}
	else{
		erreur("Expression malformée");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp * negation(){
	n_exp *$2;
	n_exp *$$;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante == NON){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$2 = negation();
		$$ = cree_n_exp_op(non, $2, NULL);
	}
	else if (est_premier(uniteCourante, _facteur_)){
		$$ = facteur();
	}
	else{
		erreur("Expression malformée : ni une négation ni un facteur");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp *facteur(){
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	n_var *$3 = NULL;
	n_appel *$4 = NULL;
	int nombre;
	n_exp *$$;
	if(uniteCourante == PARENTHESE_OUVRANTE){
		affichage_syntaxe(uniteCourante);
		uniteCourante = yylex();	
		$$ = expression();
		if(uniteCourante != PARENTHESE_FERMANTE){
			erreur("Symbole ')' manquant");
		}
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
	}
	else if (uniteCourante == NOMBRE){
		affichage_syntaxe(uniteCourante);
		//nom_token(uniteCourante, buf, val);
		uniteCourante = yylex();	
		nombre = atoi(val);
		$$ = cree_n_exp_entier(nombre);
	}
	else if(uniteCourante == LIRE){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		if(uniteCourante != PARENTHESE_OUVRANTE){
			erreur("Symbole '(' manquant");
		}
		affichage_syntaxe(uniteCourante);
		uniteCourante = yylex();	
		if(uniteCourante != PARENTHESE_FERMANTE){
			erreur("Symbole ')' manquant");
		}
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex();         
		$$ = cree_n_exp_lire();
	}
	else if(est_premier(uniteCourante, _var_)){
		$3 = var();
		int i = rechercheExecutable($3->nom);
		if(i == -1){
			erreur("Variable utilisée mais non déclarée");
		}
		if(tabsymboles.tab[i].type == T_TABLEAU_ENTIER && $3->type == 0){
			erreur("Un tableau ne doit jamais être utilisé sans indice");
		}
		else if(tabsymboles.tab[i].type == T_ENTIER && $3->type == 1){
			erreur("Une variable entiere ne doit jamais être indicée");
		}
		$$ = cree_n_exp_var($3);
	}
	else if(est_premier(uniteCourante, _appelFct_)){
		$4 = appelFct();
		$$ = cree_n_exp_appel($4);
	}
	else{
		erreur("Facteur malformé");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_var *var(){
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	char *nom = NULL;
	n_exp *indice = NULL;
	n_var *$$;
	if(uniteCourante != ID_VAR){
		erreur("Une variable doit avoir un identifiant");
	}
	affichage_syntaxe(uniteCourante);
	//nom_token(uniteCourante, buf, val);
	uniteCourante = yylex();	
	nom = duplique_chaine(val);
	indice = optIndice();
	
	if(indice == NULL){
		$$ = cree_n_var_simple(nom);
	}
	else{
		$$ = cree_n_var_indicee(nom, indice);
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_exp *optIndice(){
	n_exp *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante == CROCHET_OUVRANT){
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 
		$$ = expression();
		if($$ == NULL){
			erreur("Indice de tableau manquant");
		}
		if(uniteCourante != CROCHET_FERMANT){
			erreur("Symbole ']' manquant");
		}
        affichage_syntaxe(uniteCourante);
       	uniteCourante = yylex(); 	
	}
	else if (est_suivant(uniteCourante, _optIndice_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else{
		erreur("Erreur déclaration variable");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_appel *appelFct(){
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	fprintf(fichier, "\t\tsub\t\tesp, 4\t\t;valeur retour\n");
	char *fonction = NULL;
	n_l_exp *$2 = NULL;
	n_appel *$$ = NULL;
	//adresseArgumentCourant = 0;
	if(uniteCourante == ID_FCT){
        affichage_syntaxe(uniteCourante);
		//nom_token(uniteCourante, buf, val);
		uniteCourante = yylex();	
		fonction = duplique_chaine(val);
		if(uniteCourante == PARENTHESE_OUVRANTE){
			affichage_syntaxe(uniteCourante);
			uniteCourante = yylex();
			adresseArgumentCourant = 0;
			$2 = listeExpressions();
			if(uniteCourante != PARENTHESE_FERMANTE){
				erreur("Symbole ')' manquant");
			}
			affichage_syntaxe(uniteCourante);
			uniteCourante = yylex();
		}
		else{
			erreur("Symbole '(' manquant");
		}
		$$ = cree_n_appel(fonction, $2);
		int i = rechercheExecutable($$->fonction);
		if(i == -1){
			erreur("Fonction utilisée mais non déclarée");
		}
		else{
			nbArg = 0;
			if($$->args != NULL){
				++nbArg;
				n_l_exp *tet = $$->args;
				while(tet->queue != NULL){
					tet = tet->queue;
					++nbArg;
				}
			}
			if(nbArg != tabsymboles.tab[i].complement){
				erreur("Mauvaise nombre d'arguments");
			}
		}
	}
	else if (est_suivant(uniteCourante, _appelFct_)){
		affiche_balise_fermante(__FUNCTION__, afficheArbre);
		return $$;
	}
	else{
		erreur("Appel incorrect à une fonction");
	}
	//printf("arg : %d", nbArg);
	int nbSub = 4 * nbArg;
	fprintf(fichier, "\t\tcall\t%s\n", $$->fonction);
	if(nbArg > 0){
		fprintf(fichier, "\t\tadd\t\tesp, %d\n", nbSub);
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_l_exp *listeExpressions(){
	n_exp *$2 = NULL;
	n_l_exp *$3 = NULL;
	n_l_exp *$$ = NULL;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(est_premier(uniteCourante, _expression_)){
		$2 = expression();
		$3 = listeExpressionsBis(NULL);
		$$ = cree_n_l_exp($2, $3);
	}
	else if (est_suivant(uniteCourante, _listeExpressions_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
	}
	else
		erreur("Erreur expression");
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

n_l_exp *listeExpressionsBis(n_l_exp *herite){
	n_exp *$2;
	n_l_exp *$$ = NULL;
	n_l_exp *herite_fils;
	affiche_balise_ouvrante(__FUNCTION__, afficheArbre);
	if(uniteCourante == VIRGULE){
		affichage_syntaxe(uniteCourante);
		uniteCourante = yylex();	
		$2 = expression();
		herite_fils = cree_n_l_exp($2, herite);
		$$ = listeExpressionsBis(herite_fils);
	}
	else if (est_suivant(uniteCourante, _listeExpressionsBis_)){
		//affiche_balise_fermante(__FUNCTION__, afficheArbre);
		$$ = herite;
	}
	else{
		erreur("Erreur expression");
	}
	affiche_balise_fermante(__FUNCTION__, afficheArbre);
	return $$;
}

void analyseur_syntaxique(FILE *yyin, int arbreSynt, int arbreAsynt, int tabSymboles) {
	afficheArbre = arbreSynt;
	afficheTabSymboles = tabSymboles;
	initialise_premiers();
	initialise_suivants();
	uniteCourante = yylex();
	fichier = fopen("assembleur.txt", "w");
    if (fichier == NULL)
        exit(-1);
    fprintf(fichier, "%s\t\t%s\n\n", "%include", "'io.asm'");
	fprintf(fichier, "%s\t%s\n", "section", ".bss");
	fprintf(fichier, "%s\t%s\t%s\n", "sinput:", "resb", "255");
    n_prog *prog = programme();
    if(arbreAsynt == 1)
    	affiche_n_prog(prog);
    fclose(fichier);
}

void parcours_dec(char *nom, int size){
	fprintf(fichier, "%s\t%s\t%d\n", nom, "resw", 4*size);
}


void parcours_instr(n_instr* instr){
	switch (instr->type){
		case 0:			//incrInst
			break;
		case 1:			//affecteInst
			fprintf(fichier, "\t\tpop\t\teax\n");
			traitementVariableAffect(instr->u.affecte_.var);
			break;
		case 2:			//siInst
			break;
		case 3:			//faireInst
			break;
		case 4:			//tantqueInst
			break;
		case 5:			//appelInst
			break;
		case 6:			//retourInst
			break;
		case 7:			//ecrireInst
			fprintf(fichier, "\t\tpop\t\teax\n");
			fprintf(fichier, "\t\tcall\tiprintLF\n");
			break;
		case 8:			//videInst
			break;
		case 9:			//blocInst
			break;
	}
}


void parcours_exp(n_exp* exp){
	switch (exp->type){
		case 0:			//variable
			traitementVariableExp(exp->u.var);		
			break;
		case 1:			//operation
			operationGeneration(exp);
			break;
		case 2:			//constante
			fprintf(fichier, "\t\t%s\t%d;constante\n", "push", exp->u.entier);
			break;
		case 3:			//appel
			break;
		case 4:			//lire
			fprintf(fichier, "\t\t%s\t\t%s%s;lecture\n", "mov", "eax, ", "sinput");
			fprintf(fichier, "\t\t%s\t%s;lecture\n", "call", "readline");
			fprintf(fichier, "\t\t%s\t%s;lecture\n", "call", "atoi");
			fprintf(fichier, "\t\t%s\t%s;lecture\n", "push", "eax");
			break;
	}
}

void traitementVariableExp(n_var* var){

	int i = rechercheExecutable(var->nom);
	int portee = tabsymboles.tab[i].portee;
	int adr = 4 + tabsymboles.tab[i].adresse;
	int ebpVal;
	
	switch(portee){
		case 1:			//global
			fprintf(fichier, "\t\tmov\t\teax, [%s];traitement exp\n", var->nom);
			fprintf(fichier, "\t\tpush\teax\n");
			break;
		case 2:			//local
			fprintf(fichier, "\t\tmov\t\teax, [ebp - %d]\n", adr);
			fprintf(fichier, "\t\tpush\teax\n");
			break;
		case 3:			//parametre
			ebpVal = 4 + 4 * nbArguments - tabsymboles.tab[i].adresse;
			fprintf(fichier, "\t\tmov\t\tebx, [ebp + %d]\t;parametre\n", ebpVal);
			fprintf(fichier, "\t\tpush\tebx\t;parametre\n");	
			break;
	}
}

void traitementVariableAffect(n_var* var){

	int i = rechercheExecutable(var->nom);
	int portee = tabsymboles.tab[i].portee;
	int adr = 4 + tabsymboles.tab[i].adresse;
	
	switch(portee){
		case 1:			//global
			fprintf(fichier, "\t\tmov\t\t[%s], eax;affectVar\n", var->nom);
			fprintf(fichier, "\t\tmov\t\teax, [%s];affectVar\n", var->nom);
			break;
		case 2:			//local
			fprintf(fichier, "\t\tmov\t\t[ebp - %d], eax\n", adr);
			fprintf(fichier, "\t\tmov\t\teax, [ebp - %d]\n", adr);
			break;
		case 3:			//parametre
			printf("param2\n");		
			break;
	}
}

void operationGeneration(n_exp* exp){
	parcours_exp(exp->u.opExp_.op1);
	parcours_exp(exp->u.opExp_.op2);
	fprintf(fichier, "\t\tpop\t\tebx\n");
	fprintf(fichier, "\t\tpop\t\teax\n");
	switch (exp->u.opExp_.op){
		case 0:			//PLUS
			fprintf(fichier, "\t\tadd\t\teax, ebx\n");
			fprintf(fichier, "\t\tpush\teax\n");
			break;
		case 1:			//MOINS
			fprintf(fichier, "\t\tsub\t\teax, ebx\n");
			fprintf(fichier, "\t\tpush\teax\n");
			break;		
		case 2:			//FOIS
			fprintf(fichier, "\t\timul\teax, ebx ;multiplication\n");	
			fprintf(fichier, "\t\tpush\teax\n");	
			break;
		case 3:			//DIVISE
			fprintf(fichier, "\t\tidiv\t\tebx\n");
			fprintf(fichier, "\t\tpush\teax\n");
			break;
		case 4:			//MODULO
			fprintf(fichier, "\t\tidiv\t\tebx\n");
			fprintf(fichier, "\t\tpush\t\tedx\n");
			fprintf(fichier, "\t\tpop\t\teax\n");
			fprintf(fichier, "\t\tpush\teax\n");
			break;
		case 5:			//EGAL
			fprintf(fichier, "\t\tcmp\t\teax, ebx\n");
			fprintf(fichier, "\t\tje\t\tinst%d\n", nbInst);
			fprintf(fichier, "\t\tjmp\t\tinst%d\n", nbInst+1);
			break;
		case 6:			//DIFF
			fprintf(fichier, "\t\tcmp\t\teax, ebx\n");
			fprintf(fichier, "\t\tjne\t\tinst%d\n", nbInst);
			break;
		case 7:			//INF
			fprintf(fichier, "\t\tcmp\t\teax, ebx\n");
			fprintf(fichier, "\t\tjl\t\tinst%d\n", nbInst);
			//fprintf(fichier, "\t\tpush\t0\n");
			fprintf(fichier, "\t\tjmp\t\tinst%d\n", nbInst+1);
			break;
		case 8:			//SUP
			fprintf(fichier, "\t\tcmp\t\teax, ebx\n");
			fprintf(fichier, "\t\tjg\t\tinst%d\n", nbInst);
			break;
		case 9:			//INFEG
			fprintf(fichier, "\t\tcmp\t\teax, ebx\n");
			fprintf(fichier, "\t\tjle\t\tinst%d\n", nbInst);
			break;
		case 10:		//SUPEG
			fprintf(fichier, "\t\tcmp\t\teax, ebx\n");
			fprintf(fichier, "\t\tjge\t\tinst%d\n", nbInst);			
			break;
		case 11:		//OU
			fprintf(fichier, "\t\tor\t\teax, ebx\n");
			break;
		case 12:		//ET
			fprintf(fichier, "\t\tand\t\teax, ebx\n");
			break;
		case 13:		//NON
			fprintf(fichier, "\t\txor\t\teax, ebx\n"); //pas sur
			break;
		case 14:		//NEGATIF
			fprintf(fichier, "\t\tnot\t\teax\n");
			break;
	}
}
