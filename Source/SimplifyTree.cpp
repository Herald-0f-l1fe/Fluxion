#include "FLUX_Libs.h"
#include "math.h"
#include "float.h"
#include "DSL.h"
#include "Functions.h"
#include "calculator.h"

double const_simpler(node_t* node);
void simple_tree(tree_t* tree);
void op_simpler(node_t* node);
void simple_node(tree_t* tree);
void swap_nodes(node_t** node, node_t* child);

double const_simpler(node_t* node)
{
    if (node == NULL)
    {
        //printf(RED "ERROR IN CONST SIMPLER POINTER IS NULL\n" RESET);
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
                node->left  = NULL;
            }
            else
            {
                //printf(RED "VAR or ERROR IN RIGHT SON IN CONST_SIMPLER\n" RESET);
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
        simple_node(tree);
    } while (tree->size > get_tree_size(tree));   
}

void simple_node(tree_t* tree)
{
    const_simpler(tree->root);
    //graph_dump(tree);
    op_simpler(&(tree->root));
    //graph_dump(tree);
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
        free((*node)->right);
        swap_nodes(node, ((*node)->left));
        free(old_node);
    }  

    else if (node_is_number((*node)->left, 0))
    {
        free((*node)->left);
        node_t* old_node = *node;
        swap_nodes(node, ((*node)->right));
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
        swap_nodes(node, (*node)->right);
        free(old_node);
    }

    else if (node_is_number((*node)->right, 1))
    {
        node_destructor((*node)->right);
        swap_nodes(node, (*node)->left);
        free(old_node);
    }
}

void op_simpl_div(node_t** node)
{
    assert((*node)->left);
    assert((*node)->right);
    node_t* old_node = *node;
    if (node_is_number((*node)->left, 0))
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
        swap_nodes(node, (*node)->left);
        free(old_node);
    }
}

void swap_nodes(node_t** node, node_t* child)
{
    assert(*node);
    assert(child);
    if ((*node)->parent != NULL)
    {
        if ((*node)->parent->left == *node)
            (*node)->parent->left = child;

        else if ((*node)->parent->right == *node)
            (*node)->parent->right = child;    
        
        //else
            //printf(RED "Error in node parent\n" RESET); 
    }

    child->parent = (*node)->parent;
    *node = child;
}