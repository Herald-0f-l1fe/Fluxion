#ifndef DSL_H
#define DSL_H

#define dL dif_node(node->left, var)
#define dR dif_node(node->right, var)
#define L  copy_node(node->left)
#define R  copy_node(node->right)


// #define COMP_FUNC_() 


#define ADD_(node_left, node_right) new_node(TYPE_OP, "+", node_left, node_right)
#define SUB_(node_left, node_right) new_node(TYPE_OP, "-", node_left, node_right)
#define MUL_(node_left, node_right) new_node(TYPE_OP, "*", node_left, node_right)
#define DIV_(node_left, node_right) new_node(TYPE_OP, "/", node_left, node_right)
#define POW_(node_left, node_right) new_node(TYPE_OP, "^", node_left, node_right)



#define EXP_(node_right) new_node(TYPE_OP, "e", NULL, node_right)
#define LN_(node_right)  new_node(TYPE_OP, "ln", NULL, node_right)
#define LOG_(node_left, node_right) new_node(TYPE_OP, "log", node_left, node_right)



#define SIN_(node_right) new_node(TYPE_OP, "sin", NULL, node_right)
#define COS_(node_right) new_node(TYPE_OP, "cos", NULL, node_right)
#define TAN_(node_right) new_node(TYPE_OP, "tan", NULL, node_right)
#define COT_(node_right) new_node(TYPE_OP, "cot", NULL, node_right)



#define CH_(node_right) new_node(TYPE_OP, "cosh", NULL, node_right)
#define SH_(node_right) new_node(TYPE_OP, "sinh", NULL, node_right)
#define TANH_(node_right) new_node(TYPE_OP, "tanh", NULL, node_right)
#define COTH_(node_right) new_node(TYPE_OP, "coth", NULL, node_right)



#define ARCSIN_(node_right) new_node(TYPE_OP, "arcsin", NULL, node_right)
#define ARCCOS_(node_right) new_node(TYPE_OP, "arccos", NULL, node_right)
#define ARCTAN_(node_right) new_node(TYPE_OP, "arctan", NULL, node_right)
#define ARCCOT_(node_right) new_node(TYPE_OP, "arccot", NULL, node_right)



#define ARCSINH_(node_right) new_node(TYPE_OP, "arcsinh", NULL, node_right)
#define ARCCOSH_(node_right) new_node(TYPE_OP, "arccosh", NULL, node_right)
#define ARCTANH_(node_right) new_node(TYPE_OP, "arctanh", NULL, node_right)
#define ARCCOTH_(node_right) new_node(TYPE_OP, "arccoth", NULL, node_right)



#endif