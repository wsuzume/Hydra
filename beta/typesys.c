#include <stdio.h>
#include <stdlib.h>
#include "type.h"

#define TCNUM_SHOW 1

Type makeType(type_t *type, size_t n)
{
    Type ret = (Type)malloc(sizeof(type_t) * n);
    memcpy(ret, type, sizeof(type_t) * n);
    return ret;
}

type_t getTopLevelType(Type t)
{
    return t[0];
}

const char *HYDRA_TYPE_NAME[INVALID] = {
    "Undefined",
    "Void",
    "Nat",
    "Int",

    "Float",
    "Double",

    "ByteString",
    "Object",
    "Array",

    "Type",

    "Pointer",
    "Function",
    "Function_End",
    "Pair",
    "List",
    "Token",
};

//最後に表示したポインタを返す
Type printType(Type t)
{
    if (0 <= t[0] && t[0] < TYPE) {
        fputs(HYDRA_TYPE_NAME[t[0]], stdout);
        return t;
    }

    if (t[0] == POINTER) {
        putchar('*');
        t = printType(&t[1]);
        return t;
    }

    if (t[0] == FUNCTION) {
        putchar('@');
        putchar('(');
        t = printType(&t[1]);
        while (t[1] != FUNCTION_END) {
            putchar('-');
            putchar('>');
            t = printType(&t[1]);
        }
        putchar(')');
        return &t[1];
    }

    fputs(HYDRA_TYPE_NAME[0], stdout);
    return t;
}

ByteString showType(Type t)
{
    return NULL;
}

