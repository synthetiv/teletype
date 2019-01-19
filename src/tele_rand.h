#ifndef _TELE_RAND_H_
#define _TELE_RAND_H_

#include "types.h"

typedef struct {
    u32 z;
    u32 w;
    s16 seed;
} tele_rand_t;

extern void tele_srand(tele_rand_t *r, s16 seed);

extern s32 tele_rand(tele_rand_t *r);

#endif /* _TELE_RAND_H_ */
