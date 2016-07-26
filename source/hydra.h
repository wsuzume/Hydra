#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INF INT32_MIN

typedef int64_t dhid_t[2];
typedef int64_t hid_t;
typedef int32_t hhid_t;

enum HTYPE {
    NIL = 0,
    HID,
    BYTESTRING,
    INT,
    ATOM,
    SET,
    UNIT,
    TRIUNIT
};

typedef uint32_t type_t;
typedef int32_t Int;

typedef struct bytestring_t* ByteString;
struct bytestring_t {
    size_t size;
    char *data;
};


typedef struct atom_t* Atom;
struct atom_t {
    type_t type;
    union {
        Int id;
        hid_t hid;
        ByteString name;
    } data;
};


typedef struct object_t* Object;
struct object_t {
    type_t type;
    void *data;
};

#define ATOMTYPENUM 4

typedef struct array_t* Array;
struct array_t {
    size_t size;
    void *data;
};


typedef struct pair_t* Pair;
typedef struct pair_t* List;
struct pair_t {
    uintptr_t car;
    uintptr_t cdr;
};

#define nil (uintptr_t)NULL

//root[0]->hid_t        計算機番号で区別するときに使う
//root[1]->ByteString   文字列で区別するときに使う
//root[2]->Int          特に区別する名前を与えられなかったときや、順序がないオブジェクトに対して適当に番号をつけるときに使う
typedef struct set_t* Set;
struct set_t {
    type_t type;
    void *root[ATOMTYPENUM];
};

typedef struct unit_t* Unit;
struct unit_t {
    Atom atom;
    Object obj;
};

typedef struct triunit_t* TriUnit;
struct triunit_t {
    Atom atom;
    Object obj;
    Set set;
};

typedef struct procedure_t* Procedure;
struct procedure_t {
    type_t type;
};

typedef struct hydragraph_t* HydraGraph;
struct hydragraph_t {
    Atom atom;
    List vertex;
};

/*
typedef struct hydranode_t* HydraNode;
struct hydranode_t {
    uint64_t idx;
    uint64_t idy;
    void *p;
    HydraNet x; //defines Parallelism
    HydraNet y; //defines Concurrency
};

typedef struct hydragraph_t* HydraGraph;
struct hydragraph_t {
    HydraNode *node;
    intptr_t px;
    intptr_t py;
    intptr_t mx;
    intptr_t my;
};

typedef struct hydranet_t* HydraNet;
struct hydranet_t {
    HashMap table;
    void *p;
};

typedef struct bridge_t* Bridge;
struct bridge_t {
    uint64_t idx1;
    uint64_t idy1;
    uint64_t idx2;
    uint64_t idy2;
};

typedef struct neuroinfo_t* NeuroInfo;
struct neuroinfo_t {
    void *data;
    List blist;
};

*/
