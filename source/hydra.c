#include "hydra.h"



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

