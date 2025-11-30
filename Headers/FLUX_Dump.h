#ifndef FLUX_DUMP
#define FLUX_DUMP

#include "FLUX_Libs.h"

flux_errors edges(const node_t* node, FILE* fp);
flux_errors create_edge(const node_t* node1, const node_t* node2, FILE* fp);
flux_errors blocks(const node_t* node, FILE* fp);
flux_errors create_block(const node_t* node, FILE*  fp);
flux_errors create_file (FILE* fp, const node_t* node);
flux_errors graph_dump(const tree_t* tree);
flux_errors system_graph();
FILE* open_file(const char* file_name);

void create_html(char* new_answer, char* question);
void free_file(const char* name);


#endif