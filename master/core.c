#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <dlfcn.h>

#include "core.h"

int hydravm();

int hydra(int argc, char *argv[], void *code, long size)
{

    return hydravm();
}


int hydravm()
{
    char *rgs1;
    int flag;

    void *handle;


    switch (CODE) {
        case DLOPEN:
            handle = dlopen(rgs1, flag);
            break;
        case DLSYM:
            fun = dlsym(rgs2, rgs1);
            break;
        case DLCLOSE:
            dlclose(handle);
            break;

    }

    return EXIT_SUCCESS;
}
