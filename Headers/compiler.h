#ifndef NEW_Compiler
#define NEW_Compiler

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "FLUX_Libs.h"
#include "DSL.h"
#include "Differentiation_functions.h"

node_t* GetG(char** s);
node_t* GetE(char** s);
node_t* GetT(char** s);
node_t* GetP(char** s);
node_t* GetF(char** s);
node_t* GetS (char** s);
node_t* GetNumber(char** s);

#endif