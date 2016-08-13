#include <stdio.h>
#include <stdlib.h>

#include "type.h"
#include "object.h"
#include "master.h"

void compile()
{
    puts("Hello, master.");

    ByteString code = byteStringFromFile("master.hydra");
    if (code == NULL) {
        puts("Hydra: fatal error in loading hydra core program.");
        exit(EXIT_FAILURE);
    }

    //printf("%ld", size);
    printlnByteString(code);

    //int ret = hydra(argc, argv, code, size);
    freeByteString(code);
    return;
}
