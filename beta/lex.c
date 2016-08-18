#include <stdio.h>
#include <string.h>

#include "type.h"
#include "object.h"
#include "lex.h"


Token makeToken(const char *atom, size_t size, uint32_t type)
{
    Token ret = (Token)malloc(sizeof(struct token_t));
    ret->type = type;
    ret->data = NULL;
    if (atom == NULL) {
        ret->atom = NULL;
    } else {
        ret->atom = allocByteString(size + 1);
        memcpy(ret->atom->data, atom, size);
        ret->atom->data[size] = '\0';
    }
    return ret;
}

Token freeToken(Token token)
{
    if (token != NULL) {
        freeByteString(token->atom);
        free(token->data);
        free(token);
    }

    return NULL;
}

const char *HYDRA_TOKEN_TYPE[TAG_INVALID] = {
    //lexerによる解析時点でのタグ
    "PROGRAM_HEAD",
    "COMMA",
    "INDENT",

    "LEXATOM",
    "ROUND_BRA",
    "ROUND_KET",
    "SQUARE_BRA",
    "SQUARE_KET",
    "BRACE_BRA",
    "BRACE_KET",

    "ROUND",
    "ROUND_WITH_COMMA",
    "SQUARE",
    "SQUARE_WITH_COMMA",
    "BRACE",
    "BRACE_WITH_COMMA",

    "LEXBYTESTRING",
    "LEXINTEGER",

    //parserによる解析時点でのタグ
    "TAG_OBJLIST",          //(a b c)       #ObjList or #@(Type -> Type .. )
    "TAG_OBJLIST_DATA",     //`(a b c)      #`ObjList
    "TAG_LIST",             //(a, b, c)     #(Type)     ..List
    "TAG_PROCEDURE",        //[a b c]       #Procedure or #@[Type->Type .. ]
    "TAG_PROCEDURE_DATA",   //`[a b c]      #`Procedure
    "TAG_DATAFIELD",        //[a, b, c]     #[Type1, Type2, .. ] or #DataField
    "TAG_FIELD",            //{a b c}       #Field
    "TAG_FIELD_DATA",       //`{a b c}      #`Field
    "TAG_ARRAY",            //{a, b, c}     #{Type}     ..Array
    "TAG_MAP",              //Dictionary, unimpremented now.

    "TAG_OBJECT",

    "TAG_VAR_INT",
    "TAG_CONST_INT",

    "TAG_VAR_BYTESTRING",
    "TAG_CONST_BYTESTRING",
};

void printToken(Token token)
{
    switch (token->type) {
        case INDENT:
            printf("{%s, width=%ld}\n", HYDRA_TOKEN_TYPE[token->type], *((size_t *)token->data));
            break;
        default:
            printf("{%s, %s}\n", HYDRA_TOKEN_TYPE[token->type], token->atom->data);
            break;
    }

    return;
}

List makeTokenList(void)
{
    List ret = (List)malloc(sizeof(struct pair_t));
    ret->car = (uintptr_t)makeSimpleType(TOKEN);
    ret->cdr = nil;
    return ret;
}

void freeTokenList(List xs)
{
    List cur = (List)xs->cdr;
    List buf;

    while (cur != NULL) {
        buf = (List)cur->cdr;
        freeToken((Token)cur->car);
        free(cur);
        cur = buf;
    }

    free((void *)xs->car);
    free(xs);
    return;
}

List appendTokenList(List xs, Token x)
{
    if (xs == NULL) 
        xs = cons(x, makeTokenList());
    else
        xs->cdr = (uintptr_t)makePair((uintptr_t)x, nil);
    return (List)xs->cdr;
}

void printTokenList(List xs)
{
    puts("printTokenList:");
    List cur = (List)xs->cdr;

    if (cur == NULL) {
        puts("No nodes in tokenlist.");
        return;
    }

    while (cur != NULL) {
        printToken((Token)cur->car);
        cur = (List)cur->cdr;
    }

    return;
}




bool isSep(char c)
{
    switch (c) {
        case ' ':
        case '\t':
        //case '\n':
        //case '\0':
            return true;
    }

    return false;
}

bool isLineFeed(char c)
{
    return c == '\n' ? true : false;
}

bool isEndPoint(char c)
{
    return (c == '\n' || c == '\0') ? true : false;
}

bool isSep_Ex(char c)
{
    switch (c) {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
        case ',':
            return true;
    }

    return false;
}

uint32_t isParen(char c)
{
    if (c == '(' || ')' == c) return ROUND;
    if (c == '[' || ']' == c) return SQUARE;
    if (c == '{' || '}' == c) return BRACE;
    return 0;
}

uint32_t isParenL(char c)
{
    if (c == '(') return ROUND_BRA;
    if (c == '[') return SQUARE_BRA;
    if (c == '{') return BRACE_BRA;
    return 0;
}

uint32_t isParenR(char c)
{
    if (')' == c) return ROUND_KET;
    if (']' == c) return SQUARE_KET;
    if ('}' == c) return BRACE_KET;
    return 0;
}

uint32_t isParenR_Ex(char c)
{
    if (')' == c) return ROUND_KET;
    if (']' == c) return SQUARE_KET;
    if ('}' == c) return BRACE_KET;
    if (',' == c) return COMMA;
    return 0;
}

uint32_t isParen_Ex(char c)
{
    if (c == '(') return ROUND_BRA;
    if (c == '[') return SQUARE_BRA;
    if (c == '{') return BRACE_BRA;
    if (')' == c) return ROUND_KET;
    if (']' == c) return SQUARE_KET;
    if ('}' == c) return BRACE_KET;
    if (',' == c) return COMMA;
    return 0;
}

bool isDoubleQuote(char c)
{
    return c == '\"' ? true : false;
}

void skipSep(char *data, size_t *i)
{
    while (isSep(data[*i])) (*i)++;
}

size_t tabwidth = 4;

void setTabWidth(size_t n)
{
    tabwidth = n;
}

Token getIndent(char *data, size_t *i)
{
    size_t *w = (size_t *)malloc(sizeof(size_t));

    for (*w = 0; ; (*i)++) {
        switch (data[*i]) {
            case ' ':
                *w += 1;
                continue;
            case '\t':
                *w += tabwidth;
                continue;
            default:
                break;
        }
        break;
    }

    Token ret = makeToken(NULL, 0, INDENT);
    ret->data = w;
    return ret;
}

Token getStringLiteral(char *data, size_t *i)
{
    char *head = &data[*i+1];    //先頭のダブルクオートを読み飛ばし
    size_t j;
    for (j = 0; ; j++) {
        //putchar(head[j]);
        if (head[j] == '\0') {
            perror("parse error getStringLiteral");
            printf("%d\n", head[j]);
            exit(EXIT_FAILURE);
        }
        if (head[j] == '\\') {
            j++; continue;  //エスケープシーケンス
        }
        if (head[j] == '\"') break;
    }
    //+2はダブルクオート2個分の加算
    *i += j+2;
    return makeToken(head, j, LEXBYTESTRING);
}

bool isNum(char c)
{
    return ('0' <= c && c <= '9') ? true : false ;
}

Token getNumberLiteral(char *data, size_t *i)
{
    //現在符号なし整数のみに対応
    char *head = &data[*i];
    size_t j = 0;
    for (;; j++) {
        if (isNum(head[j])) continue;
    }
    *i += j;
    return makeToken(head, j, LEXINTEGER);
}

bool isAtomChar(char c)
{
    if (c == '=') return true;
    if (c == ':') return true;
    if (c == '!') return true;
    if (0x23 <= c && c <= 0x26) return true;
    if (c == '-') return true;
    if (c == '+') return true;
    if (c == '*') return true;
    if (c == '.') return true;
    if (0x30 <= c && c <= 0x5a) return true;
    if (0x5e <= c && c <= 0x7a) return true;

    return false;
}

Token getAtom(char *data, size_t *i)
{
    char *head = &data[*i];
    size_t j = 0;
    while (isAtomChar(head[j])) j++;
    *i += j;
    uint32_t type = isParenL(head[j]);
    if (type == 0) type = LEXATOM;
    return makeToken(head, j, type);
}

Token getToken(char *data, size_t *i)
{
    uint32_t type;

    type = isParen_Ex(data[*i]);
    if (type) {
        //'(', ')', '{', '}', '[', ']', ',' のいずれかであり、この1文字で単独のトークン
        (*i)++;
        return makeToken("", 0, type);
    } else if (isDoubleQuote(data[*i])) {
        //'\"' が開始点。文字列リテラルである。
        return getStringLiteral(data, i);
    } else if (isNum(data[*i])) {
        //数字が開始点。10進,16進,浮動小数のいずれか。
        return getNumberLiteral(data, i);
    } else if (isAtomChar(data[*i])) {
        //計算機名、型名、モジュール名、型コンストラクタ、
        //定数名、変数名、関数名など名前に用いられる識別子などが
        //ひとまとめにしてLEXATOMとして分割される。
        //これらの名前解決はASTのほうで行われる。
        return getAtom(data, i);
    } else {
        //解析不能な文字を発見した場合。
        //全角スペースは特に恨みがましいエラーを表示すること。
        perror("parse error unknown character");
        putchar(data[*i]);
        exit(EXIT_FAILURE);
    }

    return NULL;
}

List lex(ByteString code)
{
    printlnByteString(code);

    char header[] = "PROGRAM_HEAD";
    List list = makeTokenList();
    List last = appendTokenList(list, makeToken(header, strlen(header), PROGRAM_HEAD));

    char *lim = &(code->data[code->size]);
    char *data = code->data;
    size_t i = 0;

    while (data <= lim) {
        //行頭のインデントを解析
        last = appendTokenList(last, getIndent(data, &i));
        data += i; i = 0;

        //行単位の解析
        while (!isEndPoint(data[i])) {
            skipSep(data, &i);
            data += i; i = 0;
            last = appendTokenList(last, getToken(data, &i));
            data += i; i = 0;
        }

        //行末の改行をスキップ
        data += 1;
        //break;
    }

    return list;
}

/*
AST makeASTnode(Token token, AST root, AST next, AST leaf)
{
    AST ret = (AST)malloc(sizeof(struct ast_t));
    ret->token = token;
    ret->root = root;
    ret->next = next;
    ret->leaf = leaf;
    return ret;
}

AST appendASTleaf(AST ast)
{
    //葉を追加してそのアドレスを返す
}

AST appendASTnext(AST ast)
{
    //末尾に追加してそのアドレスを返す
}

AST getLastLeaf(AST ast)
{
    //葉の末尾を返す
}

bool hasComma(TokenList list, uint32_t type)
{
    //ここでコンマの有無と同時に括弧内コンマの文法エラーをチェック
    //どうせ再帰呼び出しされるので、括弧内括弧の中までは検査しない
    return false;
}

TokenList nextNode(TokenList list)
{
    TokenList next = list->next;
    free(list);
    return next;
}

static inline bool isBra(int tokentype)
{
    return tokentype == ROUND_BRA || tokentype == SQUARE_BRA || tokentype == BRACE_BRA;
}

static inline bool isKet(int tokentype)
{
    return tokentype == ROUND_KET || tokentype == SQUARE_KET || tokentype == BRACE_KET;
}
*/

/*
AST toAST(AST parent, TokenList tokenlist)
{
    //終了条件
    if (tokenlist == NULL) return parent;

    //親が空ならプログラム開始点を作成、そうでなければノードをとりあえず水平に追加
    AST ast = parent != NULL ? makeASTnode(NULL, NULL, parent, NULL) : makeASTnode(NULL, NULL, NULL, NULL);
    AST astcur;
    
    TokenList list = tokenlist;
    TokenList next;

    Token cursol = list->token;

    switch (cursol->type) {
        case PROGRAM_HEAD:
            //現在地を元にして水平にグローバルなトークンを展開
            next = list->next;
            ast->token = cursol;
            return toAST(ast, list);

        //case LEXATOM:        処理が簡単なので埋め込み済み
        //case LEXBYTESTRING:  処理が簡単なので埋め込み
        case ROUND_BRA:
        case SQUARE_BRA:
        case BRACE_BRA:
            //コンマ付きかどうか判定
            if (cursol->type == ROUND_BRA) {
                cursol->type = hasComma(list, ROUND_BRA) ? ROUND_WITH_COMMA : ROUND;
            } else if (cursol->type == SQUARE_BRA) {
                cursol->type = hasComma(list, SQUARE_BRA) ? SQUARE_WITH_COMMA : SQUARE;
            } else {
                cursol->type = hasComma(list, BRACE_BRA) ? BRACE_WITH_COMMA : BRACE;
            }

            ast->token = cursol;    //追加したノードに現在のトークンを設定

            list = nextNode(list);
            cursol = list->token;

            if (isKet(cursol->type)) {
                //中身なし
                freeToken(cursol);
                list = nextNode(list);
                return toAST(ast, list);
            }

            while (!isKet(cursol->type)) {
                if (cursol->type == COMMA) {
                    //読み捨て
                    freeToken(cursol);
                    list = nextNode(list);
                    cursol = list->token;
                } else if (cursol->type == LEXATOM || cursol->type == LEXBYTESTRING) {
                    if (ast->leaf == NULL) {
                        astcur = appendASTleaf(cursol, ast);        //ひとつ目なら葉を追加
                    } else {
                        astcur = appendASTnext(cursol, astcur);     //それ以降なら葉に追加
                    }
                    list = nextNode(list);
                    cursol = list->token;
                } else if (isBra(cursol->type)) {
                    //括弧が来たら再帰的に追加する
                    astcur = toAST(ast, list);
                }
            }

            //ketを発見してここに来ているので削除
            freeToken(cursol);
            list = nextNode(list);

            return toAST(ast, list);

        //case ROUND_KET:   未到達点
        //case SQUARE_KET:  未到達点
        //case BRACE_KET:   未到達点
        //case COMMA:       未到達点
        default:
            break;
    }

    return toAST(ast, list);
}

void interpret(AST root)
{
    return;
}


*/
