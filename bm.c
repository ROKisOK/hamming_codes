#include "bv.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define BYTE 8

typedef struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
} BitMatrix;
// constructs BitMatrix
BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) malloc(sizeof(BitMatrix));
    if (m) {
        m->rows = rows;
        m->cols = cols;
        m->vector = bv_create(rows * cols);
        if (!m->vector) {
            free(m);
            m = NULL;
        }
    }
    return m;
}
// memory deallocation
void bm_delete(BitMatrix **m) {
    if ((*m) && (*m)->vector) {
        bv_delete(&(*m)->vector);
        free(*m);
        *m = NULL;
    }
    return;
}
// getter function for rows
uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}
// getter function for cols
uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}
// sets bit at row r, column c in matrix m
void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    bv_set_bit(m->vector, r * m->cols + c);
}
// clears bit at row r, column c in matrix m
void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    bv_clr_bit(m->vector, r * m->cols + c);
}
// gets bit at row r, column c in matrix m
uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    return bv_get_bit(m->vector, r * m->cols + c);
}
// converts the first specified number of bits in the matrix into a vector
BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    BitMatrix *m = bm_create(1, length);
    uint8_t bit;
    for (uint32_t i = 0; i < length; ++i) {
        bit = (byte >> i) & 1;
        // printf("%dth bit = %d\n", i, bit);
        if (bit == 1) {
            bm_set_bit(m, 0, i);
        }
    }
    return m;
}
// converts the the given Bitmatrix into a byte
uint8_t bm_to_data(BitMatrix *m) {
    uint8_t byte = 0;
    for (uint32_t i = 0; i < BYTE; ++i) {
        byte |= bm_get_bit(m, 0, i) << i;
    }
    return byte;
}
// performs matrix multiplication and mod 2s every element in the result
BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    BitMatrix *C = bm_create(bm_rows(A), bm_cols(B));
    uint32_t sum[bm_rows(A)][bm_cols(B)];
    // initialize sum array
    for (uint32_t i = 0; i < bm_rows(A); ++i) {
        for (uint32_t j = 0; j < bm_cols(B); ++j) {
            sum[i][j] = 0;
        }
    }
    // multiply matrices
    for (uint32_t i = 0; i < bm_rows(A); ++i) {
        for (uint32_t j = 0; j < bm_cols(B); ++j) {
            for (uint32_t k = 0; k < bm_cols(A); ++k) {
                sum[i][j] ^= bm_get_bit(A, i, k) & bm_get_bit(B, k, j);
            }
        }
    }
    // set bits
    for (uint32_t i = 0; i < bm_rows(A); ++i) {
        for (uint32_t j = 0; j < bm_cols(B); ++j) {
            if (sum[i][j] == 1) {
                bm_set_bit(C, i, j);
            }
        }
    }
    return C;
}

void bm_print(BitMatrix *m) {
    // print matrix
    printf("bit matrix:\n");
    for (uint32_t i = 0; i < m->rows; ++i) {
        printf("%-2d[", i);
        for (uint32_t j = 0; j < m->cols; ++j) {
            printf("%2d", bm_get_bit(m, i, j));
            if (j + 1 != m->cols) {
                printf("|");
            } else {
                printf("]\n");
            }
        }
    }
    // print indices
    printf("    ");
    for (uint32_t i = 0; i < m->cols; ++i) {
        printf("%-3d", i);
        if (i + 1 == m->cols) {
            printf("\n");
        }
    }
}
