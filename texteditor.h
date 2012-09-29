#include<stdio.h>

#define BLOCKSIZE 256

typedef int key_t;
typedef int height_t;
typedef char object_t;

typedef struct node_text
{
        key_t key;
        height_t height;
        struct node_text *left;
        struct node_text *right;
        struct node_text *parent;
} text_t;

text_t *currentblock = NULL, *free_list = NULL;
int size_left;
int nodes_taken = 0;
int nodes_returned = 0;

text_t* create_text();
int length_text(text_t *txt);
char* get_line(text_t *txt, int index);
void append_line(text_t *txt, char* new_line);
char* set_line(text_t *txt, int index, char* new_line);
void insert_line(text_t *txt, int index, char* new_line);
char* delete_line( text_t *txt, int index);
