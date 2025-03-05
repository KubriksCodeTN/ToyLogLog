#include "hll.h"
#include <stdio.h>

int main(void) {
    // to estimate upper bound use log2(1.04/error_rate ** 2)
    hll ll = hll_with_sz(32); // error of around 10% (bigger -> more accurate)

    for (uint32_t i = 0; i < 10000; ++i) {
        add(&ll, i);
    }

    printf("%lf\n", count(&ll));
}
