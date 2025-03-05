#pragma once

#include <stdint.h>
#include <stddef.h>

struct hll {
    uint32_t* buckets;
    uint32_t sz;
    uint32_t bits;
    uint32_t mask;
};
typedef struct hll hll;

hll hll_with_sz(uint32_t);
void add(hll*, uint32_t);
double count(const hll*);
