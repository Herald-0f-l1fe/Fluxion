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
    flux_creator(&flux);
    char* buffer = tree_read(&flux, "math example.txt");
    graph_dump(&flux);
    
    flux_calculator(flux);
    tree_fprint(flux, "Output file.txt");

    tree_t dif = {};
    flux_creator(&dif);
    node_t* dif_root = diff_tree(flux, "x");
    dif.root = dif_root;
    graph_dump(&dif);
    tree_fprint(dif, "Diff file.txt");


    char* s = 0; /*(char*) calloc(200, sizeof(char));*/
    size_t n = 0;
    buffer_cleaner();
    getline(&s, &n, stdin);
    char* old = s;
    node_t* res = GetG(&s);
    free(old);
    
    dif.root = res;
    graph_dump(&dif);

    flux_destructor(&dif);
    free(buffer);
    flux_destructor(&flux);
}
    