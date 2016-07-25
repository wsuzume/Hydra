#include <stdio.h>
#include <stdint.h>
//#include "hydra.h"


enum HTYPE {
    NIL,
    HID,
};

typedef int64_t hid_t;
typedef int32_t hhid_t;

#define INF INT32_MIN

static inline hid_t makeHID(int32_t x, int32_t y)
{
    return (((int64_t)x) << 32) | (int64_t)(y & 0xffffffff);
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

int main(void)
{
    /*
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
    */

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
}
