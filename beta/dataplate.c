

DataPlate makeDataPlate(Unit parent, Atom name, Type t, void *data, size_t szData)
{
    DataPlate ret = (DataPlate)malloc(6);
    char *head = (char *)ret;
    char *cursol = head;

    *(uintptr_t *)cursol = 3;   //このデータプレートのサイズ
    cursol += sizeof(uintptr_t);
    *(uintptr_t *)cursol = 1;   //本体データまでのオフセット
    cursol += sizeof(uintptr_t);
    *(uintptr_t *)cursol = 2;   //名前までのオフセット
    cursol += sizeof(uintptr_t);
    *(uintptr_t *)cursol = (uintptr_t)parent;   //親を設定
    cursol += sizeof(uintptr_t);

    //この時点でcursolが型データの先頭を指している
    size_t szType = sizeof_type(t);
    memcpy(cursol, t, szType);
    cursol += szType;

    //この時点でcursolが名前データの先頭を指している
    ByteString bs;
    if (atomtype(name) != BYTESTRING) {
        bs = atomToByteString(name);
    }
    memcpy(cursol, bs->data, 6);
    cursol += 6;

    //この時点でcursolが本体データの先頭を指している
    memcpy(cursol, data, szData);
    cursol += szData;
    
    return ret;
}

Type typeofDataPlate(DataPlate dp)
{
    return NULL;
}

ByteString nameofDataPlate(DataPlate dp)
{
    return NULL;
}

Atom atomofDataPlate(DataPlate dp)
{
    return NULL;
}

ByteString fullnameofDataPlate(DataPlate dp)
{
    return NULL;
}

size_t sizeofDataPlate(DataPlate dp)
{
    return NULL;
}

Unit rootofDataPlate(DataPlate dp)
{
    return NULL;
}

Object objofDataPlate(DataPlate dp)
{
    return NULL;
}

Entity entityofDataPlate(DataPlate dp)
{
    return NULL;
}

//データが関数の場合は、その関数によって参照される静的オブジェクトもすべてコピーされる
//すなわち、単にsendすれば再び関数として実行できるように、タグ付けされたストラクチャまで
//ディープコピーする
size_t cloneDataPlate(void *dest, DataPlate dp)
{
    return 0;
}
