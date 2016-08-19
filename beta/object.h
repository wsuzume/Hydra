#pragma once


ByteString allocByteString(size_t n);
ByteString byteStringFromFile(const char *filename);
ByteString freeByteString(ByteString str);
void printByteString(ByteString str);
void printlnByteString(ByteString str);
ByteString appendByteString(ByteString str1, const char *str2);

List makeList(Type type);
Pair makePair(uintptr_t x, uintptr_t y);
List cons(void *x, List xs);
void *head(List xs);
void freeList(List xs);
void printList(List xs);

List makeByteStringList(int n, char *str[]);
void freeByteStringList(List xs);


static inline hid_t *makeLink(hid_t *w, hid_t x, hid_t y) { w[0] = x; w[1] = y; return w; }
static inline hid_t upsLink(hid_t *w) { return w[0]; }
static inline hid_t dwsLink(hid_t *w) { return w[1]; }
//大きいほうを返す。-1なら左のv, 1なら右のwが大きい。
static inline int compLink(hid_t *v, hid_t *w)
{
    return (v[0] < w[0] ?  1 :
           (v[0] > w[0] ? -1 :
           (v[1] < w[1] ?  1 :
           (v[1] > w[1] ? -1 : 0))));
}
static inline hid_t makeHID(int32_t x, int32_t y) { return (((int64_t)x) << 32) | (int64_t)(y & 0xffffffff); }
static inline int compHID(hid_t v, hid_t w)
{
    return (v < w ?  1 :
           (v > w ? -1 : 0));
}
static inline hhid_t getxHID(hid_t hid) { return (hhid_t)((hid >> 32) & 0xffffffff); }
static inline hhid_t getyHID(hid_t hid) { return (hhid_t)(hid & 0xffffffff); }
void printHID(hid_t hid);
hid_t view(hid_t v, hid_t hid);

