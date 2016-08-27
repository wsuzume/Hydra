
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
