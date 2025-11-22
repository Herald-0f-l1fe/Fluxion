#include "FLUX_Libs.h"
#include "FLUX_Dump.h"
#include "Functions.h"
#include "FLUX_file_work.h"

int main()
{
    node_t* node1 = (node_t*) calloc(1, sizeof(node_t));
    node_t* node2 = (node_t*) calloc(1, sizeof(node_t));
    node_t* node3 = (node_t*) calloc(1, sizeof(node_t));

    node1->type = TYPE_OP;
    node1->value.ch = '+';
    node2->type = TYPE_VAR;
    node2->value.ch = 'x';
    node3->type = TYPE_NUM;
    node3->value.d = 2.5;
    node1->left = node2;
    node1->right = node3;



    tree_t tree = {};
    tree.root = node1;

    flux_calculator(tree);
    graph_dump(&tree);

    free(node1);
    free(node2);
    free(node3);
}
