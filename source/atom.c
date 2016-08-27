
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

