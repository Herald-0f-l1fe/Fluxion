#ifndef READ
#define READ
#include "FLUX_Libs.h"
#include <stdio.h>
#include <sys/stat.h>

static size_t size_operations = sizeof(operations) / sizeof(operations[0]);

char* get_str(char** cur_pos);
node_t* node_read(char** cur_pos);
void skip_spaces(char** cur_pos);
size_t type_def(char* str, node_t* new_node);
char* tree_read (tree_t* ak, const char* file_name);
char* read_from_file_to_buffer(long int*, const char*);
long int finding_file_size(const char*);
char* filling_the_buffer_with_text(ssize_t, FILE*);
int line_counter(char*);

#endif



