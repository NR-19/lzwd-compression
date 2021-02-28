#include "lzwd.h"

//Função que cria o dicionário com os primeiros 256 padrões correspondentes ao código ASCII
dict * create_dict(){
    dict * d = (dict *) malloc(sizeof(dict));
    d->max_key = 256;
    for(int i = 1; i <= d->max_key; i++){
        d->dictionary[i].content = (char *) realloc(d->dictionary[i].content, 2 * sizeof(char));
        d->dictionary[i].key = i;
        char ch[2] = {(char) i};
        strcpy(d->dictionary[i].content, ch);
    }
    return d;
}

//Função que dá reset ao dicionário quando atinge o MAX_SIZE
void reset_dict(dict * d){
    *d = *create_dict();
}

//verificar se um dado padrão se encontra no dicionário
//1 -> Encontrou  ;  0 -> Não Encontrou
int in_dict(dict d, int key_min, char * s_content){
    if(strlen(s_content) == 1) return 1;  //correspondente aos 256 primeiros símbolos
    if(key_min > d.max_key) return 0;
    for(int i = key_min; i <= d.max_key; i++){
        if(strcmp(d.dictionary[i].content, s_content) == 0) return 1;
    }
    return 0;
}

//Função que verifica se uma dada "substring" se encontra no dicionário
// 0-> "Substring Não Encontrada" ; 1-> "Substring Encontrada"
int sub_in_dict(dict d, int key_min, char * s_content){
    if(key_min > d.max_key || strlen(s_content) == 1) return 0;
    for(int i = key_min; i <= d.max_key; i++){
        if(strlen(s_content) < strlen(d.dictionary[i].content)){
            int check = 1;
            for(int a = 0; a < strlen(s_content) && check == 1; a++){
                if(s_content[a] != d.dictionary[i].content[a]) check = 0;
            }
            if(check == 1) return 1;
        }
    }
    return 0;
}

//Função que adiciona um novo padrão ao dicionário
int add_element(dict * d, char * new_content){
    if(d->max_key == MAX_SIZE){ //caso em que o tamanho máximo é excedido e então ocorre um resete ao dicionário 
        reset_dict(d);
        return 0;
    }
    if(in_dict(*d, 1, new_content) == 1) return 0;
    d->max_key++;
    d->dictionary[d->max_key].key = d->max_key;
    unsigned long content_size = strlen(new_content) * sizeof(char) + 1;
    d->dictionary[d->max_key].content = (char *) realloc(d->dictionary[d->max_key].content, content_size);
    strcpy(d->dictionary[d->max_key].content, new_content);
    return 1;
}

//Função que obtem o código (key) dum dado padrão 
//0-> "Não Encontrado" ; Outro Valor -> Key
int get_key(dict d, int key_min, char * s_content){
    if(key_min > d.max_key) return 0;
    for(int i = key_min; i <= d.max_key; i++){
        if(strcmp(d.dictionary[i].content, s_content) == 0) return i;
    }
    return 0;
}

//Função que obtem o código (key) dum dado padrão com 16 bites
//0-> "Não Encontrado" ; Outro Valor -> Key
short get_key_16(dict d, short key_min, char * s_content){
    if(key_min > d.max_key) return 0;
    for(short i = key_min; i <= d.max_key; i++){
        if(strcmp(d.dictionary[i].content, s_content) == 0) return i;
    }
    return 0;
}

//Função que coloca os parentises a volta do codigo e passa por referencia o resultado
void output_generator_codes(dict d, char * new_string, char * result){
    int nkey = get_key(d, 1, new_string);
    char to_add[MAX_CODE_DIG] = "(";
    char number[MAX_CODE_DIG];
    sprintf(number, "%d", nkey);
    strcat(to_add, number);
    strcat(to_add, ")");
    strcpy(result, to_add);
}

/*
//Função que coloca os parentises a volta dos padrões e passa por referencia o resultado
void output_generator_string(dict d, char * new_string, char * result){
    char to_addch[MAX_CODE_DIG] = "(";
    strcat(to_addch, new_string);
    strcat(to_addch, ")");
    strcpy(result, to_addch);
}
*/

//Função que junta as funções 'output_generator_codes' e 'output_generator_string'
void output_generator(dict d, char * new_string, char * result, int debug){
    if(strlen(new_string) >= 2 || debug == 1) return output_generator_codes(d, new_string, result);
    //return output_generator_string(d, new_string, result);
}

//Função que cria um novo padrão a partir de padrões já existentes e adiciona ao dicionário
int make_pattern(char * first_pattern, char * second_pattern, dict * d){
    int len = strlen(first_pattern) + strlen(second_pattern) + 1;
    char new_pattern[len];
    strcpy(new_pattern, "");
    strcat(new_pattern, first_pattern);
    strcat(new_pattern, second_pattern);
    return add_element(d, new_pattern);
}

//Função encontra o novo padrão   
//0-> Erro ; Outro-> Char seguinte ao padrão
int find_pattern(char * p_string, unsigned long max_len, char * pattern, dict d, int N, int key_min){ 
    if(N == strlen(p_string) - 1){
        strcpy(pattern, &p_string[strlen(p_string) - 1]); //Caso em que o último caracter não pertence a nenhum padrão
        return N + 1;     //caso em está no fim da string
    }
    if(N >= strlen(p_string)) return 0;
    char temp[] = "";
    char ch = p_string[N];
    strncat(temp, &ch, 1);
    char first_pattern[max_len];
    strcpy(first_pattern, temp);     //first_pattern e second_pattern inicializadas com o mesmo símbolo
    char second_pattern[max_len];
    strcpy(second_pattern, first_pattern);
    int done = 1;
    int kmin = 1;
    while(N < strlen(p_string) && done == 1 && strlen(second_pattern) <= MAX_PAT_LEN){
        N++;
        if(in_dict(d, kmin, second_pattern) == 1){ //se estiver no dicionário
            strcpy(first_pattern, second_pattern);
            if(N < strlen(p_string)) strncat(second_pattern, &p_string[N], 1);
        }
        else{
            if(sub_in_dict(d, kmin, second_pattern) == 1){
                if(N < strlen(p_string)) strncat(second_pattern, &p_string[N], 1);
            }
            else{
                N--; //uma vez que foi aumentado para verificar se a sub-string se encontra no dicionario e essa verificação nao teve sucesso, o N é reduzidio
                done = 0; //não é compativel e termina o ciclo
            }
        }

        kmin = key_min; //uma vez analizado o primeiro padrão (um dos 256 padroes do codigo ASCII), os próximos serão padrões novos a partir do código FIRST_PATTERN_DIC
    }
    strcpy(pattern, first_pattern);
    return N;
}

/*
void print_dic(dict * d){
    for(int i = 1; i <= d->max_key; i++) printf("Key %d -> Content %s \n", d->dictionary[i].key, d->dictionary[i].content);
}
*/

//função que coloca um valor (content) num buffer sempre na primeira posição que estiver livre, considerando que uma posição estar livre é ter o seu conteudo igual a zero
int store_in_buffer(short * buffer, int size, short content){
    for(int i = 0; i < size; i++){
        if(buffer[i] == 0){
            buffer[i] = content;
            return 1;
        }
    }
    return 0;
}
/*
* Função que processa a string, de tal forma que a cada iteração identifica dois padrões consecutivos, adiciona ao dicionario um novo padrão resultante 
* da concatenação dos dois padroes referidos e por fim adiciona a um buffer de outputs o codigo do dicionario correspondente ao primeiro padrão.
* Itera até chegar ao fim da p_string.
*/
int process_string(dict *di, char * p_string, char *output, short * binary_buffer, int bbsize, int debug){
    unsigned long max_pattern_len = (strlen(p_string) / 2) + 1; //pior caso, ser metade da p_string + o '\0'
    int N = 0;
    int final_char = 0; //variavel que guarda a posição quando acaba o second_pattern
    int state = 1;
    char first_pattern[max_pattern_len];
    char second_pattern[max_pattern_len];

    while(state == 1){
        N = find_pattern(p_string, max_pattern_len, first_pattern, *di, N, FIRST_PATTERN_DIC);

        if(N == strlen(p_string)){  //caso em que chega ao fim da string no final do primeiro padrão
            state = 0;
        }

        final_char = find_pattern(p_string, max_pattern_len, second_pattern, *di, N, FIRST_PATTERN_DIC);
        make_pattern(first_pattern, second_pattern, di);  //junta os dois e coloca no dicionário

        if(final_char == strlen(p_string)){  //caso em que chega ao fim da string no final do segundo padrão
            state = 2;
        }

        char outp[MAX_CODE_DIG];
        output_generator(*di, first_pattern, outp, debug); 
        strcat(output, outp); 

        store_in_buffer(binary_buffer, bbsize, get_key_16(*di, 1, first_pattern));

        if(state == 2){ //acabando no segundo padrão, tambem o inclui no buffer de saída 
            output_generator(*di, second_pattern, outp, debug);
            store_in_buffer(binary_buffer, bbsize, get_key_16(*di, 1, second_pattern));
            strcat(output, outp);
        }
    }
    //print_dic(di);
    return 1;
}

//Função que coloca todos os elementos do buffer a zero porque vai ser considerada uma posição livre
void fill_buffer(short * buffer, int size, short content){
    for(int i = 0; i < size; i++){
        buffer[i] = content;
    }
}

//Função que retorna o número de elementos nao nulos, ou seja, os elementos que estão a ser ocupados no buffer
int buffer_size(short * buffer, int max_size){
    for(int i = 0; i < max_size; i++){
        if(buffer[i] == 0) return i;
    }
    return max_size;  //tamanho 
}

//Função que processa por blocos
void lzwd(char *filename, int debug){ 
    FILE *f;
    long long n_blocks;
    unsigned long block_size = BLOCK_SIZE;
    unsigned long size_of_last_block;
    char *buffer_e, *buffer_s;

    time_t t = time(NULL);
    double time_spent = 0.0;
    clock_t begin = clock();

    f = fopen(filename, "r");
    if (f == NULL) return;


    n_blocks = fsize(f, NULL, &block_size, &size_of_last_block);  //número de blocos

    if(n_blocks == 1)    //caso em que há um único bloco
        block_size = size_of_last_block;
    else if (n_blocks == 0) return;

    int bbsize = block_size * (n_blocks-1) + size_of_last_block;
    short buffer_binary_s[bbsize];
    fill_buffer(buffer_binary_s, bbsize, 0);  //coloca o buffer_binary_s a zeros

    buffer_e = (char *) malloc(sizeof(char)*block_size); //alocar memória para um buffer com o tamanho do BLOCK_SIZE

    buffer_s = (char *) malloc(sizeof(char)*block_size*(n_blocks-1)*6+size_of_last_block*6); 


    strcpy(buffer_s,""); //garantir que o buffer de saída está vazio
    //printf(".");
    dict di = *create_dict(); 

    for(int i = 0; i < n_blocks; i++){       //processamento por blocos
        if(i == n_blocks - 1){               //último bloco
            buffer_e = (char *) realloc(buffer_e,sizeof(char) * size_of_last_block);
            fread(buffer_e, size_of_last_block, sizeof(char), f);
            buffer_e[size_of_last_block] = '\0';  //coloca '\0' no fim da string para garantir que caracteres extra nao sejam contados
            process_string(&di, buffer_e, buffer_s, buffer_binary_s, bbsize, debug);
        }
        else {                               //outros blocos
            fread(buffer_e, sizeof(char), block_size, f);
            buffer_e[block_size] = '\0';     //coloca '\0' no fim da string para garantir que caracteres extra não sejam contados
            process_string(&di, buffer_e, buffer_s, buffer_binary_s, bbsize, debug);
        }
    }

    bbsize = buffer_size(buffer_binary_s, bbsize);  //tamanho final do bbsize 

    fclose(f);
    free(buffer_e);

    char *new_filename = strcat(filename, ".lzwd");

    if(debug == 0){  //ficheiro binário
        FILE *bfs = fopen(new_filename, "wb");
        fwrite(buffer_binary_s,2, bbsize, bfs); //2 bytes -> 16 bits
        fclose(bfs);
    }
    else{  //ficheiro com modo debug ativado
        FILE *fs = fopen(new_filename, "w");
        fprintf(fs,"%s", buffer_s);
        fclose(fs);
    }

    free(buffer_s);
    free(buffer_binary_s);

    clock_t end = clock();
    time_spent = ((double)(end - begin) / CLOCKS_PER_SEC) * 1000;


    printf("\nNelson Ribeiro, a93188, MIEI/CD, 14-fev-2021\n");
    printf("Numero de blocos: %d\n", n_blocks);
    printf("Tempo de execucao do modulo: %.2f ms\n",time_spent);
    printf("Ficheiro gerado: %s\n\n", new_filename);
}


int main(){
    char filename[25] = "aaa.txt";
    int debug = 1;
    lzwd(filename,debug);
    return 0;
}
