#ifndef FILE_WORK
#define FILE_WORK


#include "FLUX_Libs.h"
#include "../Onegin/read_from_file_to_buffer.h"
#include "FLUX_Dump.h"

flux_errors node_fprint(node_t* node, FILE* fp);
flux_errors tree_fprint(tree_t ak, const char* file_name);
char* tree_read (tree_t* ak, const char* file_name);
char* get_name(char** cur_pos);
node_t* node_read(char** cur_pos);
void skip_spaces(char** cur_pos);


#endif