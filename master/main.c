#include <stdio.h>
#include <stdlib.h>
#include "core.h"


int main(int argc, char *argv[])
{
    FILE *fp = fopen("");
    return EXIT_SUCCESS;
}


/*
int main(int argc, char *argv[])
{
    FILE *fp = fopen("master.hydra", "rb");
    if (fp == NULL) {
        puts("Hydra: fatal error: Hydra core program not found.");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp) - 1;
    fseek(fp, 0L, SEEK_SET);

    char *code = (char *)malloc(size * sizeof(char));
    fread(code, sizeof(char), size, fp);
    fclose(fp);

    //printf("%ld", size);
    //puts(code);

    int ret = hydra(argc, argv, code, size);
    free(code);
    return ret;
}
*/
