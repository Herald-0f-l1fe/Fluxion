#include "FLUX_file_work.h"

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
    char* buffer = (char*) calloc(size + 2, sizeof(char));
    if (buffer == nullptr)
        {
        printf("Memory for the buffer was not allocated.\n");
        return 0;
        }
    size_t buffer_size = fread(buffer, sizeof(char), size, fp);

    buffer[buffer_size++] = '\n';
    buffer[buffer_size++] = '\0';

    return buffer;
}

flux_errors tree_fprint(tree_t ak, const char* file_name)
{
    FILE* fp = open_file(file_name);
    node_fprint(ak.root, fp);

    fclose(fp);
    return FLUX_OK;
}

flux_errors node_fprint(node_t* node, FILE* fp)
{
    fprintf(fp, "( \"%s\" ", node->value.str);
    
    if (node->left != 0)
        node_fprint(node->left, fp);
    else
        fprintf(fp, "nil ");
    
    if (node->right != 0)
        node_fprint(node->right, fp);
    else
        fprintf(fp, "nil ");
    
    fprintf(fp, ") ");
    return FLUX_OK;
}

flux_errors value_fprint(node_t* node, FILE* fp)
{
    switch (node->type)
    {
    case TYPE_OP:
        fprintf(fp, "( \"%c\" ", node->value.ch);
        break;
    
    case TYPE_NUM:
        fprintf(fp, "( \"%d\" ", node->value.d);

    case TYPE_VAR:
        fprintf(fp, "( \"%s\" ", node->value.str);

    default:
        break;
    }
}


char* tree_read (tree_t* ak, const char* file_name)
{
    ssize_t size = 0;
    char* buffer = read_from_file_to_buffer(&size, file_name);
    char* ptr = buffer;
    free(ak->root);

    ak->root = node_read(&buffer);
    printf(CYAN "READ BASE\n" RESET);

    return ptr;
}

void skip_spaces(char** cur_pos)
{
    while (isspace(**cur_pos))
        (*cur_pos)++;
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
        new_node->answer = get_name(cur_pos);

        skip_spaces(cur_pos);
        if (**cur_pos == '(')
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
        printf(RED "NO \"(\" in the end of node\n" RESET);
    }
    return 0;
}

char* get_name(char** cur_pos)
{
    char* ptr1 = 0;
    if (**cur_pos == '\"')
    {
        (*cur_pos)++;
        ptr1 = *cur_pos;
        char* ptr2 = strchr(*cur_pos, '\"');
        *ptr2 = '\0';
        *cur_pos += ptr2-ptr1 + 1;
    }   

    else 
    {
        printf("AK_ERROR in get node name\n");
    }
    return ptr1;
}

ssize_t getline(char **ptr_line, size_t *ptr_length, FILE *stream)
{
    if (ptr_line == NULL || ptr_length == NULL || stream == NULL)
    {
        perror("Null-terminator");
        return -1;
    }
    
    if (*ptr_length == 0 || *ptr_line == NULL)
    {
        *ptr_length = 128;
        *ptr_line = (char*)calloc(*ptr_length, sizeof(char));

        if (*ptr_line == NULL)
            return -1;
    }
    
    size_t i = 0;
    int c = fgetc(stream);
    
    while (c != EOF)
    {
        if (i >= *ptr_length - 1)
        {
            *ptr_length *= 2;
            char *new_buffer = (char*)realloc(*ptr_line, *ptr_length);

            if (new_buffer == NULL)
            {
                if (i > 0) {
                    (*ptr_line)[i] = '\0';
                }
                return -1;
            }

            *ptr_line = new_buffer;
        }
        
        (*ptr_line)[i++] = (char)c;
        
        if (c == '\n') {
            break;
        }

        c = fgetc(stream);
    }
    
    if (i < *ptr_length)
        (*ptr_line)[i] = '\0';
    else {
        *ptr_length += 1;
        char *new_buffer = (char*)realloc(*ptr_line, *ptr_length);

        if (new_buffer != NULL)
        {
            *ptr_line = new_buffer;
            (*ptr_line)[i] = '\0';
        }
    }
    
    if (i == 0 && c == EOF) {
        return -1;
    }
    
    return (ssize_t)i;
}