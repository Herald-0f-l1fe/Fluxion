#include "Functions.h"
#include "FLUX_Dump.h"
#include <cmath>
#include <math.h>
#include "float.h"
#include "FLUX_file_work.h"
#include "Differentiation_functions.h"
#include "DSL.h"

#define calc_right node_calculator(node->right, str_vars)\

#define calc_left node_calculator(node->left, str_vars)

#define BIN_OPERATION(ope)                               \
    return node_calculator(node->left, str_vars) ope node_calculator(node->right, str_vars);      \

#define UN_OPERATION(name)                                  \
    return name(node_calculator(node->right, str_vars));    \




flux_errors flux_creator(tree_t* flux)
{   
    flux->canary_l = CANARY_L;
    flux->canary_r = CANARY_R;
    flux->size = 0;
    flux->root = (node_t*) calloc(1, sizeof(node_t));
    
    free_file("flux_dump.html");
    var_t* str_vars = (var_t*) calloc(var_size, sizeof(var_t));
    vars_init(str_vars);
    flux->vars = str_vars;
    assert(str_vars);


    if (flux->canary_l != CANARY_L || flux->canary_r != CANARY_R)
        return FLUX_CANARY_DIED;

    if (flux->size != 0)
        return FLUX_NO_VALID_SIZE;

    if (flux->root == nullptr)
        return FLUX_NO_ROOT;
    
    return FLUX_OK;
}

flux_errors node_destructor(node_t* root)
{
    assert(root);
    if (root->left != NULL)
        node_destructor(root->left);

    if (root->right != NULL)
        node_destructor(root->right);
        
    free(root);

    return FLUX_OK;
}

flux_errors flux_destructor(tree_t* flux)
{
    node_destructor(flux->root);
    vars_destructor(flux->vars);
    return FLUX_OK;
}


void buffer_cleaner()
{   
    while(getchar() != '\n');
}


void flux_calculator(tree_t tree)
{
    assert(tree.vars);
    double ret = node_calculator(tree.root, tree.vars);
    printf("Calculator answer is %g\n", ret);
}


double node_calculator(node_t* node, var_t* str_vars)
{
    assert(node);

    switch (node->type)
    {
    case TYPE_NUM:
        return node->value.d;
        break;

    case TYPE_VAR: 
        assert(str_vars);
        var_create(node->value.str, str_vars);    
        return var_check(node->value.str, str_vars);
        break;
        
    case TYPE_OP:
        return operations[node->value.op].func(node->value.op, node, str_vars);
        break;

    default:
        break;
    }
    return 0;
}

var_t* vars_init(var_t* str_vars)
{
    for (size_t i = 0; i < var_size; i++)
    {
        str_vars[i].name = (char*) calloc(STRSIZE, sizeof(char)); 
        if (str_vars[i].name == nullptr) return 0;
        
        ON_DEBUG(printf("VAR IS CREATED\n");)
        
        str_vars[i].val = NAN;

        
        if (str_vars[i].name == nullptr)
            printf("NULLPTR to name of var\n");
    }

    assert(str_vars);    
    return str_vars;
}

flux_errors var_create(char* name, var_t* str_vars)
{
    
    if (str_vars == NULL)
    {
        printf("Nullptr to vars arr \n");
        return FLUX_ERROR;
    }
    double var_check_res = var_check(name, str_vars);
    for (size_t i = 0; i < var_size; i++)
    {
        if (str_vars[i].name[0] == '\0' && isnan(var_check_res)) 
        {
            ON_DEBUG(printf("VAR %s IN ARR\n", str_vars[i].name);)

            free(str_vars[i].name);
            str_vars[i].name = name;
            printf("Please give value of %s\n", str_vars[i].name);
            scanf("%lg", &(str_vars[i].val));

            return FLUX_OK;
        }
    }

 
    return FLUX_ERROR;
}

double var_check(char* name, var_t* str_vars)
{
    assert(str_vars);
    assert(name);
    for (size_t i = 0; i < var_size; i++)
    {
        if (!strcmp(str_vars[i].name, name))
            return str_vars[i].val;
    }

    
    return NAN;
}

void vars_destructor(var_t* str_vars)
{
    for (size_t i = 0; i < var_size; i++)
    {
        free(str_vars[i].name); 
    }

    free(str_vars);
}


double bin_operation(int op, node_t* node, var_t* str_vars)
{
    switch(op)
    {
        case FLUX_ADD:
            ON_DEBUG(printf("+");)
            BIN_OPERATION(+);
            break;

        case FLUX_SUB:
            ON_DEBUG(printf("-");)
            BIN_OPERATION(-);
            break;
        
        case FLUX_MUL:
            ON_DEBUG(printf("*");)
            BIN_OPERATION(*);
            break;
        
        case FLUX_DIV:
            ON_DEBUG(printf("/");)
            BIN_OPERATION(/);
            break;
        
        default:
            return NAN;

    }
}

double pow_operation(int , node_t* node, var_t* str_vars)
{   
    assert(node);
    return pow(node_calculator(node->left, str_vars), node_calculator(node->right, str_vars));
}

double un_operation(int op, node_t* node, var_t* str_vars)
{
    switch (op)
    {
        case FLUX_SQVRT:
            UN_OPERATION(sqrt);
        
        case FLUX_SIN:
            UN_OPERATION(sin);

        case FLUX_COS:
            UN_OPERATION(cos);

        case FLUX_TG:
            UN_OPERATION(tan);
        
        case FLUX_SH:
            return (exp(calc_right) - exp(-calc_right))/2;

        case FLUX_CH:   
            return (exp(calc_right) + exp(-calc_right))/2;
    
        default:
            return NAN;
    }

    return NAN;
}
    
double ln_operation(int , node_t* node, var_t* str_vars)
{
        return log(node_calculator(node->right, str_vars));
}

double log_operation(int , node_t* node, var_t* str_vars)
{
    return log(node_calculator(node->left, str_vars)) / log(node_calculator(node->right, str_vars));
}

double e_operation(int , node_t* node, var_t* str_vars)
{
    return exp(node_calculator(node->right, str_vars));
}

double const_simpler(node_t* node)
{
    if (node == NULL)
    {
        printf(RED "ERROR IN CONST SIMPLER POINTER IS NULL\n" RESET);
        return NAN;
    }
    switch (node->type)
    {
    case TYPE_NUM:
        return node->value.d;
        break;

    case TYPE_VAR:
        return NAN;
        break;
    
    case TYPE_OP:
    {
        double lnode = const_simpler(node->left);
        double rnode = const_simpler(node->right);
        if (operations[node->value.op].func == un_operation || operations[node->value.op].func == ln_operation)
        {
            if (!isnan(rnode))
            {
                node->value.d = node_calculator(node, NULL);
                node_destructor(node->right);
                node->type = TYPE_NUM;
                node->right = NULL;
            }
            else
            {
                printf(RED"VAR or ERROR IN RIGHT SON IN CONST_SIMPLER\n" RESET);
                return NAN;
            }
        }
        else
        {
            if (!isnan(rnode) && !isnan(lnode))
            {
                node->value.d = node_calculator(node, NULL);
                node_destructor(node->right);
                node_destructor(node->left);
                node->type = TYPE_NUM;
                node->left = NULL;
                node->right = NULL;
            }
            else 
            {
            return NAN;
            }
        }
        return node->value.d;
        break;
    };

    default:
        return NAN;
        break;
    }
    return NAN;
}

void op_simpler(node_t** node)
{
    assert(*node);
    if ((*node)->right != NULL)
        op_simpler(&((*node)->right));
        
    if ((*node)->left != NULL)
        op_simpler(&((*node)->left));
    
    if ((*node)->type == TYPE_OP)
    {
        int op = (*node)->value.op;
        switch (op)
        {
        case FLUX_MUL:
            op_simpl_mul(node);
            break;

        case FLUX_DIV:
            op_simpl_div(node);
            break;

        case FLUX_SUB:
        case FLUX_ADD:
            op_simpl_add_sub(node);
            break;

        default:
            break;
        }
    }
}


size_t get_node_size(node_t* root)
{
    size_t size = 0;
    if (root->left != NULL)
        size += get_node_size(root->left);

    if (root->right != NULL)
        size += get_node_size(root->right);

    size++;

    return size;   
}

size_t get_tree_size(tree_t* tree)
{
    ON_DEBUG(printf("size before get = %lu", tree->size);)
    tree->size = get_node_size(tree->root);
    ON_DEBUG(printf("size after get = %lu", tree->size);)
    return tree->size;
}


void simple_tree(tree_t* tree)
{
    do
    {
        set_parents(tree->root, NULL);
        simple_node(&(tree->root));
    } while (tree->size > get_tree_size(tree));   
}

void simple_node(node_t** node)
{
    const_simpler(*node);
    op_simpler(node);
}

void set_parents(node_t* node, node_t* parent)
{
    if (node == NULL) return;
    
    node->parent = parent;
    set_parents(node->left, node);
    set_parents(node->right, node);
}

void op_simpl_add_sub(node_t** node)
{
    assert((*node)->left);
    assert((*node)->right);
    node_t* old_node = *node;
    if (node_is_number((*node)->right, 0))
    {
        swap_nodes(node, &((*node)->left));
        free(old_node);
    }  

    else if (node_is_number((*node)->left, 0))
    {
        node_t* old_node = *node;
        swap_nodes(node, &((*node)->right));
        free(old_node);
    }
}

void op_simpl_mul(node_t** node)
{
    assert(*node);
    assert((*node)->left);
    assert((*node)->right);
    node_t* old_node = *node;   
    if ((node_is_number((*node)->left, 0)) || (node_is_number((*node)->right, 0)))
    {
        (*node)->type = TYPE_NUM;
        (*node)->value.d = 0;
        node_destructor((*node)->left);
        node_destructor((*node)->right);
        (*node)->left = NULL;
        (*node)->right = NULL;
    }

    else if (node_is_number((*node)->left, 1))
    {
        node_destructor((*node)->left);
        swap_nodes(node, &(*node)->right);
        free(old_node);
    }

    else if (node_is_number((*node)->right, 1))
    {
        node_destructor((*node)->right);
        swap_nodes(node, &(*node)->left);
        free(old_node);
    }
}

void op_simpl_div(node_t** node)
{
    assert((*node)->left);
    assert((*node)->right);
    node_t* old_node = *node;
    if (node_is_number((*node)->left, 1))
    {
        (*node)->type = TYPE_NUM;
        (*node)->value.d = 0;
        node_destructor((*node)->left);
        node_destructor((*node)->right);
        (*node)->left = NULL;
        (*node)->right = NULL;
    }
    else if (node_is_number((*node)->right, 1))
    {
        node_destructor((*node)->right);
        swap_nodes(node, &(*node)->left);
        free(old_node);
    }
}


void swap_nodes(node_t** node, node_t** child)
{
    assert(*node);
    assert(*child);
    if ((*node)->parent != NULL)
    {
        if ((*node)->parent->left == *node)
            (*node)->parent->left = *child;

        else
            (*node)->parent->right = *child;    
    }

    (*child)->parent = (*node)->parent;
    *node = *child;
}