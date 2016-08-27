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
    INVALID,
};

typedef Nat32  TypeNum;
typedef Nat32* Type;

struct object_t {
    Type type;
    void *data;
};

enum Ord { LT = -1, EQ = 0, GT = 1 };

Type printType(Type t);
