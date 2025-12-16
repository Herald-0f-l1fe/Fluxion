#ifndef NEW_CALC
#define NEW_CALC

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "FLUX_Libs.h"
#include "DSL.h"
#include "Differentiation_functions.h"

node_t* GetG(char** s, var_t* str_vars);
node_t* GetE(char** s, var_t* str_vars);
node_t* GetT(char** s, var_t* str_vars);
node_t* GetP(char** s, var_t* str_vars);
node_t* GetF(char** s, var_t* str_vars);
node_t* GetS (char** s, var_t* str_vars);
node_t* GetNumber(char** s, var_t* str_vars);
void skip_spaces(char** cur_pos);

static answer operations[] = 
{
      {"+",     TYPE_OP,     FLUX_ADD,   bin_operation, dif_add,     add_tex, add_gnu       },
      {"-",     TYPE_OP,     FLUX_SUB,   bin_operation, dif_sub,     add_tex, sub_gnu       },
      {"*",     TYPE_OP,     FLUX_MUL,   bin_operation, dif_mul,     mul_tex, mul_gnu       },
      {"/",     TYPE_OP,     FLUX_DIV,   bin_operation, dif_div,     div_tex, div_gnu       },
      {"^",     TYPE_OP,     FLUX_POW,   pow_operation, dif_pow,     pow_tex, pow_gnu       },
      {"ln",    TYPE_OP,     FLUX_LN,    ln_operation,  dif_ln,      un_tex , ln_gnu        },
      {"log",   TYPE_OP,     FLUX_LOG,   log_operation, dif_log,     log_tex, log_gnu       },
      {"exp",   TYPE_OP,     FLUX_E,     e_operation,   dif_e,       un_tex , e_gnu         },
      {"sin",   TYPE_OP,     FLUX_SIN,   un_operation,  dif_sin,     un_tex , sin_gnu       },
      {"cos",   TYPE_OP,     FLUX_COS,   un_operation,  dif_cos,     un_tex , cos_gnu       },
      {"tg",    TYPE_OP,     FLUX_TG,    un_operation,  dif_tg,      un_tex , tg_gnu        },
      {"sh",    TYPE_OP,     FLUX_SH,    un_operation,  dif_sh,      un_tex , sh_gnu        },
      {"ch",    TYPE_OP,     FLUX_CH,    un_operation,  dif_ch,      un_tex , ch_gnu        },
      {"sqrt",  TYPE_OP,     FLUX_SQVRT, un_operation,  dif_pow,     un_tex , sqvrt_gnu     }
};

#endif