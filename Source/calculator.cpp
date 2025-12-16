#include "compiler.h"
#include "stdlib.h"
#include "math.h"


void skip_spaces(char** cur_pos)
{
    while (isspace(**cur_pos))
        (*cur_pos)++;
}


node_t* GetG(char** s, var_t* str_vars)
{
    node_t* val = GetE(s, str_vars);
    skip_spaces(s);

    if (*(*s) != '$')
    {
        printf("ERROR '%c' not '$'\n", *(*s));
        ON_DEBUG(printf(RED "Syntax Error in GetG\n" RESET);)
        return 0;
    }

    (*s)++;
    return val;
}

node_t* GetE(char** s, var_t* str_vars)
{
    node_t* val = GetT(s, str_vars);
    skip_spaces(s);

    while(**s == '+' || **s == '-')
    {
        char op = **s;
        (*s)++;
        node_t* val2 = GetT(s, str_vars);
        if (op == '+')
            val = ADD_(val, val2);
        else 
            val = SUB_(val, val2);
    }
    return val;
}

node_t* GetT(char** s, var_t* str_vars)
{
    node_t* val = GetS(s, str_vars);
    skip_spaces(s);
    while(*(*s) == '*' || *(*s) == '/' )
    {
        char op = *(*s);
        (*s)++;
        node_t* val2 = GetS(s, str_vars);
        if (op == '*')
        {
            val = MUL_(val, val2);
        }
        else 
        {
            val = DIV_(val, val2);
        }
    }
    return val;
}

node_t* GetP(char** s, var_t* str_vars)
{
    node_t* val = NULL;
    skip_spaces(s);
    if (*(*s) ==   '(')
    {
        (*s)++;
        val = GetE(s, str_vars);

        skip_spaces(s);
        if (*(*s) == ')')
            (*s)++;
        else
        {
            ON_DEBUG(printf("Syntax error in Getp\n");)
            return NULL;
        }
    }

    else 
    {
        char* old_s = *s;
        //printf("before %p\n", *s);
        val = GetNumber(s);
        if (val == NULL)
        {
            *s = old_s;
            //printf("after %p\n", *s);
            val = GetF(s, str_vars);
        }
    }
    return val;
}

node_t* GetNumber(char** s)
{
    char* val1 = (char*) calloc(20, sizeof(char));
    double val = 0;
    int n = 0;
    skip_spaces(s);
    sscanf(*s, "%lf%n", &val, &n);
    sprintf(val1, "%g", val);
    if (n == 0)
    {
        ON_DEBUG(printf("Syntax Error in GetNumber\n");)
        free(val1);
        return NULL;
    }
    else
    {
        ON_DEBUG(printf(YELLOW "n = %d val = %g\n" RESET , n, val);)  
        (*s) += n;
        ON_DEBUG(printf("str in GeTN = '%s'", *s));
        node_t* res = new_node(TYPE_NUM, val1, NULL, NULL);
        free(val1);
        return res;
    }

    return NULL;
}



// node_t* GetA(char** s)
// {
//     skip_spaces(s);
//     GetVAR(s);
//     skip_spaces(s);
//     if (*(*s) != '=')
//     {
//         printf("Syntax error in GetA\n");
//         return NULL;
//     }
//     GetE(s);
//     //TODO тут надо будет написать присваение переменной значение.
    
// }


// node_t* GetC(char** s)
// {
//     skip_spaces(s);
//     char cond[10] = {};
//     sscanf(*s, "%s", cond);
//     if (strcmp(cond, "if")) //TODO потом поменять на что-нибудь нормальное.
//     {
//         printf("Syntax Error in GetC\n");
//         return NULL;
//     }

//     skip_spaces(s);
//     if (*(*s) != '(')
//     {
//         printf("Syntax Error in GetC\n");
//         return NULL;   
//     }
//     GetE(s);
//     if (*(*s) != ')')
//     {
//         printf("Syntax Error in GetC\n");
//         return NULL;   
//     }
//     //TODO тут надо сделать какой-то узел, пока не добавлю операторов непонятно.
// }

node_t* GetVar(char** s, var_t* str_vars)
{
    skip_spaces(s);
    char* var = (char*) calloc(20, sizeof(char));
    int n = 0;
    sscanf(*s, "%[a-zA-Z0-9] %n", var, &n);
    //printf(CYAN "%s\n" RESET, var);
    (*s) += n;    
    node_t* node = (node_t*) calloc(1, sizeof(node_t));
    node->type = TYPE_VAR;
    node->value.str = var;
    node->left = node->right = NULL;
    if (isnan(var_check(var, str_vars)))
    {
        var_create(var, str_vars);
    }
    return node;
}

node_t* GetF(char** s, var_t* str_vars)
{
    char* func = (char*) calloc(10, sizeof(char));
    int n = 0;
    skip_spaces(s);

    sscanf(*s, "%[a-z] %n", func, &n);
    ON_DEBUG(printf ("%s\n", *s);)
    (*s) += n; 
    ON_DEBUG(printf( RED "func is %s %d\n" RESET, func, n);)
    if (n == 0)
    {
        ON_DEBUG(printf("Syntax error in GetF\n");)      
        return NULL;
    }
    node_t* node = (node_t*) calloc(1, sizeof(node_t));
    type_def(func, node);

    if (node->type == TYPE_OP && node->value.op == FLUX_LOG)
    {
        node->left = GetP(s, str_vars);
        node->right = GetP(s, str_vars);
    }

    else if (node->type == TYPE_OP)
    {
        node->right = GetP(s, str_vars);
    }
    
    else if (node->type == TYPE_VAR)
    {
        ON_DEBUG(printf("it's not a func, it's var\n");)
        (*s) -= n;
        free(node);
        node = GetVar(s, str_vars);
    }
    else
    {
        ON_DEBUG(printf("It's Number in GetF\n");)
    }
    ON_DEBUG(printf("str in GetF = '%s'\n", *s);)
    free(func);
    return node;
}

node_t* GetS (char** s, var_t* str_vars)
{
    
    node_t* nodel = GetP(s, str_vars);
    skip_spaces(s);
    if (*(*s) == '^')
    {
        (*s)++;
        node_t* noder = GetP(s, str_vars);
        nodel = POW_(nodel, noder);
    }
    return nodel;
}