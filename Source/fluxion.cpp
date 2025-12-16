#include "FLUX_Libs.h"
#include "FLUX_Dump.h"
#include "Functions.h"
#include "FLUX_file_read.h"
#include "FLUX_file_work.h"
#include "Differentiation_functions.h"
#include "calculator.h"
#include "GNUPlot.h"

int main()
{
    tree_t flux = {};
    open_tex();
    flux_creator(&flux);
    char* buffer = tree_read(&flux, "math example.txt");
    //graph_dump(&flux);
    printf(CYAN "----------------------------------\n" RESET);
    tree_t dif = {};
    flux_creator(&dif);
    dif.root = flux.root;
    //node_destructor(dif.root);
    // dif.root = diff_tree(&flux, "x", 1);
    //printf(CYAN"--------------------------------\n" RESET);
    node_t* tay = taylor(&dif, 0, 5);
    printf(CYAN"--------------------------------\n" RESET);
    dif.root = diff_tree(&flux, "x", 1);
    printf(CYAN"--------------------------------\n" RESET);
    create_plot(800, 600, "plot1.png", flux.root, &dif, tay, 0);
    tree_fprint(dif, "Diff file.txt");
    node_destructor(dif.root);
    free(buffer);
    flux_destructor(&flux);
    node_destructor(tay);
    close_tex();
}