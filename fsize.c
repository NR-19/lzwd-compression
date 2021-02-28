#include <stdio.h>
#include <stdlib.h>
#include "fsize.h"

int main(){

    unsigned long long total;
    long long n_blocks;
    unsigned long size_of_last_block, block_size;
    FILE *fp;

    // Using function fsize() when file is already opened
    fp = fopen("exemplos/aaa.txt", "rb");
    block_size = 2048;
    n_blocks = fsize(fp, NULL, &block_size, &size_of_last_block);
    total = (n_blocks-1) * block_size + size_of_last_block;
    printf("\n File: aaa.txt (file already opened)");
    printf("\n %ld blocks of %lu bytes + 1 block of %lu bytes", n_blocks-1, block_size, size_of_last_block);
    printf("\n File size = %llu bytes\n", total);
    fclose(fp);

    // Using function fsize() when file is not opened
    block_size = FSIZE_DEFAULT_BLOCK_SIZE;
    n_blocks = fsize(NULL, "exemplos/bbb.zip", &block_size, &size_of_last_block);
    total = (n_blocks-1) * block_size + size_of_last_block;
    printf("\n File: bbb.zip");
    printf("\n %ld blocks of %lu bytes + 1 block of %lu bytes", n_blocks-1, block_size, size_of_last_block);
    printf("\n File size = %llu bytes\n", total);

    return(0);
}