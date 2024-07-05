%{
  #include <stdio.h>
  #include <ctype.h>
  #include <string.h>

  #include "asa.h"
  #include "semantic.h"
  #include "codegen.h"

  extern int yylex();
  static void print_file_error(char * s, char *errmsg);

  table_symb TABLE_SYMBOLES = NULL;
  struct asa * ARBRE_ABSTRAIT = NULL;

  void yyerror(const char * s);

  char srcname[64];
  char exename[64] = "a.out";
  FILE * exefile;
%}

%union{
  int nb;
  char id[32];
  struct asa* arbre;
 };

%define parse.error verbose
%locations

%token VAR
%token PROGRAMME DEBUT FIN ALGO RENVOYER TQ FAIRE FTQ SI SINON ALORS FSI LIRE ECRIRE ALLOUER ET OU NON VRAI FAUX
%token <nb> NB
%token <id> ID

%right FLECHE "<-" '='
%left '>' '<'
%left '+' '-'
%left '*' '/' '%'
%right '('

%start PROGRAMME_ALGO

%type <arbre> EXP
%type <arbre> LIST_INSTS
%type <arbre> INST
%type <arbre> AFFECTATION
%type <arbre> LIST_ID
%type <arbre> FONCTION
%type <arbre> LIST_DECLA_VAR
%type <arbre> DECLA_VAR
%type <arbre> BOUCLE_TQ
%type <arbre> SI_ALORS

%%

SEP : '\n'
| SEP '\n'
;

EOP : %empty
| SEP
;

PROGRAMME_ALGO:
  LIST_DECLA_FCT
  MAIN
;

LIST_DECLA_FCT: %empty      
| ALGORITHME LIST_DECLA_FCT 

ALGORITHME:
  ALGO ID '(' LIST_ID ')' SEP
    LIST_DECLA_VAR
  DEBUT SEP
    LIST_INSTS
  FIN SEP { print_asa($7); print_asa($10); }
;


MAIN:
  PROGRAMME '('')' SEP
    LIST_DECLA_VAR
  DEBUT SEP
    LIST_INSTS
  FIN EOP {semantic($5); semantic($8); print_asa($5); print_asa($8); codegen($5); codegen($8);}
;

LIST_INSTS : INST       { $$ = creer_listINST($1,NULL);}
| INST LIST_INSTS       { $$ = creer_listINST($1,$2);}
;

INST : EXP SEP                  { $$ = $1; }
| AFFECTATION                   { $$ = $1; }
| ECRIRE '(' EXP ')' SEP        { $$ = creer_noeudOP('w', $3, NULL);}
| ALLOUER '(' LIST_ID ')' SEP   { $$ = NULL; }
| BOUCLE_TQ                     { $$ = $1; }
| SI_ALORS                      { $$ = $1; }
| RENVOYER EXP SEP              { $$ = creer_noeudRETURN($2);}
| FONCTION SEP                  { $$ = $1;}
;

BOUCLE_TQ:
  TQ EXP FAIRE SEP
    LIST_INSTS
  FTQ SEP  { $$ = creer_noeudBCL(creer_feuilleID("TQ"), $2, $5); }
;

SI_ALORS:
  SI EXP ALORS SEP
    LIST_INSTS
  FSI SEP  { $$ = creer_noeudBCL(creer_feuilleID("SI"), $2, $5); }
;

AFFECTATION: ID FLECHE EXP SEP    { $$ = creer_noeudaffect(creer_feuilleID($1), $3);}
| ID '[' EXP ']' FLECHE EXP SEP   { $$ = creer_noeudaffect(creer_noeudTAB(creer_feuilleID($1), $3), $6); }
;

LIST_DECLA_VAR : %empty       { }
| DECLA_VAR LIST_DECLA_VAR    { $$ = $1;}
;

DECLA_VAR : VAR LIST_ID SEP   { $$ = $2; }
| VAR ID FLECHE EXP SEP       { $$ = creer_noeudDECLA_VAR(creer_feuilleID($2), creer_noeudaffect(creer_feuilleID($2), $4), NULL);}
;

LIST_ID: ID                        { $$ = creer_noeudDECLA_VAR(creer_feuilleID($1), NULL, NULL);}
| ID ',' LIST_ID                   { $$ = creer_noeudDECLA_VAR(creer_feuilleID($1), NULL, $3);}
| ID FLECHE EXP "," LIST_ID        { $$ = creer_noeudDECLA_VAR(creer_feuilleID($1), $3, $5);}
| ID '[' NB ']'                    { $$ = creer_noeudDECLA_VAR(creer_noeudTAB(creer_feuilleID($1), creer_feuilleNB($3)), NULL, NULL);}
| ID '[' NB ']' FLECHE EXP         { $$ = creer_noeudDECLA_VAR(creer_noeudTAB(creer_feuilleID($1), creer_feuilleNB($3)), $6, NULL);}
| ID '[' NB ']' ',' LIST_ID        { $$ = creer_noeudDECLA_VAR(creer_noeudTAB(creer_feuilleID($1), creer_feuilleNB($3)), NULL, $6);}
| '@' ID                           { $$ = creer_noeudDECLA_VAR(creer_feuilleID($2), NULL, NULL);}
| '@' ID ',' LIST_ID               { $$ = creer_noeudDECLA_VAR(creer_feuilleID($2), NULL, $4);}
| FONCTION                         { $$ = NULL;}
;

FONCTION : ID '(' EXP ')'     { $$ = creer_noeudDECLA_VAR(creer_feuilleID($1), $3, NULL); }
| ID '(' ')'                  { $$ = creer_noeudDECLA_VAR(creer_feuilleID($1), NULL, NULL); }
| ID '(' ID ',' LIST_ID ')'   { $$ = creer_noeudDECLA_VAR(creer_feuilleID($1), creer_feuilleID($3), $5); }
;


EXP : NB                 { $$ = creer_feuilleNB($1); }
| EXP '+' EXP            { $$ = creer_noeudOP('+',$1,$3); }
| EXP '-' EXP            { $$ = creer_noeudOP('-',$1,$3); }
| '-' EXP                { $$ = creer_noeudOP('-',NULL,$2); }
| EXP '*' EXP            { $$ = creer_noeudOP('*',$1,$3); }
| EXP '/' EXP            { $$ = creer_noeudOP('/',$1,$3); }
| EXP '%' EXP            { $$ = creer_noeudOP('%',$1,$3); }
| EXP '<' EXP            { $$ = creer_noeudOP('<',$1,$3); }
| EXP '>' EXP            { $$ = creer_noeudOP('>',$1,$3); }
| EXP '=' EXP            { $$ = creer_noeudOP('=',$1,$3); }
| '(' EXP ')'            { $$ = $2; }
| ID                     { $$ = creer_feuilleID($1);}
| ID '[' EXP ']'         { $$ = creer_noeudTAB(creer_feuilleID($1),$3); }
| LIRE '(' ')'           { $$ = creer_noeudOP('r',NULL,NULL); }
;

%%

int main( int argc, char * argv[] ) {
  extern FILE *yyin;

  if (argc > 1){
    strcpy(srcname, argv[1]);
    if ( (yyin = fopen(srcname,"r"))==NULL ){
      char errmsg[256];
      sprintf(errmsg,"fichier \x1b[1m\x1b[33m' %s '\x1b[0m introuvable",srcname);
      print_file_error(argv[0],errmsg);
      exit(1);
    }
  }  else {
    print_file_error(argv[0],"aucun fichier en entree");
    exit(1);
  }
  if (argc == 3){
    strcpy(exename, argv[2]);
  }
  exefile = fopen(exename,"w");
  TABLE_SYMBOLES = ts_init_table("global");
  yyparse();
  fclose(yyin);
}



static void print_file_error(char * prog, char *errmsg){
  fprintf(stderr,
	  "\x1b[1m%s:\x1b[0m \x1b[31m\x1b[1merreur fatale:\x1b[0m %s\nechec de la compilation\n",
	  prog, errmsg);
}

void yyerror(const char * s)
{
  fprintf(stderr, "\x1b[1m%s:%d:%d:\x1b[0m %s\n", srcname, yylloc.first_line, yylloc.first_column, s);
  exit(0);
}
