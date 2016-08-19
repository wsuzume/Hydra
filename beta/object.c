#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "object.h"

//(null文字除いて)n文字分の領域を確保する
//sizeには格納できる文字数(n)が保存される
//安全のため、先頭と末尾にはnull文字が代入される
ByteString allocByteString(size_t n)
{
    ByteString ret = (ByteString)malloc(sizeof(struct bytestring_t));

    ret->size = n;
    ret->data = (char *)malloc((n+1) * sizeof(char));
    ret->data[0] = '\0';
    ret->data[n] = '\0';

    return ret;
}

ByteString makeByteString(const char *str)
{
    size_t len = strlen(str);
    ByteString ret = allocByteString(len + 1);
    strncpy(ret->data, str, len);
    return ret;
}

ByteString freeByteString(ByteString str)
{
    if (str != NULL) {
        free(str->data);
        free(str);
    }

    return NULL;
}

int compByteString(ByteString str1, ByteString str2)
{
    char *s1 = str1->data;
    char *s2 = str2->data;

    while ((*s1 == *s2) && *s1) {
        s1++;
        s2++;
    }

    return (*s1 == *s2) ? 0 :
           (*s1 < *s2)  ? 1 : -1;
}

void printByteString(ByteString str)
{
    putchar('\"');
    fputs(str->data, stdout);
    putchar('\"');
}

void printlnByteString(ByteString str)
{
    putchar('\"');
    fputs(str->data, stdout);
    putchar('\"');
    putchar('\n');
}

ByteString appendByteString(ByteString str1, const char *str2)
{
    size_t len1 = strlen(str1->data);
    size_t len2 = strlen(str2);
    size_t maxlen = len1 + len2 + 1;
    char *buf;

    if (str1->size < maxlen) {
        buf = realloc(str1->data, maxlen);
        if (buf == NULL) {
            perror("Bad alloc in appendByteString()");
            return str1;
        }
        str1->size = maxlen;
        str1->data = buf;
    }

    strncpy(str1->data + len1, str2, len2);
    return str1;
}

ByteString byteStringFromFile(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return NULL;
    }

    fseek(fp, 0L, SEEK_END);
    size_t n = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    ByteString ret = allocByteString(n-1);
    size_t size = fread(ret->data, 1, n-1, fp);
    if (size == 0) {
        free(ret->data);
        free(ret);
        ret = NULL;
    }

    fclose(fp);
    return ret;
}





List makeList(Type type)
{
    List ret = (List)malloc(sizeof(struct pair_t));
    ret->car = (uintptr_t)type;
    ret->cdr = nil;
    return ret;
}

Pair makePair(uintptr_t x, uintptr_t y)
{
    Pair ret = (Pair)malloc(sizeof(struct pair_t));
    ret->car = x;
    ret->cdr = y;
    return ret;
}

List cons(void *x, List xs)
{
    xs->cdr = (uintptr_t)makePair((uintptr_t)x, xs->cdr);
    return xs;
}

void *head(List xs)
{
    if (xs->cdr == nil) return NULL;

    Pair p = (Pair)xs->cdr;
    void *ret = (void *)p->car;
    xs->cdr = p->cdr;
    free(p);

    return ret;
}

void freeList(List xs)
{
    free(xs);
    return;
}

void printList(List xs)
{
    Type t = (Type)xs->car;
    List cur = (List)xs->cdr;

    putchar('[');
    if (cur != NULL) {
        print(t, (void *)cur->car);
        cur = (List)cur->cdr;
    }
    while (cur != NULL) {
        putchar(',');
        putchar(' ');
        print(t, (void *)cur->car);
        cur = (List)cur->cdr;
    }
    putchar(']');

    return;
}


//Functions below from here to THERE must be called from ../main.c
//Otherwise memory leaks may happen.

List makeByteStringList(int n, char *str[])
{
    List xs = makeList(makeSimpleType(BYTESTRING));
    int i;
    for (i = n-1; i >= 0; i--) {
        //puts(str[i]);
        cons(makeByteString(str[i]), xs);
    }
    return xs;
}

void freeByteStringList(List xs)
{
    if (xs == NULL) return;

    free((void *)xs->car);  //free Type descripter
    List cur = (List)xs->cdr;
    List buf;
    while (cur != NULL) {
        buf = (List)cur->cdr;
        freeByteString((ByteString)cur->car);
        free(cur);
        cur = buf;
    }

    free(xs);
    return;
}

//THERE

void printHID(hid_t hid)
{
    hhid_t x = getxHID(hid);
    hhid_t y = getyHID(hid);
    //printf("%d,%d", x, y);
    if (x == INF) {
        if (y == INF) {
            printf("@inf:inf");
        } else {
            printf("@inf:%d", y);
        }
    } else if (y == INF) {
        printf("@%d:inf", x);
    } else {
        printf("@%d:%d", x, y);
    }

    return;
}

hid_t view(hid_t v, hid_t hid)
{
    hhid_t x = getxHID(hid);
    hhid_t y = getyHID(hid);

    if (x == INF) {
        switch (y) {
            case INF: return hid;
            case -1: return v;
            case 0:  return hid;
            case 1:  return v;
            default: return (y > 0) ? makeHID(x, y-1) : makeHID(x, y+1);
        }
    }
    if (y == INF) {
        switch (x) {
            case INF: return hid;
            case -1: return v;
            case 0:  return hid;
            case 1:  return v;
            default: return (x > 0) ? makeHID(x-1, y) : makeHID(x+1, y);
        }
    }

    return hid;
}


Atom makeAtomHID(hid_t hid)
{
    Atom ret = (Atom)malloc(sizeof(struct atom_t));
    ret->type = HID;
    ret->data.hid = hid;
    return ret;
}

Atom makeAtomByteString(const char *p)
{
    Atom ret = (Atom)malloc(sizeof(struct atom_t));
    ret->type = BYTESTRING;
    ret->data.name = makeByteString(p);
    return ret;
}

void printAtom(Atom atom)
{
    if (atom->type == HID) {
        printHID(atom->data.hid);
    } else if (atom->type == BYTESTRING) {
        printByteString(atom->data.name);
    }
    return;
}

void freeAtom(Atom atom)
{
    if (atom->type == BYTESTRING) {
        freeByteString(atom->data.name);
    }
    free(atom);
    return;
}

int compAtom(Atom a1, Atom a2)
{
    if (a1->type < a2->type) return 1;
    if (a1->type > a2->type) return -1;

    switch (a1->type) {
        case INT:
            if (a1->data.id < a2->data.id) return 1;
            if (a1->data.id > a2->data.id) return -1;
            return 0;
        case HID:
            return compHID(a1->data.hid, a2->data.hid);
        case BYTESTRING:
            return compByteString(a1->data.name, a2->data.name);
        default:
            break;
    }

    return 0;
}


Object makeObject()
{
    return NULL;
}

Entity makeEntity()
{
    return NULL;
}

Unit makeUnit()
{
    return NULL;
}

Set makeSet()
{
    return NULL;
}
