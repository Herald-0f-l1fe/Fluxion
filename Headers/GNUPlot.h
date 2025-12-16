#ifndef TEMP
#define TEMP
#include "FLUX_Libs.h"
#include "calculator.h"


void create_plot(int size_x, int size_y, char* output_image, node_t* func, tree_t* diff, node_t* taylor, double x0);
void sys_plot(FILE* gp);
void print_func(FILE* gp, node_t* func, node_t* diff, tree_t* taylor, double x0);
FILE* start_plot(int size_x, int size_y, char* output_image, char* title);

#endif