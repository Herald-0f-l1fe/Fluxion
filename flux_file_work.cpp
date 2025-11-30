#include "FLUX_file_work.h"


flux_errors tree_fprint(tree_t ak, const char* file_name)
{
    FILE* fp = open_file(file_name);
    node_fprint(ak.root, fp);

    fclose(fp);
    return FLUX_OK;
}

flux_errors node_fprint(node_t* node, FILE* fp)
{
    value_fprint(node, fp);
    
    if (node->left != 0)
        node_fprint(node->left, fp);
    else
        fprintf(fp, "nil ");
    

    if (node->right != 0)
        node_fprint(node->right, fp);
    else
        fprintf(fp, "nil ");
    
    fprintf(fp, ") ");
    return FLUX_OK;
}

flux_errors value_fprint(node_t* node, FILE* fp)
{
    switch (node->type)
    {
    case TYPE_OP:
        fprintf(fp, "( \"%s\" ", operations[node->value.op].str);
        break;
    
    case TYPE_NUM:
        fprintf(fp, "( \"%g\" ", node->value.d);
        break;

    case TYPE_VAR:
        fprintf(fp, "( \"%s\" ", node->value.str);
        break;

    default:    
        break;
    }
    return FLUX_OK;
}
