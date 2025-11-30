#ifndef DIFF
#define DIFF
#include "FLUX_Libs.h"

#include "DSL.h"

size_t type_def(char* str, node_t* new_node);
node_t* diff_tree(tree_t flux, char* var);
node_t* dif_node(node_t* node, char* var); 
node_t* copy_node(node_t* cnode);

node_t* new_node(flux_type type, const char* value, node_t* left_s, node_t* right_s);


#endif