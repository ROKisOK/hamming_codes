#include "bm.h"

#include <stdint.h>
#include <stdio.h>

#define BYTE   8
#define NIBBLE 4

typedef enum HAM_STATUS {

    HAM_OK = -3, // No errors detected.
    HAM_ERR = -2, // Uncorrectable.
    HAM_CORRECT = -1, // Detected error and corrected.
} HAM_STATUS;

// encoding algorithm
// param: G - generator matrix, msg - nibble of data to be encoded
// returns encoded code
uint8_t ham_encode(BitMatrix *G, uint8_t msg) {
    BitMatrix *m = bm_from_data(msg, NIBBLE);
    BitMatrix *C = bm_multiply(m, G);
    // bm_print(C);
    uint8_t code = bm_to_data(C);
    bm_delete(&m);
    bm_delete(&C);
    return code;
}

// decoding algorithm
// param: Ht - parity transpose matrix, code - 8 bit code, *msg - decoded message that is passed back
// returns: whether an error was detected and if it was corrected
HAM_STATUS ham_decode(BitMatrix *Ht, uint8_t code, uint8_t *msg) {
    static const int8_t table[16] = { HAM_OK, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR,
        HAM_ERR, 2, HAM_ERR, 1, 0, HAM_ERR };
    BitMatrix *C = bm_from_data(code, BYTE);
    BitMatrix *e = bm_multiply(C, Ht);
    int8_t status = table[bm_to_data(e)];
    bm_delete(&C);
    bm_delete(&e);
    if (status == HAM_OK) {
        *msg = code;
        return HAM_OK;
    } else if (status == HAM_ERR) {
        *msg = code;
        return HAM_ERR;
    } else {
        code ^= 1 << status;
        *msg = code;
    }
    return HAM_CORRECT;
}
