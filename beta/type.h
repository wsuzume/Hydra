#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#ifdef ENVIRONMENT64
typedef int64_t     Int;
typedef uint64_t    Nat;
typedef int32_t     HalfInt;
typedef uint32_t    HalfNat;
#endif

#ifdef ENVIRONMENT32
typedef int32_t     Int;
typedef uint32_t    Nat;
typedef int16_t     HalfInt;
typedef uint16_t    HalfNat;
#endif

typedef uint8_t   Nat8;
typedef uint16_t  Nat16;
typedef uint32_t  Nat32;
typedef uint64_t  Nat64;
typedef int8_t    Int8;
typedef int16_t   Int16;
typedef int32_t   Int32;
typedef int64_t   Int64;
typedef char      Byte;
typedef char16_t  Word;
typedef char32_t  Char;
typedef size_t    Index;
typedef intptr_t  Address;
typedef void *    Pointer;
typedef float     Float;
typedef double    Double;
typedef bool      Bool;

#define INF INT32_MIN
typedef int64_t dhid_t[2];
typedef int64_t hid_t;
typedef int32_t hhid_t;


enum TYPENUM {
    UNDEFINED = 0,
    VOID,
    NAT,
    INT,
    
    FLOAT,
    DOUBLE,

    BYTESTRING,
    OBJECT,
    ARRAY,

    TYPE,

    POINTER,
    FUNCTION,
    FUNCTION_END,
    PAIR,
    LIST,
    TOKEN,
    INVALID,
};

enum Ord { LT = -1, EQ = 0, GT = 1 };

typedef Nat32  type_t;
typedef Nat32* Type;

typedef struct pair_t* Pair;
typedef struct pair_t* List;
struct pair_t {
    uintptr_t car;
    uintptr_t cdr;
};

#define nil (uintptr_t)NULL

typedef struct bytestring_t* ByteString;
struct bytestring_t {
    size_t size;
    char *data;
};

struct object_t* Object;
struct object_t {
    Type type;
    void *data;
};


typedef struct atom_t* Atom;
struct atom_t {
    Type type;
    union {
        Int id;
        hid_t hid;
        ByteString name;
    } data;
};


type_t getTopLevelType(Type t);
Type printType(Type t);


void print(Type t, void *a);
