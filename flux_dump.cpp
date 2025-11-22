#include "FLUX_Dump.h"

flux_errors edges(const node_t* node, FILE* fp)
{
    assert(node != NULL);
    assert(fp != NULL);

    if (node->left != NULL)
    {
        create_edge(node, node->left, fp);
        edges(node->left, fp);
    }

    if (node->right != NULL)
    {
        create_edge(node, node->right, fp);
        edges(node->right, fp);
    }
    
    return FLUX_OK;
}

flux_errors create_edge(const node_t* node1, const node_t* node2, FILE* fp)
{
    assert(node1 != NULL);
    assert(node2 != NULL);
    assert(fp != NULL);

    fprintf(fp, "n%p->n%p[color=\"green\"]\n", node1, node2);

    return FLUX_OK;
}

flux_errors blocks(const node_t* node, FILE* fp)
{
    assert(node != NULL);
    assert(fp != NULL);

    if (node->left != NULL)
        blocks(node->left, fp);

    create_block(node, fp);
    
    if (node->right != NULL)
        blocks(node->right, fp);

    return FLUX_OK;
}

flux_errors create_block(const node_t* node, FILE*  fp)
{
    assert(node != NULL);
    assert(fp != NULL);

    switch (node->type)
    {
    case TYPE_NUM:
        fprintf(fp, "n%p [shape=circle, label =\"value = %g\", ", node, node->value.d);
        break;
    
    case TYPE_OP:
        fprintf(fp, "n%p [shape=triangle, label =\"value = %c\",", node, node->value.ch);
        break;  
    
    case TYPE_VAR:
        fprintf(fp, "n%p [shape=Mrecord, label =\"value = %g\",", node, node->value.d);
        break;

    default:
        printf(RED "ERROR IN CREATING BLOCK\n" RESET);
        return FLUX_ERROR;
    }
    fprintf(fp, "style=filled, fontcolor=\"black\", fillcolor=\"white\"];\n");
    return FLUX_OK;
}

flux_errors create_file (FILE* fp, const node_t* node)
{
    assert(fp);
    assert(node);
    fprintf(fp, "digraph G {\n"
                "splines=ortho\n"
                "node [style=filled, fillcolor=red, fontcolor=white, shape=octagon]\n"
            );
    blocks(node ,fp);
    edges(node, fp);
    
    fprintf(fp, "}\n");
    return FLUX_OK;
}

flux_errors graph_dump(const tree_t* tree)
{
    ON_DEBUG(printf("tree size is %lu\n", tree->size);)

    FILE* fp = open_file("graph.dot");
    create_file(fp, tree->root);
    fclose(fp);

    system_graph();
    return FLUX_OK;
}

FILE* open_file(const char* file_name)
{
    FILE* fp = fopen(file_name, "w");
    assert(fp != 0);
    return fp;
}

flux_errors system_graph()
{
    system("mkdir -p photo");
    static size_t file_number = 0;
    char str[STRSIZE] = {};
    sprintf(str, "dot graph.dot -T png -o photo/graph%lu.png", file_number);
    system(str);
    file_number++;
    ON_DEBUG(printf("graph set\n");)
    return FLUX_OK;
}    

void create_html(char* new_answer, char* question)
{
    FILE* fp = fopen("flux_dump.html", "a");
    static size_t file_number = 0;
    fprintf(fp, "<pre>\n");
    
    if (new_answer == NULL || question == NULL)
        fprintf(fp, "Start fluxion\n");
    
    char path[70] = {};

    sprintf(path, "photo\\graph%lu.png", file_number);    
    fprintf(fp, "<img src=\"%s\">\n\n\n", path);
    
    file_number++;
    fclose(fp);
}

void free_file(const char* name)
{
    FILE* fp = fopen(name, "w");
    fclose(fp);
}

