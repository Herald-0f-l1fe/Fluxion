#include "Differentiation_functions.h"
#include <cmath>

node_t* copy_node(node_t* cnode)
{
    
    node_t* new_node = (node_t*) calloc(1, sizeof(node_t));
    new_node->type = cnode->type;
    new_node->value = cnode->value;
    if (cnode->left)
        new_node->left = copy_node(cnode->left);

    if (cnode->right)
        new_node->right = copy_node(cnode->right);

    return new_node;
}

node_t* diff_tree(tree_t flux, char* var)
{
    node_t* ans = dif_node(flux.root, var);
    printf("First dif\n");
    return ans;

}

node_t* dif_node(node_t* node, char* var)
{
    assert(node);
    node_t* new_node = (node_t*) calloc(1, sizeof(node_t));
    switch (node->type)
    {
    case TYPE_NUM:
        new_node->type = TYPE_NUM;
        new_node->value.d = 0;
        break;
    case TYPE_VAR:
        new_node->type = TYPE_NUM;

        if (!strcmp(var, node->value.str))
            new_node->value.d = 1;

        else
            new_node->value.d = 0;

        break;

    case TYPE_OP:
        new_node = operations[node->value.op].dif(node, var);
        break;

    default:
        break;
    }
    return new_node;
}


node_t* dif_add(node_t* node, char* var)
{
    assert(node);
    assert(var);
    node_t* nnode = ADD_(dL, dR);
       

    return nnode;
}

node_t* dif_sub(node_t* node, char* var)
{
    assert(node);
    assert(var);
    node_t* nnode = SUB_(dL, dR);
       

    return nnode;
}

node_t* new_node(flux_type type, const char* value, node_t* left_s, node_t* right_s)
{
    node_t* node = (node_t*) calloc(1, sizeof(node_t));
    type_def((char*) value, node);
    node->left = left_s;
    node->right = right_s;
    return node;
}


node_t* dif_mul(node_t* node, char* var)
{
    assert(var);
    assert(node);
    node_t* lnode = MUL_(dL, R);
    node_t* rnode = MUL_(L, dR);
    return ADD_(    lnode, rnode);
}

node_t* dif_div(node_t* node, char* var)
{
    node_t* lnode = MUL_(dL, R);
    node_t* rnode = MUL_(L, dR);
    node_t* chisl = SUB_(lnode, rnode);
    node_t* znam = MUL_(R, R);
    node_t* frac = DIV_(chisl, znam);
    
    return frac;
}

node_t* dif_ln(node_t* node, char* var)
{
    node_t* left_n = DIV_(new_node(TYPE_NUM, "1", 0 , 0), R);
    node_t* right_n = dR;
    node_t* res = MUL_(right_n, left_n);
}

node_t* dif_log(node_t* node, char* var)
{
    char strdouble[20] = {};
    snprintf(strdouble, 19, "%lf" , log(node->left->value.d));
    node_t* znam = MUL_(new_node(TYPE_NUM, strdouble, 0, 0), R);
    node_t* frac = DIV_(dR, znam);
    return frac;
}

node_t* dif_e(node_t* node, char* var)
{
    return MUL_(EXP_(R), dR);
}

node_t* dif_sin(node_t* node, char* var)
{
    return MUL_(dR, COS_(R));
}

node_t* dif_cos(node_t* node, char* var)
{
    node_t* _res = MUL_(dR, SIN_(R));
    return MUL_(_res, new_node(TYPE_OP, "-1", NULL, NULL));
}

node_t* dif_tg(node_t* node, char* var)
{
    node_t* znam = MUL_(COS_(R), COS_(R));
    return DIV_(dR, znam);
}

node_t* dif_ctg(node_t* node, char* var)
{   
    node_t* znam = MUL_(SIN_(L), SIN_(R));
    node_t* frac = DIV_(new_node(TYPE_NUM, "-1", NULL, NULL), znam);

    return MUL_(dR, frac);
}

node_t* dif_sh(node_t* node, char* var)
{
    return MUL_(dR, CH_(R));
}

node_t* dif_ch(node_t* node, char* var)
{
    node_t* res = MUL_(new_node(TYPE_NUM, "-1", NULL, NULL), SH_(R));
    return MUL_(dR, res);
}

node_t* dif_pow(node_t*node, char* var)
{   
    node_t* pow = MUL_(LN_(L), R);
    return MUL_(dif_node(pow, var), EXP_(pow));
}

