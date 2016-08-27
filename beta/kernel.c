#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "kernel.h"

#include <unistd.h>

typedef struct vm_t* VM;
struct vm_t {
    pthread_t pid;
    pthread_attr_t attr;
    void *code;
};

void vmfree(VM vm)
{
    free(vm->code);
    free(vm);
    return;
}

void *vmcore(void *vmptr)
{
    VM this = (VM)vmptr;
    char *head = (char *)(this->code);
    printf("%s\n", head);

    vmfree((VM)vmptr);
    return NULL;
}

VM vminit(void *code)
{
    VM ret = (VM)malloc(sizeof(struct vm_t));
    ret->code = code;
    pthread_attr_init(&(ret->attr));
    pthread_attr_setdetachstate(&(ret->attr), PTHREAD_CREATE_DETACHED);
    if(pthread_create(&(ret->pid), &(ret->attr), vmcore, ret) != 0) {
        perror("Error in pthread_create");
    }
    pthread_attr_destroy(&(ret->attr));

    return ret;
}

int vmmain(void **code, size_t n)
{
    puts("Hello, VM!");

    VM *vm = (VM *)malloc(n * sizeof(struct vm_t));
    size_t i;
    for (i = 0; i < n; i++) {
        vm[i] = vminit(code[i]);
        sleep(1);
    }

    return 0;
}
