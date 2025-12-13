#ifndef DIFF
#define DIFF
#include "FLUX_Libs.h"
#include "calculator.h"
#include "FLUX_Dump.h"
#include "DSL.h"
#include "Functions.h"
size_t type_def(const char* str, node_t* new_node);
node_t* diff_tree(tree_t* flux, const char* var, size_t n);
node_t* dif_node(node_t* node, const char* var); 
node_t* copy_node(node_t* cnode);
node_t* teylor(tree_t* flux, double x0, size_t n);

node_t* new_node(flux_type type, const char* value, node_t* left_s, node_t* right_s);


#endif