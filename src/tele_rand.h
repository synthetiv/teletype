#ifndef _TELE_RAND_H_
#define _TELE_RAND_H_

#include "types.h"

typedef struct {
    u32 z;
    u32 w;
    s16 seed;
} tele_rand_t;

extern void tele_rand_init(tele_rand_t *r, u32 seed);

extern s16 tele_rand_next(tele_rand_t *r);

#endif /* _TELE_RAND_H_ */
