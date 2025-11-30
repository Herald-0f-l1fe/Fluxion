#include "Headers/Functions.h"
#include "FLUX_Dump.h"
#include <cmath>
#include "FLUX_file_work.h"

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
        var_create(node->value.str, str_vars);    
        return var_check(node->value.str, str_vars);
        break;
        
    case TYPE_OP:
        return operations[node->value.op].func(node->value.op, node, str_vars);


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
        
        str_vars[i].val = 0xDEADBEEE;

        
        if (str_vars[i].name == nullptr)
            printf("NULLPTR to name of var\n");
    }

    assert(str_vars);    
    return str_vars;
}

flux_errors var_create(char* name, var_t* str_vars)
{
    
    if (str_vars == 0)
    {
        printf("Nullptr to labels arr \n");
        return FLUX_ERROR;
    }
    double var_check_res = var_check(name, str_vars);
    for (size_t i = 0; i < var_size; i++)
    {
        if (str_vars[i].name[0] == '\0' && var_check_res == 0xDEADBEEE) 
        {
            ON_DEBUG(printf("VAR %s IN ARR\n", str_vars[i].name);)

            strcpy(str_vars[i].name, name);
            printf("Please give value of %s\n", name);
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

    
    return 0xDEADBEEE;
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

double pow_operation(int op, node_t* node, var_t* str_vars)
{
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
    
double ln_operation(int op, node_t* node, var_t* str_vars)
{
        return log(node_calculator(node->right, str_vars));
}

double log_operation(int op, node_t* node, var_t* str_vars)
{
    return log(node_calculator(node->left, str_vars)) / log(node_calculator(node->right, str_vars));
}

double e_operation(int op, node_t* node, var_t* str_vars)
{
    return exp(node_calculator(node->right, str_vars));
}

double flux_simpler(node_t* node)
{
    switch (node->type)
    {
    case TYPE_NUM:
        return node->value.d;
        break;
    
    case TYPE_OP:
    {
        double lnode = flux_simpler(node->left);
        double rnode = flux_simpler(node->right);
        if (lnode != NAN && rnode != NAN)
        {
            
            node->type = TYPE_NUM;
            node->value.d = rnode + lnode;
            free(node->left);
            free(node->right);
            node->left = NULL;
            node->right = NULL;
        }
        else
            return NAN;
        break;
    };

    case TYPE_VAR:
        return NUM;
        break;

    default:
        break;
    }
        


}

node_t* 