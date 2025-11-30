#include "FLUX_file_read.h"

char* read_from_file_to_buffer(ssize_t* size, const char* str)
{
    FILE* fp = fopen(str, "r");
    if (fp == nullptr) 
        printf("File %s didn't open\n", str);

    *size = finding_file_size(str);

    char* buffer = filling_the_buffer_with_text(*size, fp);

    fclose(fp);
    return buffer;
}

long int finding_file_size(const char* str)
{
    struct stat file_info = {};
    if (stat(str, &file_info) == 0) 
    {
        printf("Size of file %ld\n", file_info.st_size);
    }
    else
    {
        printf("smth went wrong, erasing all data on hard drive...\n");
    }
    return file_info.st_size;
}

char* filling_the_buffer_with_text(ssize_t size, FILE* fp)
{
    char* buffer = (char*) calloc((size_t) size + 2, sizeof(char));
    if (buffer == nullptr)
        {
        printf("Memory for the buffer was not allocated.\n");
        return 0;
        }
    size_t buffer_size = fread(buffer, sizeof(char), (size_t) size, fp);

    buffer[buffer_size++] = '\n';
    buffer[buffer_size++] = '\0';

    return buffer;
}

char* tree_read (tree_t* ak, const char* file_name)
{
    ssize_t size = 0;
    char* buffer = read_from_file_to_buffer(&size, file_name);
    char* ptr = buffer;
    free(ak->root);

    ak->root = node_read(&buffer);
    printf(CYAN "READ EXAMPLE\n" RESET);

    return ptr;
}



char* get_str(char** cur_pos)
{
    char* ptr1 = 0;
    ptr1 = *cur_pos;

    char* ptr2 = strchr(*cur_pos, ' ');
    *ptr2 = '\0';
    *cur_pos += ptr2-ptr1 + 1;

    return ptr1;
}

size_t type_def(char* str, node_t* new_node)
{
    for (size_t i = 0; i < size_operations; i++)
    {
        if (!strcasecmp(str, operations[i].str))
        {
            new_node->type = operations[i].type;
            new_node->value.op = operations[i].oper;
            return i;
        }
    }

    if (strcasecmp(str, "0.0") && atof(str) != 0)
    {
        new_node->type = TYPE_NUM;
        new_node->value.d = atof(str);
        return FLUX_NUM;
    }
        

    else
    {
        new_node->type = TYPE_VAR;
        new_node->value.str = str;
        return FLUX_VAR;
    }

    return 0;
}




node_t* node_read(char** cur_pos)
{
    char ans[100] = {};
    skip_spaces(cur_pos);
    if (**cur_pos == '(')
    {
        node_t* new_node = (node_t*) calloc (1, sizeof(node_t));
        (*cur_pos)++;

        skip_spaces(cur_pos);
        new_node->value.str = get_str(cur_pos);

        type_def(new_node->value.str, new_node);

        if (**cur_pos == '(' && new_node->type == TYPE_OP && operations[new_node->value.op].func != un_operation)
            new_node->left = node_read(cur_pos);

        else
        {
            sscanf(*cur_pos, "%s", ans);
            ON_DEBUG(printf("ans is \"%s\"\n", ans);)
            if (!strcmp(ans, "nil"))
            {
                *cur_pos += 4;
                skip_spaces(cur_pos);
            }

            else
                printf(RED "AK_ERROR in with node in graph left leaf is bad\n" RESET);
        }

        if (**cur_pos == '(')
            new_node->right = node_read(cur_pos);

        else
        {
            sscanf(*cur_pos, "%s", ans);
            if (!strcmp(ans, "nil"))
            {
                *cur_pos += 4;
                skip_spaces(cur_pos);
            }
            else
                printf(RED "AK_ERROR in file with graph\n" RESET);
        }
        
        if (**cur_pos == ')')
        {
            ON_DEBUG(printf(GREEN "GET NODE\n" RESET);)
            (*cur_pos)++;
            skip_spaces(cur_pos);
            return new_node;
        }
        else
            printf(RED "NO \")\" in the end of node\n" RESET);
    }

    else 
    {
        printf(RED "NO \"(\" in the start of node\n" RESET);
    }

    return 0;
}
