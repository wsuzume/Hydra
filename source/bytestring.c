
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


