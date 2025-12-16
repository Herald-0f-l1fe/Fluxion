#ifndef SIMPLIFY
#define SIMPLIFY

#include "FLUX_Libs.h"
#include "math.h"
#include "float.h"
#include "DSL.h"
#include "Functions.h"
#include "calculator.h"

double const_simpler(node_t* node);
void simple_tree(tree_t* tree);
void op_simpler(node_t* node);
void simple_node(node_t** node);
void swap_nodes(node_t** node, node_t* child);

#endif