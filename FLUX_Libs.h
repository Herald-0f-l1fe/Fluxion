#ifndef FLUX_LIBS
#define FLUX_LIBS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "COLORS.h"
#define DEBUG
#include "DEBUG.h"
#include <assert.h>


enum flux_type
{
    TYPE_NUM,
    TYPE_OP,
    TYPE_VAR,
};

enum operate
{
    FLUX_ADD,
    FLUX_SUB,   
    FLUX_MUL, 
    FLUX_DIV,
    FLUX_POW,
    FLUX_LN,
    FLUX_LOG,
    FLUX_E,
    FLUX_SIN,
    FLUX_COS,
    FLUX_TG,
    FLUX_SH,
    FLUX_CH,
    FLUX_SQVRT,
    FLUX_NUM,
    FLUX_VAR,
};

union flux_value
{
    double d;
    char* str;
    operate op;
};

struct var_t
{
    char* name;
    double val;
};

struct  node_t
{
    flux_type type;
    flux_value value;
    node_t* left;
    node_t* right;
};


struct  tree_t
{
    int canary_l;
    node_t* root;
    var_t* vars;
    size_t size;
    int canary_r;
};

enum flux_errors
{
    FLUX_OK = 0,
    FLUX_ERROR = 1,
    FLUX_CANARY_DIED,
    FLUX_NO_VALID_SIZE,
    FLUX_NO_ROOT,
};

struct path
{
    node_t* node;
    char c;
};

typedef double (*func_t)(int op, node_t* node, var_t* str_vars);

typedef node_t* (*diff_t)(node_t* node, char* str_vars);


struct answer
{
    const char* str;
    flux_type type;
    operate oper;
    func_t func;
    diff_t dif;
};

double bin_operation(int op, node_t* node, var_t* str_vars);
double pow_operation(int op, node_t* node, var_t* str_vars);
double un_operation(int op, node_t* node, var_t* str_vars);
double ln_operation(int op, node_t* node, var_t* str_vars);
double log_operation(int op, node_t* node, var_t* str_vars);
double e_operation(int op, node_t* node, var_t* str_vars);


node_t* dif_add(node_t* node, char* var);
node_t* dif_sub(node_t* node, char* var);
node_t* dif_mul(node_t* node, char* var);
node_t* dif_div(node_t* node, char* var);
node_t* dif_pow(node_t* node, char* var);
node_t* dif_ln(node_t* node, char* var);
node_t* dif_log(node_t* node, char* var);
node_t* dif_e(node_t* node, char* var);
node_t* dif_sin(node_t* node, char* var);
node_t* dif_cos(node_t* node, char* var);
node_t* dif_tg(node_t* node, char* var);
node_t* dif_sh(node_t* node, char* var);
node_t* dif_ch(node_t* node, char* var);


static answer operations[] = 
{
      {"+",     TYPE_OP,     FLUX_ADD,   bin_operation, dif_add     },
      {"-",     TYPE_OP,     FLUX_SUB,   bin_operation, dif_sub     },
      {"*",     TYPE_OP,     FLUX_MUL,   bin_operation, dif_mul     },
      {"/",     TYPE_OP,     FLUX_DIV,   bin_operation, dif_div     },
      {"^",     TYPE_OP,     FLUX_POW,   pow_operation, dif_pow     },
      {"ln",    TYPE_OP,     FLUX_LN,    ln_operation,  dif_ln      },
      {"log",   TYPE_OP,     FLUX_LOG,   log_operation, dif_log     },
      {"e",     TYPE_OP,     FLUX_E,     e_operation,   dif_e       },
      {"sin",   TYPE_OP,     FLUX_SIN,   un_operation,  dif_sin     },
      {"cos",   TYPE_OP,     FLUX_COS,   un_operation,  dif_sin     },
      {"tg",    TYPE_OP,     FLUX_TG,    un_operation,  dif_tg      },
      {"sh",    TYPE_OP,     FLUX_SH,    un_operation,  dif_sh      },
      {"ch",    TYPE_OP,     FLUX_CH,    un_operation,  dif_ch      },
      {"sqrt",  TYPE_OP,     FLUX_SQVRT, un_operation,  dif_pow     },
      {"",      TYPE_NUM,    FLUX_NUM                               },
      {"",      TYPE_VAR,    FLUX_VAR                               }
};


#define CANARY_L 0xB055
#define CANARY_R 0xDEAD
#define STRSIZE 100



#endif
