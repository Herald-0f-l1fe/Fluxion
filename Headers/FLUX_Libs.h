#ifndef FLUX_LIBS
#define FLUX_LIBS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../common/COLORS.h"
// #define DEBUG
#include "../common/DEBUG.h"
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
    node_t* parent;
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

typedef node_t* (*diff_t)(node_t* node, const char* str_vars);

typedef void    (*tex_t)(node_t* node,  FILE* fp);

typedef void    (*gnu_t)(node_t* node, FILE* gp);

struct answer
{
    const char* str;
    flux_type type;
    operate oper;
    func_t func;
    diff_t dif;
    tex_t tex;
    gnu_t gnu;
};
//CALC FUNCTIONS
double bin_operation(int op, node_t* node, var_t* str_vars);
double pow_operation(int op, node_t* node, var_t* str_vars);
double un_operation (int op, node_t* node, var_t* str_vars);
double ln_operation (int op, node_t* node, var_t* str_vars);
double log_operation(int op, node_t* node, var_t* str_vars);
double e_operation  (int op, node_t* node, var_t* str_vars);

//DIFF FUNCTIONS
node_t* dif_add (node_t* node, const char* var);
node_t* dif_sub (node_t* node, const char* var);
node_t* dif_mul (node_t* node, const char* var);
node_t* dif_div (node_t* node, const char* var);
node_t* dif_pow (node_t* node, const char* var);
node_t* dif_ln  (node_t* node, const char* var);
node_t* dif_log (node_t* node, const char* var);
node_t* dif_e   (node_t* node, const char* var);
node_t* dif_sin (node_t* node, const char* var);
node_t* dif_cos (node_t* node, const char* var);
node_t* dif_tg  (node_t* node, const char* var);
node_t* dif_sh  (node_t* node, const char* var);
node_t* dif_ch  (node_t* node, const char* var);

//LATEX FUNCTION
void log_tex(node_t* node, FILE* fp);
void add_tex(node_t* node, FILE* fp);
void mul_tex(node_t* node, FILE* fp);
void div_tex(node_t* node, FILE* fp);
void pow_tex(node_t* node, FILE* fp);
void un_tex (node_t* node, FILE* fp);

//GNU_PLOT FUNCTION
void node_gnu   (node_t* node, FILE* gp);
void add_gnu    (node_t* node, FILE* gp);
void sub_gnu    (node_t* node, FILE* gp);
void mul_gnu    (node_t* node, FILE* gp);
void div_gnu    (node_t* node, FILE* gp);
void pow_gnu    (node_t* node, FILE* gp);
void log_gnu    (node_t* node, FILE* gp);
void ln_gnu     (node_t* node, FILE* gp);
void sin_gnu    (node_t* node, FILE* gp);
void cos_gnu    (node_t* node, FILE* gp);
void tg_gnu     (node_t* node, FILE* gp);
void sh_gnu     (node_t* node, FILE* gp);
void ch_gnu     (node_t* node, FILE* gp);
void th_dnu     (node_t* node, FILE* gp);
void sqvrt_gnu   (node_t* node, FILE* gp);
void e_gnu      (node_t* node, FILE* gp);


#define CANARY_L 0xB055
#define CANARY_R 0xDEAD
#define STRSIZE 100



#endif
