#ifndef FLUX_LIBS
#define FLUX_LIBS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../common/COLORS.h"
#define DEBUG
#include "../common/DEBUG.h"
#include <assert.h>


enum flux_type
{
    TYPE_NUM,
    TYPE_OP,
    TYPE_VAR,
};

union flux_value
{
    double d;
    char ch;
    char* str;
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

struct var_t
{
    char* name;
    double val;
};

size_t var_size = 5;

#define CANARY_L 0xB055
#define CANARY_R 0xDEAD
#define STRSIZE 100



#endif
