#include "hydra.h"


static inline hid_t *makeLink(hid_t *w, hid_t x, hid_t y)
{
    w[0] = x;
    w[1] = y;
    return w;
}

static inline hid_t upsLink(hid_t *w)
{
    return w[0];
}

static inline hid_t dwsLink(hid_t *w)
{
    return w[1];
}

//大きいほうを返す。-1なら左のv, 1なら右のwが大きい。
static inline int compLink(hid_t *v, hid_t *w)
{
    return (v[0] < w[0] ?  1 :
           (v[0] > w[0] ? -1 :
           (v[1] < w[1] ?  1 :
           (v[1] > w[1] ? -1 : 0))));
}

static inline hid_t makeHID(int32_t x, int32_t y)
{
    return (((int64_t)x) << 32) | (int64_t)(y & 0xffffffff);
}

static inline int compHID(hid_t v, hid_t w)
{
    return (v < w ?  1 :
           (v > w ? -1 : 0));
}

static inline hhid_t getxHID(hid_t hid)
{
    return (hhid_t)((hid >> 32) & 0xffffffff); 
}

static inline hhid_t getyHID(hid_t hid)
{
    return (hhid_t)(hid & 0xffffffff);
}

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

/* test code 1 */
/*
int main(void)
{
    printHID(makeHID(3, 5));
    putchar('\n');
    printHID(makeHID(-2, 1));
    putchar('\n');
    printHID(makeHID(4, -6));
    putchar('\n');
    printHID(makeHID(-2, -8));
    putchar('\n');
    printHID(makeHID(0, 0));
    putchar('\n');
    printHID(makeHID(5, 0));
    putchar('\n');
    printHID(makeHID(-6, 0));
    putchar('\n');
    printHID(makeHID(0, 8));
    putchar('\n');
    printHID(makeHID(0, -2));
    putchar('\n');
    printHID(makeHID(INF, 0));
    putchar('\n');
    printHID(makeHID(INF, 2));
    putchar('\n');
    printHID(makeHID(INF, -3));
    putchar('\n');
    printHID(makeHID(0, INF));
    putchar('\n');
    printHID(makeHID(4, INF));
    putchar('\n');
    printHID(makeHID(-5, INF));
    putchar('\n');
    printHID(makeHID(INF, INF));
    putchar('\n');

    printHID(view(makeHID(15, 31), makeHID(INF, INF)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(INF, 0)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(0, INF)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(INF, 1)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(INF, -1)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(1, INF)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(-1, INF)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(INF, 2)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(INF, -3)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(4, INF)));
    putchar('\n');
    printHID(view(makeHID(15, 31), makeHID(-5, INF)));
    putchar('\n');

    dhid_t v, w;
    makeLink(v, makeHID(6,5), makeHID(4, 8));
    makeLink(w, makeHID(3,5), makeHID(4, 8));
    printHID(upsLink(w));
    printHID(dwsLink(w));
    putchar('\n');
    printf("%d\n", makeHID(3,5) < makeHID(4,8) ? 1 : 0);
    printf("%d\n", compLink(v, w));
}
*/

ByteString makeByteString(char *str)
{
    ByteString ret = (ByteString)malloc(sizeof(struct bytestring_t));
    ret->size = strlen(str) + 1;
    ret->data = str;
    return ret;
}

void freeByteString(ByteString str)
{
    free(str);
    return;
}

void printByteString(ByteString str)
{
    fputs(str->data, stdout);
    return;
}

int compByteString(ByteString a1, ByteString a2)
{
    char *s1 = a1->data;
    char *s2 = a2->data;

    while ((*s1 == *s2) && *s1) {
        *s1++;
        *s2++;
    }

    return (*s1 == *s2) ? 0 :
           (*s1 < *s2)  ? 1 : -1;
}



Atom makeAtomHID(hid_t hid)
{
    Atom ret = (Atom)malloc(sizeof(struct atom_t));
    ret->type = HID;
    ret->data.hid = hid;
    return ret;
}

Atom makeAtomByteString(char *p)
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



Object makeObject(type_t type, void *data)
{
    Object ret = (Object)malloc(sizeof(struct object_t));
    ret->type = type;
    ret->data = data;
    return ret;
}

Object freeObject(Object obj)
{
    if (obj->type == BYTESTRING) {
        freeByteString((ByteString)obj->data);
    }
    free(obj);
    return;
}

void printObject(Object obj)
{
    if (obj->type == BYTESTRING) {
        fputs("(ByteString:", stdout);
        printByteString((ByteString)obj->data);
        putchar(')');
    }
    return;
}




List makeList(type_t type)
{
    List ret = (List)malloc(sizeof(struct pair_t));
    ret->car = (uintptr_t)type;
    ret->cdr = nil;
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

TriUnit makeTriUnit(Atom atom, Object obj, Set set)
{
    TriUnit ret = (TriUnit)malloc(sizeof(struct triunit_t));
    ret->atom = atom;
    ret->obj = obj;
    ret->set = set;
    return ret;
}

static inline Atom atomof(TriUnit t)
{
    return t->atom;
}

static inline Object bark(TriUnit t)
{
    return t->obj;
}

static inline Set leaf(TriUnit t)
{
    return t->set;
}

TriUnit addTriUnit(TriUnit t, TriUnit u)
{
    //未実装
    return t;
}

Set makeSet(type_t type)
{
    Set ret = (Set)calloc(1, sizeof(struct set_t));
    ret->type = type;
    return ret;
}


static inline int compTriUnit(TriUnit t, TriUnit u);
void printTriUnit(TriUnit t);
Set setinsert(Set s, TriUnit t)
{
    Atom atom = t->atom;
    List list = s->root[atom->type];
    
    if (list == NULL) list = s->root[atom->type] = makeList(atom->type);

    List pre = list;
    List cur = (List)list->cdr;
    int comp;
    while (cur != NULL) {
        comp = compTriUnit((TriUnit)cur->car, t);
        if (comp == -1) {
            cons(t, pre);
            return s;
        } else if (comp == 0) {
            perror("Error at addVertex(): you can't add a triunit with the same HID.");
            return s;
        }

        pre = cur;
        cur = (List)cur->cdr;
    }

    pre->cdr = (uintptr_t)makePair((uintptr_t)t, nil);

    return s;
}


void printSet(Set set)
{
    if (set->type == TRIUNIT) {
        putchar('{');
        int i = 0;
        List cur;
        int printflag = 0;
        for (; i < ATOMTYPENUM; i++) {
            cur = (List)set->root[i];
            if (cur == NULL) continue;
            cur = (List)cur->cdr;
            if (cur != NULL) {
                if (i != 0 && printflag) putchar(',');
                printAtom(((TriUnit)cur->car)->atom);
                cur = (List)cur->cdr;
                printflag = 1;
            }
            while (cur != NULL) {
                putchar(',');
                printAtom(((TriUnit)cur->car)->atom);
                cur = (List)cur->cdr;
            }
        }
        putchar('}');
    } else {
        fputs("{SET}", stdout);
    }

    return;
}

void freeSet(Set set)
{
    free(set);
    return;
}

TriUnit setsearch(Set s, Atom atom)
{
    List cur = s->root[atom->type];
    if (cur->cdr == nil) return NULL;

    cur = (List)cur->cdr;
    while (cur != NULL) {
        if (compAtom(atom, ((TriUnit)cur->car)->atom) == 0) {
            return (TriUnit)cur->car;
        }
        cur = (List)cur->cdr;
    }

    return NULL;
}


TriUnit freeTriUnit(TriUnit tu)
{
    freeAtom(tu->atom);
    freeObject(tu->obj);
    freeSet(tu->set);
    free(tu);
    return;
}


static inline int compTriUnit(TriUnit t, TriUnit u)
{
    return compAtom(t->atom, u->atom);
}

void printTriUnit(TriUnit t)
{
    putchar('$');
    putchar('{');
    printAtom(t->atom);
    putchar(',');
    printObject(t->obj);
    putchar(',');
    printSet(t->set);
    putchar('}');
    return;
}

HydraGraph makeHydraGraph(Atom atom)
{
    HydraGraph hg = (HydraGraph)malloc(sizeof(struct hydragraph_t));
    hg->atom = atom;
    hg->vertex = makeList(TRIUNIT);
    return hg;
}

HydraGraph addVertex(HydraGraph hg, TriUnit t)
{
    List list = hg->vertex;
    if (list->cdr == nil) {
        cons(t, list);
        return hg;
    }

    List pre = list;
    List cur = (List)list->cdr;
    int comp;
    while (cur != NULL) {
        comp = compTriUnit((TriUnit)cur->car, t);
        if (comp == -1) {
            cons(t, pre);
            return hg;
        } else if (comp == 0) {
            perror("Error at addVertex(): you can't add a vertex with the same HID.");
            return hg;
        }

        pre = cur;
        cur = (List)cur->cdr;
    }

    pre->cdr = (uintptr_t)makePair((uintptr_t)t, nil);

    return hg;
}

void freeHydraGraph(HydraGraph hg)
{
    freeAtom(hg->atom);
    return;
}

void printHydraGraph(HydraGraph hg)
{
    printAtom(hg->atom);
    putchar('\n');
    List list = hg->vertex;
    List buf = (List)list->cdr;
    while (buf != NULL) {
        printTriUnit((TriUnit)buf->car);
        putchar('\n');
        buf = (List)buf->cdr;
    }
    return;
}


TriUnit searchHydraGraph(HydraGraph hg, Atom atom)
{
    List cur = hg->vertex;
    if (cur->cdr == nil) return NULL;

    cur = (List)cur->cdr;
    while (cur != NULL) {
        if (compAtom(atom, ((TriUnit)cur->car)->atom) == 0) {
            return (TriUnit)cur->car;
        }
        cur = (List)cur->cdr;
    }

    return NULL;
}

/* test code 2 */
/*
int main(void)
{
    Atom atom = makeAtomHID(makeHID(INF,INF));
    printAtom(atom);
    putchar('\n');
    
    Atom atom1 = makeAtomHID(makeHID(4,2));
    Atom atom2 = makeAtomHID(makeHID(1,3));
    
    printf("%d\n", compAtom(atom1, atom2));
    printAtom(atom1);
    printAtom(atom2);
    putchar('\n');

    List list = makeList(ATOM);
    cons(atom1, cons(atom2, list));
    printAtom((Atom)head(list));
    printAtom((Atom)head(list));
    putchar('\n');
    freeList(list);

    TriUnit t = makeTriUnit(makeAtomHID(makeHID(98,43)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    printTriUnit(t);
    putchar('\n');
    freeTriUnit(t);

    HydraGraph hg = makeHydraGraph(makeAtomHID(makeHID(INF, INF)));
    TriUnit t1 = makeTriUnit(makeAtomHID(makeHID(48,65)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit t2 = makeTriUnit(makeAtomHID(makeHID(INF,INF)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit t3 = makeTriUnit(makeAtomHID(makeHID(48,65)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit t4 = makeTriUnit(makeAtomHID(makeHID(-32,3)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit t5 = makeTriUnit(makeAtomHID(makeHID(23,0)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit t6 = makeTriUnit(makeAtomHID(makeHID(1,8)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit t7 = makeTriUnit(makeAtomHID(makeHID(-1,4)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit t8 = makeTriUnit(makeAtomHID(makeHID(23,8)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    addVertex(hg, t1);
    addVertex(hg, t2);
    addVertex(hg, t3);
    addVertex(hg, t4);
    addVertex(hg, t5);
    addVertex(hg, t6);
    addVertex(hg, t7);
    addVertex(hg, t8);

    printHydraGraph(hg);

    Atom key1 = makeAtomHID(makeHID(41, 90));
    Atom key2 = makeAtomHID(makeHID(1, 8));

    TriUnit tribuf = searchHydraGraph(hg, key1);
    if (tribuf == NULL) {
        printf("No such node ");
        printAtom(key1);
        putchar('\n');
    } else {
        printf("Node detected ");
        printTriUnit(tribuf);
        putchar('\n');
    }

    tribuf = searchHydraGraph(hg, key2);
    if (tribuf == NULL) {
        printf("No such node ");
        printAtom(key2);
        putchar('\n');
    } else {
        printf("Node detected ");
        printTriUnit(tribuf);
        putchar('\n');
    }

    freeAtom(key2);
    freeAtom(key1);

    freeTriUnit(t8);
    freeTriUnit(t7);
    freeTriUnit(t6);
    freeTriUnit(t5);
    freeTriUnit(t4);
    freeTriUnit(t3);
    freeTriUnit(t2);
    freeTriUnit(t1);
    freeHydraGraph(hg);


    Set s = makeSet(TRIUNIT);

    TriUnit u1 = makeTriUnit(makeAtomByteString("hoge"), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit u2 = makeTriUnit(makeAtomByteString("fuga"), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit u3 = makeTriUnit(makeAtomByteString("piyo"), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit u4 = makeTriUnit(makeAtomHID(makeHID(INF,INF)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit u5 = makeTriUnit(makeAtomHID(makeHID(48,65)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));
    TriUnit u6 = makeTriUnit(makeAtomHID(makeHID(-32,3)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(ATOM));

    Atom name1 = makeAtomHID(makeHID(48,65));
    Atom name2 = makeAtomHID(makeHID(3,7));
    Atom name3 = makeAtomByteString("hoge");
    Atom name4 = makeAtomByteString("poyo");

    setinsert(s, u1);
    setinsert(s, u2);
    setinsert(s, u3);
    setinsert(s, u4);
    setinsert(s, u5);
    setinsert(s, u6);

    printSet(s);
    putchar('\n');
    
    tribuf = setsearch(s, name1);
    if (tribuf == NULL) {
        printf("No such node ");
        printAtom(name1);
        putchar('\n');
    } else {
        printf("Node detected ");
        printTriUnit(tribuf);
        putchar('\n');
    }

    tribuf = setsearch(s, name2);
    if (tribuf == NULL) {
        printf("No such node ");
        printAtom(name2);
        putchar('\n');
    } else {
        printf("Node detected ");
        printTriUnit(tribuf);
        putchar('\n');
    }
    
    tribuf = setsearch(s, name3);
    if (tribuf == NULL) {
        printf("No such node ");
        printAtom(name3);
        putchar('\n');
    } else {
        printf("Node detected ");
        printTriUnit(tribuf);
        putchar('\n');
    }

    tribuf = setsearch(s, name4);
    if (tribuf == NULL) {
        printf("No such node ");
        printAtom(name4);
        putchar('\n');
    } else {
        printf("Node detected ");
        printTriUnit(tribuf);
        putchar('\n');
    }

    freeAtom(name4);
    freeAtom(name3);
    freeAtom(name2);
    freeAtom(name1);

    freeTriUnit(u6);
    freeTriUnit(u5);
    freeTriUnit(u4);
    freeTriUnit(u3);
    freeTriUnit(u2);
    freeTriUnit(u1);
    
    freeSet(s);

    freeAtom(atom);
    freeAtom(atom1);
    freeAtom(atom2);
    return 0;
}
*/

/* test code 3 */
int main(void)
{
    HydraGraph hg = makeHydraGraph(makeAtomHID(makeHID(INF, INF)));
    TriUnit t1 = makeTriUnit(makeAtomHID(makeHID(48,65)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    TriUnit t2 = makeTriUnit(makeAtomHID(makeHID(INF,INF)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    TriUnit t3 = makeTriUnit(makeAtomHID(makeHID(79,-54)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    TriUnit t4 = makeTriUnit(makeAtomHID(makeHID(-32,3)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    TriUnit t5 = makeTriUnit(makeAtomHID(makeHID(23,0)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    TriUnit t6 = makeTriUnit(makeAtomHID(makeHID(1,8)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    TriUnit t7 = makeTriUnit(makeAtomHID(makeHID(-1,4)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    TriUnit t8 = makeTriUnit(makeAtomHID(makeHID(23,8)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    addVertex(hg, t1);
    addVertex(hg, t2);
    addVertex(hg, t3);
    addVertex(hg, t4);
    addVertex(hg, t5);
    addVertex(hg, t6);
    addVertex(hg, t7);
    addVertex(hg, t8);

    printHydraGraph(hg);

    putchar('\n');

    TriUnit u1 = makeTriUnit(makeAtomHID(makeHID(22,33)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    TriUnit u2 = makeTriUnit(makeAtomHID(makeHID(INF,INF)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    TriUnit u3 = makeTriUnit(makeAtomHID(makeHID(44,55)), makeObject(BYTESTRING, makeByteString("Hello, world!")), makeSet(TRIUNIT));
    setinsert(leaf(t1), u1);
    setinsert(leaf(t1), u2);
    setinsert(leaf(t1), u3);

    printTriUnit(t1);
    putchar('\n');
    
    putchar('\n');
    printHydraGraph(hg);
    freeTriUnit(u3);
    freeTriUnit(u2);
    freeTriUnit(u1);

    freeTriUnit(t8);
    freeTriUnit(t7);
    freeTriUnit(t6);
    freeTriUnit(t5);
    freeTriUnit(t4);
    freeTriUnit(t3);
    freeTriUnit(t2);
    freeTriUnit(t1);
    freeHydraGraph(hg);




    return EXIT_SUCCESS;
}
