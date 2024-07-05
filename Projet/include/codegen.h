#ifndef CODEGEN_H
#define CODEGEN_H

#include "asa.h"
#include "ram_os.h"
#include "ts.h"

extern FILE * exefile;

void codegen(asa *p);
void code_NB(asa * p);
void code_OP(asa * p);
void code_ID(asa *p);
void code_DECVAR(asa *p);
void code_LISTINST(asa *p);
void code_AFFECT(asa *p);
void code_LISTDEC(asa *p);  
void code_BCL(asa *p);
void code_RETURN(asa *p);

extern table_symb TABLE_SYMBOLES;
#endif
