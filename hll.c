#include "hll.h"
#include "murmur3.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

// only for p in [4, 16]
double alpha(uint32_t p) {
    if (p == 4) 
        return 0.673;
    
    if (p == 5)
        return 0.697;

    if (p == 6)
        return 0.709;

    return 0.7213 / (1.0 + 1.079 / (1 << p));
}

static inline uint32_t blog2(uint32_t x) {
    return 8 * sizeof(uint32_t) - __builtin_clz(x) - 1;
}

static inline uint32_t h(uint32_t x) {
    x = ((x >> 16) ^ x) * 0x119de1f3U;
    x = ((x >> 16) ^ x) * 0x119de1f3U;
    x = (x >> 16) ^ x;
    return x;
}

double hmean(uint32_t* v, uint32_t sz) {
    double sum = .0;
    for (uint32_t i = 0; i < sz; ++i)
        sum += 1. / (1 << v[i]);
    return (sz * sz) / sum;
}

hll hll_with_sz(uint32_t sz) {
    hll new_hll;
    new_hll.buckets = malloc(sz * sizeof(uint32_t));
    memset(new_hll.buckets, 0, sz * sizeof(uint32_t));
    new_hll.sz = sz;
    new_hll.bits = blog2(sz);

    return new_hll;
}

void add(hll* ll, uint32_t x) {
    uint32_t hash;
    MurmurHash3_x86_32(&x, sizeof(uint32_t), 4, &hash);
    uint32_t i = hash >> (8 * sizeof(uint32_t) - ll->bits);
    hash &= ~(i << (8 * sizeof(uint32_t) - ll->bits));
    uint32_t lz = hash ? __builtin_clz(hash) - ll->bits : 8 * sizeof(uint32_t) - ll->bits - 1;
    ll->buckets[i] = MAX(ll->buckets[i], lz + 1);
}

double count(const hll* ll) {
    double mean = hmean(ll->buckets, ll->sz);
    double E = alpha(ll->bits) * mean;
    /* if (E < 2.5 * ll->sz) TODO */
    return E;
}
