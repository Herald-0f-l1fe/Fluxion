#ifndef NEW_CALC
#define NEW_CALC

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "FLUX_Libs.h"
#include "DSL.h"
#include "Differentiation_functions.h"

node_t*  GetG(char** s);
node_t* GetE(char** s);
node_t* GetT(char** s);
node_t* GetP(char** s);
node_t* GetN(char** s);
void skip_spaces(char** cur_pos);


#endif