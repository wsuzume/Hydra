#pragma once

typedef struct list_t* List;
struct list_t {
    void *car;
    List cdr;
};

typedef struct hydranode_t* HydraNode;
struct hydranode_t {
    uint64_t idx;
    uint64_t idy;
    void *p;
    HydraNet x; //defines Parallelism
    HydraNet y; //defines Concurrency
};

typedef struct hydragraph_t* HydraGraph;
struct hydragraph_t {
    HydraNode *node;
    intptr_t px;
    intptr_t py;
    intptr_t mx;
    intptr_t my;
};

typedef struct hydranet_t* HydraNet;
struct hydranet_t {
    HashMap table;
    void *p;
};

typedef struct bridge_t* Bridge;
struct bridge_t {
    uint64_t idx1;
    uint64_t idy1;
    uint64_t idx2;
    uint64_t idy2;
};

typedef struct neuroinfo_t* NeuroInfo;
struct neuroinfo_t {
    void *data;
    List blist;
};
