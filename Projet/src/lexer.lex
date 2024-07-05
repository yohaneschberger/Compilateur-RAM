%{
  #include <string.h>
  #include "parser.h"

  extern void yyerror(char *);

  char errmsg[256]="";
  const char charerr[] = "\x1b[1m\x1b[31m[erreur lexicale]\x1b[0m caractère \x1b[41m%c\x1b[0m inattendu";

  /* MACRO défini
   * Action executee avant chaque action semantique (meme vide)
   * et rempli la variable Bison `yylloc` avec la position du token
   */
#define YY_USER_ACTION                                             \
  yylloc.first_line = yylloc.last_line;                            \
  yylloc.first_column = yylloc.last_column;                        \
  if (yylloc.last_line == yylineno)                                \
    yylloc.last_column += yyleng;                                  \
  else {                                                           \
    yylloc.last_line = yylineno;                                   \
    yylloc.last_column = 1;					   \
  }



%}

%option nounput
%option noinput
%option yylineno

CHIFFRE  [0-9]
NOMBRE   [0-9]+
IDENT    [a-zA-Z]+

%%

{NOMBRE}        { yylval.nb = atoi(yytext); return NB; }
[-*+/=%\n<>@]   { return *yytext; }
"PROGRAMME"     { return PROGRAMME; }
"DEBUT"         { return DEBUT; }
"VAR"           { return VAR; }
"<-"            { return FLECHE; }
"FIN"           { return FIN; }
"ALGO"          { return ALGO; }
"RENVOYER"      { return RENVOYER; }
"TQ"            { return TQ; }
"ALORS"         { return ALORS; }
"SI"            { return SI; }
"SINON"         { return SINON; }
"FAIRE"         { return FAIRE; }
"FTQ"           { return FTQ; }
"FSI"           { return FSI; }
"LIRE"          { return LIRE; }
"ECRIRE"        { return ECRIRE; }
"ALLOUER"       { return ALLOUER; }
"ET"            { return ET; }
"OU"            { return OU; }
"NON"           { return NON; }
"VRAI"          { return VRAI; }
"FAUX"          { return FAUX; }
","             { return *yytext; }
"["             { return *yytext; }
"]"             { return *yytext; }
"("             { return *yytext; }
")"             { return *yytext; }
"//".*"\n"      { /* ignorer les commentaires */ }
{IDENT}         { strcpy(yylval.id, yytext); return ID; }
[ \t]           { /* ignorer les blancs */ }

.         {
	    sprintf(errmsg,charerr, yytext[0]);
            yyerror(errmsg);
            return 1;
          }

%%
