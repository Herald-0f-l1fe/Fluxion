#ifndef FILE_WORK
#define FILE_WORK


#include "FLUX_Libs.h"

#include "FLUX_Dump.h"

flux_errors node_fprint(node_t* node, FILE* fp);
flux_errors tree_fprint(tree_t ak, const char* file_name);
flux_errors value_fprint(node_t* node, FILE* fp);




#endif