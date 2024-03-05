#include "bv.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BYTE 8

typedef struct BitVector {
    uint32_t length; // Length in bits
    uint8_t *vector; // Array of bytes each with type uint8_t
} BitVector;
// constructs BitVector
// param: length of BitVector
BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *) malloc(sizeof(BitVector));
    if (v) {
        v->length = length;
        uint32_t bytes
            = (length + BYTE - 1) / BYTE; //ceiling division, number of bytes in vector array
        v->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
        if (!v->vector) {
            free(v);
            v = NULL;
        }
    }
    return v;
}
// memory deallocation
void bv_delete(BitVector **v) {
    if (*v && (*v)->vector) {
        free((*v)->vector);
        free(*v);
        *v = NULL;
    }
    return;
}
// getter function for length
uint32_t bv_length(BitVector *v) {
    return v->length;
}

// param: *v - bitvector, i - index of bit to be set
// return: void
void bv_set_bit(BitVector *v, uint32_t i) {
    uint32_t byte = i / BYTE;
    uint32_t offset = i % BYTE;
    v->vector[byte] |= 1 << offset;
}
// param: *v - bitvector, i - index of bit to be cleared
// return: void
void bv_clr_bit(BitVector *v, uint32_t i) {
    uint32_t byte = i / BYTE;
    uint32_t offset = i % BYTE;
    v->vector[byte] &= ~(1 << offset);
}
// param: *v - bitvector, i - index of bit in *v, bit - xor this bit with bit in index i
// return: void
void bv_xor_bit(BitVector *v, uint32_t i, uint8_t bit) {
    uint32_t byte = i / BYTE;
    uint32_t offset = i % BYTE;
    v->vector[byte] ^= bit << offset;
}
// param: *v - bitvector, i - index of bit to get
// return: bit at index i in *V
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    uint32_t byte = i / BYTE;
    uint32_t offset = i % BYTE;
    return (v->vector[byte] & 1 << offset) >> offset;
}
// param: *v
// return: void
void bv_print(BitVector *v) {
    // print bit vector
    printf("bit vector: [");
    for (uint32_t i = 0; i < v->length; ++i) {
        printf("%d", bv_get_bit(v, i));
        if (i + 1 != v->length) {
            printf(", ");
        } else {
            printf("]\n             ");
        }
    }
    // print indices
    for (uint32_t i = 0; i < v->length; ++i) {
        printf("%d", i);
        if (i + 1 != v->length) {
            printf(", ");
        } else {
            printf("\n");
        }
    }
}
