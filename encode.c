#include "bm.h"
#include "bv.h"
#include "hamming.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "hi:o:"
#define BLOCK   4096

// Returns the lower nibble of val
uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}

// Returns the upper nibble of val
uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}

// Packs two nibbles into a byte
uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
}

int main(int argc, char **argv) {

    FILE *infile = NULL;
    FILE *outfile = NULL;
    char *infile_name;
    char *outfile_name;
    int opt = 0;

    bool input = false;
    bool output = false;
    bool help = false;
    // get user input
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 'i':
            input = true;
            infile_name = optarg;
            break;
        case 'o':
            output = true;
            outfile_name = optarg;
            break;
        default: break;
        }
    }

    if (help) {
        printf("SYNOPSIS\n");
        printf("  A Hamming(8, 4) systematic code generator.\n\n");
        printf("USAGE");
        printf("  ./encode [-h] [-i infile] [-o outfile]\n\n");
        printf("OPTIONS\n");
        printf("  %-15sProgram usage and help.\n", "-h");
        printf("  %-15sInput data to encode.\n", "-i infile");
        printf("  %-15sOutput of encoded data.\n", "-o outfile");
    }

    // if input file is specified open it, default input file is stdin
    if (input) {
        infile = fopen(infile_name, "rb");
        if (infile == NULL) {
            fprintf(stderr, "failed to open %s\n", infile_name);
            return 1;
        }

    } else {
        infile = stdin;
    }
    // if output file is specified open it, default output file is stdout
    if (output) {
        outfile = fopen(outfile_name, "wb");

        if (outfile == NULL) {
            fprintf(stderr, "failed to open %s\n", outfile_name);
            return 1;
        }

    } else {
        outfile = stdout;
    }
    // code taken from assignment doc Darrell Long
    // getting and setting file permissions
    struct stat statbuf;
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    // Generator matrix G
    BitMatrix *G = bm_create(4, 8);
    for (uint32_t i = 0; i < bm_rows(G); ++i) {
        for (uint32_t j = 0; j < bm_cols(G); ++j) {
            if (i == j) {
                bm_set_bit(G, i, j);
            }
            if (j > 3 && j % 4 != i) {
                bm_set_bit(G, i, j);
            }
        }
    }

    char msg;
    char msg1;
    char msg2;
    uint8_t code1;
    uint8_t code2;
    // encode input file and write to encoded data to output file
    while ((msg = fgetc(infile)) != EOF) {
        msg1 = lower_nibble(msg);
        msg2 = upper_nibble(msg);

        code1 = ham_encode(G, msg1);
        code2 = ham_encode(G, msg2);

        fputc(code1, outfile);
        fputc(code2, outfile);
    }

    bm_delete(&G);
    fclose(infile);
    fclose(outfile);
    return 1;
}
