#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fsize.h"
#define BLOCK_SIZE 65536 //tamanho do bloco -> 64Kbytes
#define MAX_SIZE 4096
#define MAX_CODE_DIG 7
#define MAX_PAT_LEN 4096
#define FIRST_PATTERN_DIC 257

// ** Estruturas que definem o dicionário **

//Cada elemento do dicionário tem o seu código (key) que corresponde ao seu padrão (content).
typedef struct{
    int key;
    char *content;
}dict_element;

//dicionário
typedef struct{
    int max_key;  //código do último padrão existente no dicionário
    dict_element dictionary[MAX_SIZE];
}dict;

dict * create_dict();

void reset_dict(dict * d);

int in_dict(dict d, int key_min, char *s_content);

int sub_in_dict(dict d, int key_min, char *s_content);

int add_element(dict *d, char *new_content);

int get_key(dict d, int key_min, char *s_content);

short get_key_16(dict d, short key_min, char * s_content);

void output_generator_codes(dict d, char *new_string, char *result);

//void output_generator_string(dict d, char *new_string, char *result);

void output_generator(dict d, char *new_string, char *result, int debug);

int make_pattern(char *first_pattern, char *second_pattern, dict *d);

int find_pattern(char * p_string, unsigned long max_len, char * pattern, dict d, int N, int key_min);

int store_in_buffer(short * buffer, int size, short content);

int process_string(dict *di, char * p_string, char *output, short * binary_buffer, int bbsize, int debug);

void fill_buffer(short * buffer, int size, short content);

int buffer_size(short * buffer, int max_size);

void lzwd(char *filename, int debug);

