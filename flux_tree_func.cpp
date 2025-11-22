#include "Functions.h"
#include "FLUX_Dump.h"

flux_errors flux_creator(tree_t* flux)
{   
    flux->canary_l = CANARY_L;
    flux->canary_r = CANARY_R;
    flux->size = 0;
    flux->root = (node_t*) calloc(1, sizeof(node_t));
    
    free_file("flux_dump.html");
    if (flux->canary_l != CANARY_L || flux->canary_r != CANARY_R)
        return FLUX_CANARY_DIED;
    if (flux->size != 0)
        return FLUX_NO_VALID_SIZE;
    if (flux->root == nullptr)
        return FLUX_NO_ROOT;
    
    return FLUX_OK;
}

flux_errors flux_destructor(node_t* root)
{
    assert(root);
    if (root->left != NULL)
        flux_destructor(root->left);

    if (root->right != NULL)
        flux_destructor(root->right);
    free(root);

    return FLUX_OK;
}


void buffer_cleaner()
{   
    while(getchar() != '\n');
}


// void clean_stack(stack_t* stack)
// {
//     assert(stack);
//     void* ptr  = 0;
//     while(stack->size > 0)
//     {
//         stack_pop(stack, &ptr);
//         free(ptr);
//     }
// }

void flux_calculator(tree_t tree, var_t* str_vars)
{
    double ret = node_calculator(tree.root, str_vars);
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
    {
        switch (node->value.ch)
        {
        case '+':
            OPERATION(+)
            break;
        
        case '-':
            OPERATION(-);
            
        case '*':
            OPERATION(*);

        case '/':
            OPERATION(/);

        default:
            break;
        }
    }

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
            printf("NULLPTR to name of label");
    }

    
    return str_vars;
}

flux_errors var_create(char* name, var_t* str_vars)
{
    if (str_vars == 0)
    {
        printf("Nullptr to labels arr \n");
        return FLUX_ERROR;
    }
    
    for (size_t i = 0; i < var_size; i++)
    {
        if (str_vars[i].name[0] == '\0' && var_check(name, str_vars) == 0xDEADBEEE) 
        {
            ON_DEBUG(printf("VAR %s IN ARR\n", str_vars[i].name);)

            strcpy(str_vars[i].name, name);
            printf("Please give value of %s\n", name);
            scanf("%g", &(str_vars[i].val));

            return FLUX_OK;
        }
    }

 
    return FLUX_ERROR;
}

double var_check(char* name, var_t* str_vars)
{
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

node_t* node_read(char** cur_pos)
{
    char ans[100] = {};
    skip_spaces(cur_pos);
    if (**cur_pos == '(')
    {
        node_t* new_node = (node_t*) calloc (1, sizeof(node_t));
        (*cur_pos)++;

        skip_spaces(cur_pos);
        get_val(cur_pos);


        skip_spaces(cur_pos);
        if (**cur_pos == '(')
            new_node->left = node_read(cur_pos);

        else
        {
            sscanf(*cur_pos, "%s", ans);
            ON_DEBUG(printf("ans is \"%s\"\n", ans);)
            if (!strcmp(ans, "nil"))
            {
                *cur_pos += 4;
                skip_spaces(cur_pos);
            }

            else
                printf(RED "AK_ERROR in with node in graph left leaf is bad\n" RESET);
        }

        if (**cur_pos == '(')
            new_node->right = node_read(cur_pos);

        else
        {
            sscanf(*cur_pos, "%s", ans);
            if (!strcmp(ans, "nil"))
            {
                *cur_pos += 4;
                skip_spaces(cur_pos);
            }
            else
                printf(RED "AK_ERROR in file with graph\n" RESET);
        }
        
        if (**cur_pos == ')')
        {
            ON_DEBUG(printf(GREEN "GET NODE\n" RESET);)
            (*cur_pos)++;
            skip_spaces(cur_pos);
            return new_node;
        }
        else
            printf(RED "NO \")\" in the end of node\n" RESET);
    }

    else 
    {
        printf(RED "NO \"(\" in the end of node\n" RESET);
    }
    return 0;
}

char* get_val(char** cur_pos)
{
    char* ptr1 = 0;
    if (**cur_pos == '\"')
    {
        (*cur_pos)++;
        ptr1 = *cur_pos;
        char* ptr2 = strchr(*cur_pos, '\"');
        *ptr2 = '\0';
        *cur_pos += ptr2-ptr1 + 1;
    }   

    else 
    {
        printf("AK_ERROR in get node name\n");
    }
    return ptr1;
}

void skip_spaces(char** cur_pos)
{
    while (isspace(**cur_pos))
        (*cur_pos)++;
}
