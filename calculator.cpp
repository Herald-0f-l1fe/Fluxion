#include "calculator.h"



void skip_spaces(char** cur_pos)
{
    while (isspace(**cur_pos))
        (*cur_pos)++;
}


node_t* GetG(char** s)
{
    node_t* val = GetE(s);
    skip_spaces(s);

    if (*(*s) != '$')
    {
        printf("Syntax Error in GetG\n");
        return 0;
    }

    (*s)++;
    return val;
}

node_t* GetE(char** s)
{
    node_t* val = GetT(s);
    skip_spaces(s);

    while(**s == '+' || **s == '-')
    {
        char op = **s;
        (*s)++;
        node_t* val2 = GetT(s);
        if (op == '+')
            val = ADD_(val, val2);
        else 
            val = SUB_(val, val2);
    }
    return val;
}

node_t* GetT(char** s)
{
    node_t* val = GetP(s);
    skip_spaces(s);
    while(**s == '*' || **s == '/')
    {
        char op = **s;
        (*s)++;
        node_t* val2 = GetP(s);
        if (op == '*')
            node_t* res = MUL_(val, val2);
        else 
            node_t* res = DIV_(val, val2);
    }
    return val;
}

node_t* GetP(char** s)
{
    node_t* val = NULL;
    skip_spaces(s);
    if (**s ==   '(')
    {
        (*s)++;
        val = GetE(s);

        skip_spaces(s);
        if (**s == ')')
            (*s)++;
        else
        {
            printf("Syntax error in Getp\n");
            return NULL;
        }
    }

    else
        val = GetN(s);
    return val;
}

node_t* GetN(char** s)
{
    char val[20] = {};
    size_t n = 0;
    skip_spaces(s);
    sscanf(*s, "%s %n", &val, &n);
    if (n == 0)
    {
        printf("Syntax Error in GetN\n");
        return NULL;
    }
    else
    {
        (*s) += n;
        node_t* res = new_node(TYPE_NUM, val, NULL, NULL);
        return res;
    }
    return NULL;
}

