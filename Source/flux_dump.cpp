#include "FLUX_Dump.h"
#include "calculator.h"

const char* phrases[] = 
{
    "Когда Бог создавал мир, он сказал, что это очевидно.\n",
    "Адам посмотрел и сказал, что он это делать не будет, потому что не хочет, поэтому мы сделаем это за него.\n",
    "Когда Бог делал Еву из ребра Адама, он подсчитал, что =>\n",
    "Когда Бог только создал свет и тьму, они уже знали, что...\n",
    "Даже самое первое растение понимало, что...\n",
    "Первым следом человечества на земле была надпись\n",
    "Луна не стала звездой, потому что она не понимала, почему\n",
    "Рыба-капля живёт так глубоко, потому что она один раз забыла, что\n",
    "В седьмой день, когда Бог расслабленно считал интегралы в уме, он доказал, что\n",
    "Если ты не знаешь этот факт, то ты не поступишь в Синергию, смотри ниже\n",
    "Ну тут сам разберёшься\n",
    "Мы это никогда не доказывали, потому что мы думали, что это очевидно. Но это правда очевидно!\n"

};

#define add_sub_tex(op)                     \
    fprintf(fp, "(");                       \
                                            \
    node_tex(node->left, fp);               \
                                            \
    fprintf(fp, #op);                       \
                                            \
    node_tex(node->right, fp);              \
                                            \
    fprintf(fp, ")");                       \

#define un_op_tex(op)                       \
    fprintf(fp, #op"(");                    \
    node_tex(node->right, fp);              \
    fprintf(fp, ")");

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
        fprintf(fp, "n%p [shape=circle, label =\"%g\", ", node, node->value.d);
        break;
    
    case TYPE_OP:
        fprintf(fp, "n%p [shape=triangle, label =\"%s\",", node, operations[node->value.op].str);
        break;  
    
    case TYPE_VAR:
        fprintf(fp, "n%p [shape=Mrecord, label =\"'%s'\",", node, node->value.str);
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

void latex_dump(tree_t* tree)
{  
    FILE* fp = fopen("dump.tex", "a+");
    fprintf(fp, "\\[");
    node_tex(tree->root, fp);
    fprintf(fp, "\\]\n"); 
    fclose(fp);
}

void open_tex()
{  
    FILE* fp = fopen("dump.tex", "w");
    fprintf(fp, "\\documentclass[12pt,a4paper]{article}\n\\begin{document}\n");
    fclose(fp);
}

void close_tex()
{  
    FILE* fp = fopen("dump.tex", "a+");
    fprintf(fp, "\\end{document}\n");
    fclose(fp);
    system("pdflatex dump.tex");
}

void node_tex(node_t* node, FILE* fp)
{
    assert(node);
    switch (node->type)
    {
    case TYPE_NUM:
        fprintf(fp, "{%g}", node->value.d);
        break;
    
    case TYPE_VAR:
        fprintf(fp, "%s", node->value.str);
        break;

    case TYPE_OP:
        operations[node->value.op].tex(node, fp);
        break;

    default:
        break;
    }
}


void add_tex(node_t* node, FILE* fp)
{
    int op = node->value.op;
    switch (op)
    {
    case FLUX_ADD:
        add_sub_tex(+);
        break;
    
    case FLUX_SUB:
        add_sub_tex(-);

    default:
        break;
    }
}

void mul_tex(node_t* node, FILE* fp)
{
    node_tex(node->left, fp);
    fprintf(fp, " \\cdot ");
    node_tex(node->right, fp);
};

void div_tex(node_t* node, FILE* fp)
{
    fprintf(fp, "\\frac{");
    node_tex(node->left, fp);
    fprintf(fp, "}{");
    node_tex(node->right, fp);
    fprintf(fp, "}");
}

void pow_tex(node_t* node, FILE* fp)
{
    fprintf(fp, "(");
    node_tex(node->left, fp);
    fprintf(fp, ")^{");
    node_tex(node->right, fp);
    fprintf(fp, "}");
}

void un_tex(node_t* node, FILE* fp)
{
    int op = node->value.op;
    switch (op)
    {
    case FLUX_LN:
        un_op_tex(\\ln);
        break;

    case FLUX_E:
        un_op_tex(\\exp);
        break;

    case FLUX_SIN:
        un_op_tex(\\sin);
        break;

    case FLUX_COS:
        un_op_tex(\\cos);
        break;

    case FLUX_TG:
        un_op_tex(\\tg);
        break;

    case FLUX_SH:
        un_op_tex(\\sinh);
        break;

    case FLUX_CH:
        un_op_tex(\\cosh);
        break;

    case FLUX_SQVRT:
        un_op_tex(\\sqrt);
        break;

    default:
        break;
    }
}

void log_tex(node_t* node, FILE* fp)
{
    fprintf(fp, "log_(");
    node_tex(node->left, fp);
    fprintf(fp, ") (");
    node_tex(node->right, fp);
    fprintf(fp, ")");
}