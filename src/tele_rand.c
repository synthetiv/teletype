#include "tele_rand.h"

void tele_rand_init(tele_rand_t *r, u32 seed) {
    r->z = 1823672;
    r->w = seed;
    r->seed = seed;
}

s16 tele_rand_next(tele_rand_t *r) {
    r->z = 36969 * (r->z & 65535) + (r->z >> 16);
    r->w = 18000 * (r->w & 65535) + (r->w >> 16);
    return (r->z << 16) + r->w;
}
