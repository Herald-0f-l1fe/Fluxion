#include "GNUPlot.h"
#include "FLUX_Dump.h"




FILE* start_plot(int size_x, int size_y, char* output_image, char* title)
{
    free_file("Plot.gp");
    FILE* gp = open_file("plot.gp");
    fprintf(gp, "set terminal pngcairo size %d, %d enhanced font 'Arial,12'\n", size_x, size_y);
    fprintf(gp, "set output '%s'\n", output_image);
    fprintf(gp, "set title '%s' font 'Arial,16'\n", title);
    fprintf(gp, "set xlabel 'x' font 'Arial,14'\n");
    fprintf(gp, "set ylabel 'f(x)' font 'Arial,14'\n");
    fprintf(gp, "set grid lt 0 lw 1 lc rgb '#DDDDDD'\n");
    fprintf(gp, "set key outside right top box\n");
    fprintf(gp, "set yrange [-1:1]\n");
    return gp;
}

void print_func(FILE* gp, node_t* func, tree_t* diff, node_t* taylor, double x0)
{
    fprintf(gp, "set object circle at %g,%g size 0.1 fillcolor rgb \"black\" fillstyle solid noborder \n", x0, node_calculator(func, diff->vars));   
    fprintf(gp, "plot \\\n");
    fprintf(gp, "  ");
    node_gnu(func, gp);
    fprintf(gp, " with lines lw 2 lc rgb 'red' title 'func', \\\n");
    fprintf(gp, "  ");
    node_gnu(diff->root, gp);
    fprintf(gp, " with lines lw 2 lc rgb 'blue' title 'diff', \\\n");
    fprintf(gp, "  ");
    node_gnu(taylor, gp);
    fprintf(gp, " with lines lw 2 lc rgb 'green' title 'taylor'\n");
}

void sys_plot(FILE* gp)
{
    fclose(gp);
    system("gnuplot plot.gp");
    //remove("plot.gp");
}


void create_plot(int size_x, int size_y, char* output_image, node_t* func, tree_t* diff, node_t* taylor, double x0)
{
    FILE* gp = start_plot(size_x, size_y, output_image, "Как же мне поебать");
    print_func(gp, func, diff, taylor, x0);
    sys_plot(gp);   
}



#define bin_gnu(op)                             \
    fprintf(gp, "(");                           \
    node_gnu(node->left, gp);                   \   
    fprintf(gp, ")" #op "(");                   \ 
    node_gnu(node->right, gp);                  \
    fprintf(gp, ")");         


#define un_gnu(op, node)                        \
    fprintf(gp, #op "(");                       \
    node_gnu(node, gp);                         \
    fprintf(gp, ")");                           



void add_gnu(node_t* node, FILE* gp)
{
    bin_gnu(+)
}

void sub_gnu(node_t* node, FILE* gp)
{
    bin_gnu(-)
}

void mul_gnu(node_t* node, FILE* gp)
{
    bin_gnu(*)
}

void div_gnu(node_t* node, FILE* gp)
{
    bin_gnu(/)
}

void pow_gnu(node_t* node, FILE* gp)
{
    bin_gnu(**)
}

void log_gnu(node_t* node, FILE* gp)
{
    un_gnu(ln, node->right)
    fprintf(gp, "/");
    un_gnu(ln, node->left)
}

void e_gnu(node_t* node, FILE* gp)
{
    un_gnu(exp, node->right);
}

void sin_gnu(node_t* node, FILE* gp)
{
    un_gnu(sin, node->right)
}

void cos_gnu(node_t* node, FILE* gp)
{
    un_gnu(cos, node->right)
}

void tg_gnu(node_t* node, FILE* gp)
{
    un_gnu(tg, node->right)
}

void sh_gnu(node_t* node, FILE* gp)
{
    un_gnu(sinh, node->right)
}

void ch_gnu(node_t* node, FILE* gp)
{
    un_gnu(cosh, node->right)
}

void th_gnu(node_t* node, FILE* gp)
{
    un_gnu(tanh, node->right)
}

void ln_gnu(node_t* node, FILE* gp)
{
    un_gnu(ln, node->right)
}

void sqvrt_gnu(node_t* node, FILE* gp)
{
    un_gnu(sqrt, node->right)
}

void node_gnu(node_t* node, FILE* gp)
{
    assert(node);
    assert(gp);
    switch (node->type)
    {
    case TYPE_VAR:
        fprintf(gp, "%s", node->value.str);
        break;
    case TYPE_NUM:
        fprintf(gp, "%g", node->value.d);    
        break;
    case TYPE_OP:
        operations[node->value.op].gnu(node, gp);
        break;
    default:
        break;
    }
}