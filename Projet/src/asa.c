#include "asa.h"

static const char str_type[][16] = {"NB", "OP" ,"ID", "DEC", "LISTINST", "AFFECT", "LISTDEC", "TAB", "BOUCLE", "RENVOYER"};
static const char line[36] = "-----------------------------------";

static char buffer[32];
static char TABULATION[256] = "";
static int indent = 0;

asa * creer_feuilleNB(int val)
{
  asa *p;

  if ((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeNB;
  p->nb.val = val;

  return p;
}

asa * creer_feuilleID(char* id){
  asa *p;
  if((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeID;
  strcpy(p->id.id,id);

  return p;
}

asa * creer_listINST( asa * p1, asa * p2)
{
  asa *p;
  if((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeLISTINST;
  p->listinsts.inst=p1;
  p->listinsts.suite=p2;

  return p;
}

asa * creer_noeudTAB(asa * p1, asa * p2)
{
  asa *p;
  if((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeTAB;
  p->tab.id = p1;
  p->tab.exp = p2;

  return p;
}

asa * creer_noeudLISTDECLA_VAR(asa * p1, asa * p2)
{
  asa *p;
  if((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeLISTDEC;
  p->listdec.dec = p1;
  p->listdec.suite = p2;

  return p;
}

asa * creer_noeudOP( int ope, asa * p1, asa * p2)
{
  asa * p;

  if ((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeOP;
  p->op.ope = ope;
  p->op.noeud[0] =p1;
  p->op.noeud[1] =p2;

  return p;
}

asa * creer_noeudDECLA_VAR(asa * p1, asa * p2, asa * p3)
{
  asa * p;

  if ((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeDEC;
  p->dec_var.id = p1;
  p->dec_var.exp = p2;
  p->dec_var.decla_suiv = p3;

  return p;
}

asa * creer_noeudaffect(asa * p1, asa * p2)
{
  asa * p;

  if ((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeAFFECT;
  p->affect.id = p1;
  p->affect.exp = p2;

  return p;
}

asa * creer_noeudBCL(asa * id, asa * p2, asa * p3)
{
  asa * p;

  if ((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeBCL;
  p->bcl.id = id;
  p->bcl.exp = p2;
  p->bcl.inst = p3;

  return p;
}

asa * creer_noeudRETURN(asa * p1)
{
  asa * p;

  if ((p = malloc(sizeof(asa))) == NULL)
    error_asa("echec allocation mémoire");

  p->type = typeRETURN;
  p->retourner.exp = p1;

  return p;
}


void free_asa(asa *p)
{
  if (!p) return;
  switch (p->type) {
  case typeOP:
    free_asa(p->op.noeud[0]);
    free_asa(p->op.noeud[1]);
    break;
  case typeNB:
    break;
  default: break;
  }
  free(p);
}

static void print_field(char * field_name, char * field_val, char *TABULATION, char *escape_seq)
{
  char buffer[128] = "";
  sprintf(buffer, "%s|%s%*s| ",
	  TABULATION,
	  field_name,
	  MAX_SIZE_FIELD_NAME-(int)strlen(field_name),"");
  printf("%s%s%s" TXT_NULL "%*s\n",
	 buffer,
	 escape_seq,
	 field_val,
	 MAX_SIZE_FIELD_VAL-MAX_SIZE_FIELD_NAME-(int)strlen(field_val),"|");
}

void print_asa(asa * p){
  int i;
  if (!p) return;
  for (i=0; i<indent; i++){ TABULATION[i] = '\t'; }
  TABULATION[i]='\0';

  printf("%s%s\n", TABULATION, line);

  sprintf(buffer, "%p", p);
  print_field("  noeud", buffer, TABULATION,TXT_BLUE TXT_BOLD);

  sprintf(buffer, "%s", str_type[p->type]);
  print_field("  typeNoeud", buffer, TABULATION,"");

  printf("%s%s\n" \
	 "%s|%*s|%*s|\n",TABULATION, line,
	 TABULATION, MAX_SIZE_FIELD_NAME,"",MAX_SIZE_FIELD_VAL-MAX_SIZE_FIELD_NAME,"");

  sprintf(buffer, "%d", p->memadr);
  print_field("adr mem", buffer, TABULATION,"");

  sprintf(buffer, "%d", p->codelen);
  print_field("taille code", buffer, TABULATION,"");

  switch (p->type) {
  case typeOP:
    print_typeOP(p);
    break;
  case typeNB:
    print_typeNB(p);
    break;
  case typeID:
    print_typeID(p);
    break;
  case typeAFFECT:
    print_typeAFFECT(p);
    break;
  case typeDEC:
    print_typeDEC(p);
    break;
  case typeLISTINST:
    print_typeLISTINST(p);
    break;
  case typeLISTDEC:
    print_typeLISTDEC(p);
    break;
  case typeTAB:
    print_typeTAB(p);
    break;
  case typeBCL:
    print_typeBCL(p);
    break;
  case typeRETURN:
    print_typeRETURN(p);
    break;
  default:
    break;
  }
}

void print_typeOP(asa *p)
{
  sprintf(buffer, "'%c'", p->op.ope);
  print_field("operateur", buffer, TABULATION,"");

  sprintf(buffer, "%p", p->op.noeud[0]);
  print_field("ope gauche", buffer, TABULATION,TXT_GREEN);
  sprintf(buffer, "%p", p->op.noeud[1]);
  print_field("ope droite", buffer, TABULATION,TXT_GREEN);

  printf("%s%s\n", TABULATION,line);
  indent++; print_asa(p->op.noeud[0]); indent--;
  indent++; print_asa(p->op.noeud[1]); indent--;

}

void print_typeID(asa *p)
{
  sprintf(buffer, "%s", p->id.id);
  print_field("identificateur", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);

}

void print_typeDEC(asa *p)
{
  sprintf(buffer, "%p", p->dec_var.id);
  print_field("declaration", buffer, TABULATION,"");

  sprintf(buffer, "%p", p->dec_var.exp);
  print_field("expression", buffer, TABULATION,TXT_GREEN);
  sprintf(buffer, "%p", p->dec_var.decla_suiv);
  print_field("suite declar", buffer, TABULATION,TXT_GREEN);

  printf("%s%s\n", TABULATION,line);
  indent++; print_asa(p->dec_var.id); indent--;
  indent++; print_asa(p->dec_var.exp); indent--;
  indent++; print_asa(p->dec_var.decla_suiv); indent--;

}

void print_typeLISTINST(asa *p)
{
  sprintf(buffer, "%p", p->listinsts.inst);
  print_field("instruction", buffer, TABULATION,TXT_GREEN);
  sprintf(buffer, "%p", p->listinsts.suite);
  print_field("suite d inst", buffer, TABULATION,TXT_GREEN);

  printf("%s%s\n", TABULATION,line);
  indent++; print_asa(p->listinsts.inst); indent--;
  indent++; print_asa(p->listinsts.suite); indent--;

}

void print_typeAFFECT(asa *p)
{
  sprintf(buffer, "%p", p->affect.id);
  print_field("affectation", buffer, TABULATION,"");

  sprintf(buffer, "%p", p->affect.exp);
  print_field("expression", buffer, TABULATION,TXT_GREEN);

  printf("%s%s\n", TABULATION,line);
  indent++; print_asa(p->affect.id); indent--;
  indent++; print_asa(p->affect.exp); indent--;
}

void print_typeLISTDEC(asa *p)
{
  sprintf(buffer, "%p", p->listdec.dec);
  print_field("declaration", buffer, TABULATION,TXT_GREEN);
  sprintf(buffer, "%p", p->listdec.suite);
  print_field("suite declaration", buffer, TABULATION,TXT_GREEN);

  printf("%s%s\n", TABULATION,line);
  indent++; print_asa(p->listdec.dec); indent--;
  indent++; print_asa(p->listdec.suite); indent--;

}

void print_typeNB(asa *p)
{
  sprintf(buffer, "%d", p->nb.val);
  print_field("val", buffer, TABULATION,"");
  printf("%s%s\n", TABULATION,line);
}

void print_typeVAR(asa *p)
{
  sprintf(buffer, "%p", p->dec_var.id);
  print_field("ID", buffer, TABULATION,TXT_GREEN);
  sprintf(buffer, "%p", p->dec_var.exp);
  print_field("EXPRESSION", buffer, TABULATION,TXT_GREEN);
  sprintf(buffer, "%p", p->dec_var.decla_suiv);
  print_field("AFFECTATION", buffer, TABULATION,TXT_GREEN);

  printf("%s%s\n", TABULATION,line);
  indent++; print_asa(p->dec_var.id); indent--;
  indent++; print_asa(p->dec_var.exp); indent--;
  indent++; print_asa(p->dec_var.decla_suiv); indent--;
}

void print_typeTAB(asa *p)
{
  sprintf(buffer, "%p", p->tab.id);
  print_field("tableau", buffer, TABULATION,"");

  sprintf(buffer, "%p", p->tab.exp);
  print_field("expression", buffer, TABULATION,TXT_GREEN);

  printf("%s%s\n", TABULATION,line);
  indent++; print_asa(p->tab.id); indent--;
  indent++; print_asa(p->tab.exp); indent--;
}

void print_typeBCL(asa *p)
{
  sprintf(buffer, "%p", p->bcl.id);
  print_field("boucle", buffer, TABULATION, TXT_GREEN);

  sprintf(buffer, "%p", p->bcl.exp);
  print_field("condition", buffer, TABULATION, TXT_GREEN);

  sprintf(buffer, "%p", p->bcl.inst);
  print_field("instructions", buffer, TABULATION, TXT_GREEN);

  printf("%s%s\n", TABULATION,line);
  indent++; print_asa(p->bcl.id); indent--;
  indent++; print_asa(p->bcl.exp); indent--;
  indent++; print_asa(p->bcl.inst); indent--;
}

void print_typeRETURN(asa *p)
{
  sprintf(buffer, "%p", p->retourner.exp);
  print_field("expression", buffer, TABULATION,TXT_GREEN);

  printf("%s%s\n", TABULATION,line);
  indent++; print_asa(p->retourner.exp); indent--;
}

void error_asa(const char * s){
  fprintf(stderr, TXT_BOLD TXT_RED "[erreur ASA]" TXT_NULL " %s", s);
  exit(1);
}
