#ifndef ASA_H
#define ASA_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define  MAX_SIZE_FIELD_NAME  12
#define  MAX_SIZE_FIELD_VAL   32
#define  TXT_RED    "\x1b[31m"
#define  TXT_GREEN  "\x1b[32m"
#define  TXT_BLUE   "\x1b[34m"
#define  TXT_BOLD   "\x1b[1m"
#define  TXT_NULL   "\x1b[0m"

#define TYPE_ENTIER  'e'
#define TYPE_TABLEAU 't'
#define TYPE_PTR     'p'
#define TYPE_FCT     'f'

typedef enum {typeNB, typeOP, typeID, typeDEC, typeLISTINST, typeAFFECT, typeLISTDEC, typeTAB, typeBCL, typeRETURN} typeNoeud;

typedef struct {
  int val;
} feuilleNb;

typedef struct {
  int ope;
  struct asa * noeud[2];
} noeudOp;

typedef struct {
  char id[32];
}feuilleId;

typedef struct {
  struct asa* inst;
  struct asa* suite;
}noeudLISTINSTS;

typedef struct {
  struct asa * id;
  struct asa * exp;
  struct asa * decla_suiv;
}noeudDECLA_VAR;

typedef struct {
  struct asa* dec;
  struct asa* suite;
}noeudLISTDEC;

typedef struct {
  struct asa * id;
  struct asa * exp;
}noeudAFFECT;

typedef struct {
  struct asa * id;
  struct asa * exp;
}noeudTAB;

typedef struct {
  struct asa * id;
  struct asa * exp;
  struct asa * inst;
}noeudBOUCLE;

typedef struct {
  struct asa * exp;
}noeudRETURN;


typedef struct asa{
  typeNoeud type;
  int memadr;
  int codelen;
  union {
    feuilleNb nb;
    noeudOp op;
    feuilleId id;
    noeudLISTINSTS listinsts;
    noeudDECLA_VAR dec_var;
    noeudAFFECT affect;
    noeudLISTDEC listdec;
    noeudTAB tab;
    noeudBOUCLE bcl;
    noeudRETURN retourner;
  };
} asa;

asa * creer_feuilleNB( int value );
asa * creer_noeudOP( int ope, asa * p1, asa * p2 );
asa * creer_feuilleID(char* id);
asa * creer_listINST(asa * p1, asa * p2);
asa * creer_noeudTAB(asa * id, asa * p2);
asa * creer_noeudLISTDECLA_VAR(asa * p1, asa * p2);
asa * creer_noeudDECLA_VAR(asa * p1, asa * p2, asa * p3);
asa * creer_noeudaffect(asa * p1, asa * p2);
asa * creer_noeudBCL(asa * id, asa * p2, asa * p3);
asa * creer_noeudRETURN(asa * p1);
void free_asa(asa *p);

void print_asa(asa * p);
void print_typeOP(asa *p);
void print_typeNB(asa *p);
void print_typeID(asa *id);
void print_typeDEC(asa *p);
void print_typeLISTINST(asa *p);
void print_typeAFFECT(asa *p);
void print_typeLISTDEC(asa *p);
void print_typeVAR(asa *p);
void print_typeTAB(asa *p);
void print_typeBCL(asa *p);
void print_typeRETURN(asa *p);


void error_asa(const char * s);

#endif
