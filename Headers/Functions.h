#ifndef FL_TREE_FUNC
#define FL_TREE_FUNC

#include "FLUX_Libs.h"


double node_calculator(node_t* node, var_t* str_vars);
void flux_calculator(tree_t tree);
flux_errors flux_creator(tree_t* flux);
flux_errors flux_destructor(tree_t* root);
flux_errors node_destructor(node_t* root);
var_t* vars_init(var_t* str_vars);
flux_errors var_create(char* name, var_t* str_vars);
double var_check(char* name, var_t* str_vars);
void vars_destructor(var_t* str_vars);





size_t get_node_size(node_t* root);
size_t get_tree_size(tree_t* tree);
void set_parents(node_t* node, node_t* parent);

void buffer_cleaner();
static size_t var_size = 5; 
void op_simpl_add_sub(node_t** node);
void op_simpl_mul(node_t** node);
void op_simpl_div(node_t** node);

#endif

