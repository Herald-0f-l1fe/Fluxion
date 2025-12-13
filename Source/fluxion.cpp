#include "FLUX_Libs.h"
#include "FLUX_Dump.h"
#include "Functions.h"
#include "FLUX_file_read.h"
#include "FLUX_file_work.h"
#include "Differentiation_functions.h"
#include "calculator.h"
int main()
{
    tree_t flux = {};
    open_tex();
    flux_creator(&flux);
    char* buffer = tree_read(&flux, "math example.txt");
    graph_dump(&flux);
    printf(CYAN "----------------------------------\n" RESET);
    tree_t dif = {};
    flux_creator(&dif);
    node_destructor(dif.root);
    dif.root = diff_tree(&flux, "x", 1);
    printf(CYAN"--------------------------------\n" RESET);
    node_t* tey = teylor(&dif, 0, 4);
    printf(CYAN"--------------------------------\n" RESET);
    
    tree_fprint(dif, "Diff file.txt");
    node_destructor(dif.root);
    free(buffer);
    flux_destructor(&flux);
    node_destructor(tey);
    close_tex();
}