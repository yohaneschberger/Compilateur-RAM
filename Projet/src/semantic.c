#include "semantic.h"

void semantic(asa *p){
  if(!p) return;
  switch (p->type){
  case typeNB:
    semantic_NB(p);
    printf("AJOUT D'UN NB\n");
    break;
  case typeOP:
    semantic_OP(p);
    printf("AJOUT D'UN OP\n");
    break;
  case typeID:
    semantic_ID(p);
    printf("AJOUT D'UN ID\n");
    break;
  case typeDEC:
    semantic_DECVAR(p);
    printf("AJOUT D'UN DEC\n");
    break;
  case typeLISTINST:
    semantic_LISTINST(p);
    printf("AJOUT D'UN LISTINST\n");
    break;
  case typeAFFECT:
    semantic_AFFECT(p);
    printf("AJOUT D'UN AFFECT\n");
    break;
  case typeLISTDEC:
    semantic_LISTDEC(p);
    printf("AJOUT D'UN LISTDEC\n");
    break;
  case typeTAB:
    semantic_TAB(p);
    printf("AJOUT D'UN TAB\n");
    break;
  case typeBCL:
    semantic_BCL(p);
    printf("AJOUT D'UN BCL\n");
    break;
  case typeRETURN:
    semantic_RETURN(p);
    printf("AJOUT D'UN RETURN\n");
    break;
  default:
    break;
  }
}

char CTXT[32] = "global";

void semantic_NB(asa * p)
{
  p->codelen = 1;
}

void semantic_OP(asa * p)
{
  switch (p->op.ope){
  case '+':  // operateur binaire associatif a gauche (+ - * / % )
    semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;
  case '-':
    semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;
  case '*':
    semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;
  case '/':
    semantic(p->op.noeud[0]);
    if(p->op.noeud[1]->type==typeNB){
      if(p->op.noeud[1]!=0){
        semantic(p->op.noeud[1]);
      }
      else{
        perror("division par 0");
        exit(1);
      }
    }
    else{
      semantic(p->op.noeud[1]);
    }
    break;
  case '%':
    semantic(p->op.noeud[0]);
    semantic(p->op.noeud[1]);
    break;
  case 'w':
    semantic(p->op.noeud[0]);
    break;
  default:
    break;
  }
}

void semantic_ID(asa * p){
  if(!(ts_rechercher_identificateur(TABLE_SYMBOLES, p->id.id, CTXT))){
    perror("Identificateur non declare");
    exit(1);
  }
}

void semantic_DECVAR(asa * p){
  if(p->dec_var.id->type==typeTAB){
    if(ts_rechercher_identificateur(TABLE_SYMBOLES, p->dec_var.id->tab.id->id.id, CTXT)){
      perror("La variable est déjà déclarée\n");
    }
    else{
      ts_ajouter_identificateur(TABLE_SYMBOLES, CTXT, p->dec_var.id->tab.id->id.id, TYPE_TABLEAU, 1);
    }
  }
  else{
    if(ts_rechercher_identificateur(TABLE_SYMBOLES, p->dec_var.id->id.id, CTXT)){
      perror("La variable est déjà déclarée\n");
    }
    else{
      ts_ajouter_identificateur(TABLE_SYMBOLES, CTXT, p->dec_var.id->id.id, TYPE_ENTIER, 1);
    }
  }
  semantic(p->dec_var.exp);
  semantic(p->dec_var.decla_suiv);
}
void semantic_LISTINST(asa * p){
  semantic(p->listinsts.inst);
  semantic(p->listinsts.suite);
}

void semantic_AFFECT(asa * p){
  if(p->affect.id->type==typeTAB){
    if(!(ts_rechercher_identificateur(TABLE_SYMBOLES, p->affect.id->tab.id->id.id, CTXT))){
      if ( ts_rechercher_identificateur(TABLE_SYMBOLES,p->affect.id->tab.id->id.id,CTXT)==NULL){
        perror("Erreur variable non declaree avant affectation");
        exit(1);
      }
    }
  }
  else{
    if(!(ts_rechercher_identificateur(TABLE_SYMBOLES, p->affect.id->id.id, CTXT))){
      if ( ts_rechercher_identificateur(TABLE_SYMBOLES,p->affect.id->id.id,CTXT)==NULL){
        perror("Erreur variable non declaree avant affectation");
        exit(1);
      }
    }
  }
  semantic(p->affect.exp);
}

void semantic_LISTDEC(asa * p){
  semantic(p->listdec.dec);
  semantic(p->listdec.suite);
}

void semantic_TAB(asa * p){
  if(!(ts_rechercher_identificateur(TABLE_SYMBOLES, p->tab.id->id.id, CTXT))){
    if ( ts_rechercher_identificateur(TABLE_SYMBOLES, p->tab.id->id.id,CTXT)==NULL){
      perror("Erreur liste non declaree");
      exit(1);
    }
  }
  semantic(p->tab.exp);
}

void semantic_BCL(asa * p){
  if(!(ts_rechercher_identificateur(TABLE_SYMBOLES, p->bcl.id->id.id, CTXT))){
    if ( ts_rechercher_identificateur(TABLE_SYMBOLES, p->bcl.id->id.id,CTXT)==NULL){
      perror("Erreur variable de boucle non declaree");
      exit(1);
    }
  }
  semantic(p->bcl.exp);
}

void semantic_RETURN(asa * p){
  semantic(p->retourner.exp);
}
