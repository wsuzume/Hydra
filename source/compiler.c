#include "lex.h"


CodeGen compile(int argc, char *argv[])
{
    ByteString source = byteStringFromFile(argv[1]);
    printLn(source);
    TokenList tokenlist = toTokenList(source);
    printTokenList(tokenlist);
    
    //AST ast = toAST(tokenlist);

    freeByteString(source);
    freeTokenList(tokenlist);

    return NULL;
}
