#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


int main(int argc, char *argvs[])
{
    void *handle = dlopen("./beta/hydmaster.so", RTLD_LAZY);

    if (handle == (void *)0) {
        fprintf(stderr, "%s\n", dlerror());
        return EXIT_FAILURE;
    }

    void (*compile)(void) = dlsym(handle, "compile");
    if (compile == 0) {
        fprintf(stderr, "%s\n", dlerror());
        return EXIT_FAILURE;
    }

    compile();

    if (dlclose(handle) != 0) {
        fprintf(stderr, "%s\n", dlerror());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
