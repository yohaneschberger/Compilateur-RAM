#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ts.h"
#include "asa.h"
#include "codegen.h"

void semantic(asa *p);
void semantic_NB(asa * p);
void semantic_OP(asa * p);
void semantic_ID(asa * p);
void semantic_DECVAR(asa * p);
void semantic_LISTINST(asa * p);
void semantic_AFFECT(asa * p);
void semantic_LISTDEC(asa * p);
void semantic_TAB(asa * p);
void semantic_BCL(asa * p);
void semantic_RETURN(asa * p);

extern table_symb TABLE_SYMBOLES;

#endif
