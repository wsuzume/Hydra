#include "hydra.h"

enum BRIDGETYPE {
    BR_NONE = 0,
    BR_WRITE = 1,
    BR_READ = 2,
    BR_RDWR = 3,
};

typedef void* SysMem;

#define INNER_PARALLEL      0x00
#define INNER_CONCURRENT    0x01
#define OUTER_PARALLEL      0x02
#define OUTER_CONCURRENT    0x03

typedef struct bridge_t* Bridge;
struct bridge_t {
    ByteString name;
    uint32_t status;
    void *shm;
    long size;

    union {
        SysMem mem;
        FILE *file;
        int stream;
    } via;

};


int initshm(const char *name, long size)
{
    int fd = open(name, O_RDWR | O_BINARY | O_CREAT, S_IREAD | S_IWRITE);
    if (fd == -1) {
        perror("could not open file.");
        return -1;;
    }
    long fsize = lseek(fd, 0, 2);
    lseek(fd, 0, 0);
    void *buf = calloc(size, 1);
    if (buf == NULL) {
        perror("memory exhaustion.");
        return -1;
    }
    write(fd, buf, size);
    free(buf);
    lseek(fd, 0, 0);
    return fd;
}

void *bindshm(int fd, size_t size)
{
    void *shm = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm == MAP_FAILED) {
        perror("map failed.");
        return NULL;
    }
    return shm;
}

void alloc_bridge(const char *name, uint32_t status, long size, void *p)
{
    Bridge b = (Bridge)malloc(sizeof(struct bridge_t));
    b->status = status;
    switch (b->status & 0x03) {
        case INNER_PARALLEL:

            break;
        case INNER_CONCURRENT:
            b->size = size;
            b->via.file = initshm(name, size);
            b->shm = bindshm(b->via.file, size);
            break;
        case OUTER_PARALLEL:
        case OUTER_CONCURRENT:
            b->via.stream = socket(PF_INET, SOCK_STREAM, 0);
            //connect();
            break;
        default:
            break;
    }

}

void write_bridge(Bridge b, void *src, size_t size, size_t num)
{
    switch (b & 0xff) {
        case INNER_PARALLEL:
            //memcpy(buf1, buf2, size);
            break;
        case INNER_CONCURRENT:
            //fwrite(buf, size, num, fp);
            break;
        case OUTER_PARALLEL:
            //write(socket, buf, n);
            break;
        case OUTER_CONCURRENT:
            //write(socket, buf, n);
            break;
        default:
            break;
    }
}

void read_bridge(Bridge b)
{
    switch (b & 0xff) {
        case INNER_PARALLEL:
            break;
        case INNER_CONCURRENT:
            break;
        case OUTER_PARALLEL:
            break;
        case OUTER_CONCURRENT:
            break;
        default:
            break;
    }

}

void clear_bridge(Bridge b)
{
    switch (b & 0xff) {
        case INNER_PARALLEL:
            break;
        case INNER_CONCURRENT:
            break;
        case OUTER_PARALLEL:
            break;
        case OUTER_CONCURRENT:
            break;
        default:
            break;
    }

}

void free_bridge(Bridge b)
{
    switch (b & 0xff) {
        case INNER_PARALLEL:
            break;
        case INNER_CONCURRENT:
            break;
        case OUTER_PARALLEL:
            break;
        case OUTER_CONCURRENT:
            break;
        default:
            break;
    }

}
