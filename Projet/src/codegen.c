#include "codegen.h"

int cptinst=0;
char CTXT2[32] = "global";

void codegen(asa *p)
{
  if(!cptinst) 
  {
    fprintf(exefile,"LOAD #32 ;\nSTORE 2 ;\nLOAD #64 ;\nSTORE 8 ;\nLOAD #65 ;\nSTORE 3 ;\nLOAD #1023 ;\nSTORE 9 ;\nLOAD #26 ;\nSTORE 10 ;\n");
    cptinst+=10;
  }
  if(!p) return;
  switch (p->type){
  case typeNB:
    code_NB(p);
    break;
  case typeOP:
    code_OP(p);
    break;
  case typeID:
    code_ID(p);
    break;
  case typeDEC:
    code_DECVAR(p);
    break;
  case typeLISTINST:
    code_LISTINST(p);
    break;
  case typeAFFECT:
    code_AFFECT(p);
    break;
  case typeLISTDEC:
    code_LISTDEC(p);
    break;
  case typeBCL:
    code_BCL(p);
    break;
  case typeRETURN:
    code_RETURN(p);
    break;
  default:
    break;
  }
}

void code_NB(asa * p){
  //on stocke la valeur de l'entier dans l'ACC
  fprintf(exefile,"LOAD #%-7d ;\n", p->nb.val);cptinst++;
  fprintf(exefile,"STORE @%-7d ;\n", RAM_OS_STK_REG);cptinst++;
}

void code_OP(asa * p){
  /*
   * On commence par générer le code des noeuds dans l'ordre de l'associativité
   */
  switch (p->op.ope){
    /*
     * Operateurs associatifs à gauche, on génère d'abord la partie droite
     * on l'empile et on génère la partie gauche
     */
  case '+':
    codegen(p->op.noeud[1]);
    fprintf(exefile,"INC %-9d ;\n", RAM_OS_STK_REG);cptinst++;
    codegen(p->op.noeud[0]);
    break;
  case '-':
    codegen(p->op.noeud[1]);
    fprintf(exefile,"INC %-9d ;\n", RAM_OS_STK_REG);cptinst++;
    codegen(p->op.noeud[0]);
    break;
  case '*':
    codegen(p->op.noeud[1]);
    fprintf(exefile,"INC %-9d ;\n", RAM_OS_STK_REG);cptinst++;
    codegen(p->op.noeud[0]);
    break;
  case '/':
    codegen(p->op.noeud[1]);
    fprintf(exefile,"INC %-9d ;\n", RAM_OS_STK_REG);cptinst++;
    codegen(p->op.noeud[0]);
    break;
  case '%':
    codegen(p->op.noeud[1]);
    fprintf(exefile,"INC %-9d ;\n", RAM_OS_STK_REG);cptinst++;
    codegen(p->op.noeud[0]);
    break;
  default:
    break;
  }

  /*
   * On gère ensuite les opérateurs au cas par cas et on dépile
   */

  switch (p->op.ope){
  case '+':
    fprintf(exefile,"DEC %-9d ;\nADD @%-8d ;\n", RAM_OS_STK_REG, RAM_OS_STK_REG);cptinst++;cptinst++;
    fprintf(exefile,"STORE @%-9d ;\n", RAM_OS_STK_REG);cptinst++;
    break;
  case '-':
    fprintf(exefile,"DEC %-9d ;\nSUB @%-8d ;\n", RAM_OS_STK_REG, RAM_OS_STK_REG);cptinst++;cptinst++;
    fprintf(exefile,"STORE @%-9d ;\n", RAM_OS_STK_REG);cptinst++;
    break;
  case '*':
    fprintf(exefile,"DEC %-9d ;\nMUL @%-8d ;\n", RAM_OS_STK_REG, RAM_OS_STK_REG);cptinst++;cptinst++;
    fprintf(exefile,"STORE @%-9d ;\n", RAM_OS_STK_REG);cptinst++;
    break;
  case '/':
    fprintf(exefile,"DEC %-9d ;\nDIV @%-8d ;\n", RAM_OS_STK_REG, RAM_OS_STK_REG);cptinst++;cptinst++;
    fprintf(exefile,"STORE @%-9d ;\n", RAM_OS_STK_REG);cptinst++;
    break;
  case '%':
    fprintf(exefile,"DEC %-9d ;\nMOD @%-8d ;\n", RAM_OS_STK_REG, RAM_OS_STK_REG);cptinst++;cptinst++;
    fprintf(exefile,"STORE @%-9d ;\n", RAM_OS_STK_REG);cptinst++;
    break;
  default:
    break;
  }
}

void code_ID(asa * p){
  symbole * id;
  id=ts_rechercher_identificateur(TABLE_SYMBOLES,p->id.id,CTXT2);
  fprintf(exefile,"LOAD %-7d ;\n", id->adr);cptinst++;
}

void code_DECVAR(asa * p){
  codegen(p->dec_var.id);
  codegen(p->dec_var.exp);
  codegen(p->dec_var.decla_suiv);
}

void code_LISTINST(asa * p){
  codegen(p->listinsts.inst);
  codegen(p->listinsts.suite);
}

void code_AFFECT(asa * p){
  codegen(p->affect.exp);
  fprintf(exefile,"INC %-9d ;\n", RAM_OS_STK_REG);cptinst++;
}

void code_LISTDEC(asa * p){
  codegen(p->listinsts.inst);
  codegen(p->listinsts.suite);
}

void code_BCL(asa * p){
  char cmd[128];
  codegen(p->bcl.exp);
  int dbcl=cptinst,fbcl;
  if(!(strcmp(p->bcl.id->id.id,"SI"))){
    fprintf(exefile,"JUMZ TMPSI ;\n");cptinst++; //on connait pas encore le nombre d'instruction
    codegen(p->bcl.inst);
    fbcl=cptinst;
    sprintf(cmd,"sed -i 's/TMPSI/%-7d/g' exefile",fbcl); //une fois qu'on connait le nombre d'instruction dans la boucle on peut remplacer le TMP par la première ligne hors de la boucle
    system(cmd);
  }
  if(!(strcmp(p->bcl.id->id.id,"TQ"))){
    fprintf(exefile,"JUMZ TMPTQ ;\n");cptinst++;
    codegen(p->bcl.inst);
    fprintf(exefile,"JUMP %-7d ;\n",dbcl);cptinst++; //ligne fbcl
    fbcl=cptinst+1;
    sprintf(cmd,"sed -i 's/TMPTQ/%-7d/g' exefile",fbcl);
    system(cmd);

  }
}

void code_RETURN(asa * p){
  codegen(p->retourner.exp);
}
