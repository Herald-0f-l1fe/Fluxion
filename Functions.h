#ifndef FL_TREE_FUNC
#define FL_TREE_FUNC

#include "FLUX_Libs.h"

#define OPERATION(op)                                                        \
    return node_calculator(node->left) op node_calculator(node->right);      \

double node_calculator(node_t* node);
void flux_calculator(tree_t tree);
flux_errors flux_creator(tree_t* flux);
flux_errors flux_destructor(node_t* root);
void buffer_cleaner();

#endif

