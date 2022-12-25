#include <iostream>

#include "MemoryAllocator.h"

const int bufferSize = 48 * 4;

int main()
{
    auto alloc = new MemoryAllocator();
    alloc->init();
    int* a = (int*)alloc->alloc(sizeof(int));
    *a = 17;
    int* b = (int*)alloc->alloc(sizeof(int));
    *b = 18;
    int* c = (int*)alloc->alloc(sizeof(int));
    *c = 19;
    int* d = (int*)alloc->alloc(sizeof(int));
    *d = 20;
    int* e = (int*)alloc->alloc(sizeof(int));
    *e = 21;
    int* f = (int*)alloc->alloc(sizeof(int));
    *f = 22;
    int* g = (int*)alloc->alloc(sizeof(int));
    *g = 23;
    int* h = (int*)alloc->alloc(sizeof(int));
    *h = 24;
    int* i = (int*)alloc->alloc(sizeof(int));
    *i = 25;
    int* k = (int*)alloc->alloc(sizeof(int));
    *k = 26;
    int* l = (int*)alloc->alloc(sizeof(int));
    *l = 27;
    int* m = (int*)alloc->alloc(sizeof(int));
    *m = 28;
    int* n = (int*)alloc->alloc(sizeof(int));
    *n = 29;

    alloc->free(a);
    alloc->free(b);
    alloc->free(c);
    alloc->free(d);
    alloc->free(e);
    alloc->free(f);
    alloc->free(g);
    alloc->free(h);
    alloc->free(i);
    alloc->free(k);
    alloc->free(l);
    alloc->free(m);
    alloc->free(n);
#ifdef _DEBUG
    alloc->dumpBlocks();
    alloc->dumpStat();
#endif
    alloc->destroy();
    delete alloc;

}
