#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "object.h"

size_t HYDRA_TYPE_SIZE[INVALID] = {
    0,
    0,
    sizeof(Nat),
    sizeof(Int),

    sizeof(Float),
    sizeof(Double),

    sizeof(Type),

    sizeof(ByteString),
    sizeof(Object),
    sizeof(Array),
};

size_t sizeof_type(Type t)
{
    if(1 <= t[0] || t[0] < TYPE) {
        return HYDRA_TYPE_SIZE[t[0]];
    }
    return 0;
}

size_t HYDRA_TYPE_ALIGN[INVALID] = {
    0,
    0,
    alignof(Nat),
    alignof(Int),
    
    alignof(Float),
    alignof(Double),
    alignof(ByteString),

    alignof(Type),

    alignof(Object),
    alignof(Array),
};

size_t alignof_type(Type t)
{
    if(1 <= t[0] || t[0] < INVALID) {
        return HYDRA_TYPE_ALIGN[t[0]];
    }
    return 0;
}

void show(Type t, void *p)
{
    return;
}


typedef void (*PrintFun)(void *);

void printUndefined(void *a) { fputs("Undefined", stdout); }
void printVoid(void *a) { fputs("Void", stdout); }
void printInt(void *a) { printf("%d", *((Int *)a)); }
void printNat(void *a) { printf("%d", *((Nat *)a)); }
void printFloat(void *a) { printf("%f", *((Float *)a)); }
void printDouble(void *a) { printf("%lf", *((Double *)a)); }
void printType_X(void *a) { printType((Type)a); }
void printByteString_X(void *a) { printByteString((ByteString)a); }
void printList_X(void *a) { printList((List)a); }

PrintFun printFun[INVALID] = {
    printUndefined,
    printVoid,
    printInt,
    printNat,
    printFloat,
    printDouble,
    printType_X,
    printByteString_X,
    printList_X
};

void print(Type t, void *a)
{
    if (0 < t[0] && t[0] < TYPE) {
        (*(printFun[t[0]]))(a);
    }
    return;
}
