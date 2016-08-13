#pragma once


ByteString allocByteString(size_t n);
ByteString byteStringFromFile(const char *filename);
ByteString freeByteString(ByteString str);
void printByteString(ByteString str);
void printlnByteString(ByteString str);
ByteString appendByteString(ByteString str1, const char *str2);
