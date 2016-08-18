#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "type.h"
#include "object.h"

enum TOKENTYPE {
    //lexerによる解析時点でのタグ
    PROGRAM_HEAD,
    COMMA,
    INDENT,

    LEXATOM,
    ROUND_BRA,
    ROUND_KET,
    SQUARE_BRA,
    SQUARE_KET,
    BRACE_BRA,
    BRACE_KET,

    ROUND,
    ROUND_WITH_COMMA,
    SQUARE,
    SQUARE_WITH_COMMA,
    BRACE,
    BRACE_WITH_COMMA,

    LEXBYTESTRING,
    LEXINTEGER,

    //parserによる解析時点でのタグ
    TAG_OBJLIST,          //(a b c)       #ObjList or #@(Type -> Type .. )
    TAG_OBJLIST_DATA,     //`(a b c)      #`ObjList
    TAG_LIST,             //(a, b, c)     #(Type)     ..List
    TAG_PROCEDURE,        //[a b c]       #Procedure or #@[Type->Type .. ]
    TAG_PROCEDURE_DATA,   //`[a b c]      #`Procedure
    TAG_DATAFIELD,        //[a, b, c]     #[Type1, Type2, .. ] or #DataField
    TAG_FIELD,            //{a b c}       #Field
    TAG_FIELD_DATA,       //`{a b c}      #`Field
    TAG_ARRAY,            //{a, b, c}     #{Type}     ..Array
    TAG_MAP,              //Dictionary, unimpremented now.

    TAG_OBJECT,

    TAG_VAR_INT,
    TAG_CONST_INT,

    TAG_VAR_BYTESTRING,
    TAG_CONST_BYTESTRING,
    TAG_INVALID
};


typedef struct codegen_t* CodeGen;
struct codegen_t {
    intptr_t head;
    intptr_t cursol;
    size_t size;
};

typedef struct token_t* Token;
struct token_t {
    ByteString atom;
    uint32_t type;
    void *data;     //you can use this member freely if type is not INDENT
                    //free(data) is called automatically in freeToken()
    uint32_t L;     //line
    uint32_t C;     //column
};

typedef struct tokenlist_t* TokenList;
struct tokenlist_t {
    Token token;
    TokenList next;
};

typedef struct ast_t* AST;
struct ast_t {
    Token token;
    AST root;
    AST next;
    AST leaf;
};

List lex(ByteString code);
void printTokenList(List xs);
void freeTokenList(List xs);

/*
int repl(int argc, char *argv[]);
CodeGen compile(int argc, char *argv[]);


List lex(ByteString code);
void printTokenList(TokenList tokenlist);
TokenList freeTokenList(TokenList tokenlist);
AST toAST(TokenList tokenlist);

*/
