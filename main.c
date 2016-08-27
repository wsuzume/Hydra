#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "surface.h"

void *dlget(void *handle, const char *symbol)
{
    void *fun = dlsym(handle, symbol);
    if (fun == 0) {
        fprintf(stderr, "%s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    return fun;
}

int main(int argc, char *argvs[])
{
    void *handle = dlopen("./beta/hydmaster.so", RTLD_LAZY);

    if (handle == (void *)0) {
        fprintf(stderr, "%s\n", dlerror());
        return EXIT_FAILURE;
    }

    List (*mbsl)(int, char **) = dlget(handle, "makeByteStringList");
    void (*printList)(List) = dlget(handle, "printList");
    void (*fbsl)(List) = dlget(handle, "freeByteStringList");

    void (*compile)(void) = dlget(handle, "compile");

    List arglist = (*mbsl)(argc, argvs);
    (*printList)(arglist);  putchar('\n');
    (*fbsl)(arglist);

    compile();

    if (dlclose(handle) != 0) {
        fprintf(stderr, "%s\n", dlerror());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
