#include "type.h"
#include "object.h"
#include "lex.h"


Token makeToken(const char *atom, size_t size, uint32_t type)
{
    Token ret = (Token)malloc(sizeof(struct token_t));
    ret->atom = allocByteString(size + 1);
    memcpy(ret->atom->data, atom, size);
    ret->atom->data[size] = '\0';
    ret->type = type;
    return ret;
}

Token freeToken(Token token)
{
    if (token != NULL) {
        freeByteString(token->atom);
        free(token);
    }

    return NULL;
}

void printToken(Token token)
{
    printf("{%s, tokentype=%d}\n", token->atom->data, getTopLevelType(token->type));
    return;
}

/*
List makeTokenListNode(Token token, List next)
{
    List ret = (List)malloc(sizeof(struct pair_t));
    ret->car = (uintptr_t)token;
    ret->cdr = (uintptr_t)next;
    return ret;
}

List appendTokenList(List xs, Token x)
{
    if (xs == NULL) return makeTokenListNode(x, NULL);
    xs->cdr = (uintptr_t)makeTokenListNode(x, NULL);
    return xs->next;
}

List freeTokenList(List tokenlist)
{
    List cur = tokenlist;
    List buf;

    while (cur != NULL) {
        buf = (List)cur->cdr;
        freeToken(cur->token);
        free(cur);
        cur = buf;
    }

    return NULL;
}

void printTokenList(TokenList tokenlist)
{
    puts("printTokenList:");
    TokenList cur = tokenlist;

    if (cur == NULL) {
        puts("No nodes in tokenlist.");
        return;
    }

    while (cur != NULL) {
        printToken(cur->token);
        cur = cur->next;
    }

    return;
}




bool isSep(char c)
{
    switch (c) {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            return true;
    }

    return false;
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

int isParen(char c)
{
    if (c == '(' || ')' == c) return ROUND_BRACKET;
    if (c == '[' || ']' == c) return SQUARE_BRACKET;
    if (c == '{' || '}' == c) return BRACE;
    return 0;
}

int isParenL(char c)
{
    if (c == '(') return ROUND_BRA;
    if (c == '[') return SQUARE_BRA;
    if (c == '{') return BRACE_BRA;
    return 0;
}

int isParenR(char c)
{
    if (')' == c) return ROUND_KET;
    if (']' == c) return SQUARE_KET;
    if ('}' == c) return BRACE_KET;
    return 0;
}

int isParenR_Ex(char c)
{
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

TokenList lex(ByteString code)
{
    char header[] = "PROGRAM_HEAD";
    TokenList list = makeTokenListNode(makeToken(header, strlen(header), PROGRAM_HEAD), NULL);
    TokenList last = list;

    size_t lim = *(code->size);
    char *data = code->data;
    size_t i = 0;

    char *copyhead;
    size_t j = 0;

    uint32_t tokentype;
    Token token = NULL;
    Token token2 = NULL;

    while (1) {
        //空白を無視
        //printf("%ld\n", i);
        skipSep(data, &i);
        //printf("%ld\n", i);
        if (i >= lim) break;
        copyhead = &data[i];
        //printf("%c\n", copyhead[0]);
        j = 0;
        for (; i < lim; i++) {
            if (isDoubleQuote(data[i])) {
                copyhead = &data[i+1];
                j = 0;
                while (1) {
                    if (&copyhead[j] >= &data[lim]) {
                        perror("EOF before double-quotation end.");
                        exit(1);
                    }
                    if (copyhead[j] != '\"') j++;
                    else break;
                }

                token = makeToken(copyhead, j, LEXBYTESTRING);
 
                last = appendTokenList(last, token);

                token = NULL;
                i += j + 2;
                break;
            }

            if (isSep_Ex(data[i]) || isParen(data[i])) {
                //トークンの区切り
                tokentype = (uint32_t)isParenL(data[i]);
                //if (tokentype == 0) tokentype = LEXATOM;

                if (j == 0 && isParenR_Ex(data[i])) {
                    token = makeToken("", 0, isParenR_Ex(data[i]));
                } else if (isParenR_Ex(data[i])) {
                    token = makeToken(copyhead, j, tokentype);
                    token2 = makeToken("", 0, isParenR_Ex(data[i]));
                } else {
                    token = makeToken(copyhead, j, tokentype);
                }

                last = appendTokenList(last, token);
                if (token2 != NULL) last = appendTokenList(last, token2);
                
                token = NULL;
                token2 = NULL;
                //printf("%c\n", data[i]);
                i++;
                //printf("%c\n", data[i]);
                break;
            }
            //それ以外はトークンの一部とみなす
            j++;
            if (j >= 512) {
                perror("Too long name: the token has too many characters.");
            }
        }
    }

    return list;
}


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
