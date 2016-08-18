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



