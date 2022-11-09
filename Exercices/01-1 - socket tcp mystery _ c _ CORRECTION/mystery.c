#include "mystery.h"

#include <stdint.h>
#include <stdlib.h>

#define RAND_R_STEPS 23

uint32_t key_generator(uint32_t id)
{
    unsigned int seed = (unsigned int) id;
    int key;
    int i;

    for (i = 0; i < RAND_R_STEPS; i++) {
        key = rand_r(&seed);
    }

    return (uint32_t) key;
}
