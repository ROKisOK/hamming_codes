#include "bm.h"
#include "bv.h"
#include "hamming.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "hi:o:v"

// helper functions from Darrell Long's assignment doc
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
    bool verbose = false;
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
        case 'v': verbose = true;
        default: break;
        }
    }

    // transpose of parity checker matrix
    BitMatrix *H_T = bm_create(8, 4);
    for (uint32_t i = 0; i < bm_rows(H_T); ++i) {
        for (uint32_t j = 0; j < bm_cols(H_T); ++j) {
            bm_set_bit(H_T, i, j);
            if (i == j) {
                bm_clr_bit(H_T, i, j);
            }
            if (i > 3 && i % 4 != j) {
                bm_clr_bit(H_T, i, j);
            }
        }
    }
    // bm_print(H_T);

    if (help) {
        printf("SYNOPSIS\n");
        printf("  A Hamming(8, 4) systematic code decoder.\n\n");
        printf("USAGE");
        printf("  ./decode [-h] [-v] [-i infile] [-o outfile]\n\n");
        printf("OPTIONS\n");
        printf("  %-15sProgram usage and help.\n", "-h");
        printf("  %-15sPrint statistics of decoding to stderr.\n", "-v");
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

    // code taken from Darrell Long's assignment doc
    // getting and setting file permissions
    struct stat statbuf;
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    uint32_t total_bytes = 0;
    uint32_t uncorrected = 0;
    uint32_t corrected = 0;

    uint32_t count = 0;
    int byte = 0;
    uint8_t lower;
    uint8_t upper;
    HAM_STATUS status;
    // get encoded data from input file and decode to output file
    while ((byte = fgetc(infile)) != EOF) {
        count += 1;
        if (count % 2 == 1) {
            status = ham_decode(H_T, (uint8_t) byte, &lower);
        } else {
            status = ham_decode(H_T, (uint8_t) byte, &upper);
        }

        // calculate statistics if user specified verbose option
        if (verbose) {
            if (status == HAM_OK) {
                total_bytes += 1;
            } else if (status == HAM_ERR) {
                total_bytes += 1;
                uncorrected += 1;
            } else if (status == HAM_CORRECT) {
                total_bytes += 1;
                corrected += 1;
            }
        }
        // if upper and lower bytes were recieved pack them and write to outfile
        if (count % 2 == 0) {
            fputc(pack_byte(upper, lower), outfile);
        }
    }

    // print statistics if verbose was specified
    if (verbose) {
        fprintf(stderr, "Total bytes processed: %d\n", total_bytes);
        fprintf(stderr, "Uncorrected errors: %d\n", uncorrected);
        fprintf(stderr, "Corrected errors: %d\n", corrected);
        fprintf(stderr, "error rate = %f\n", ((float) uncorrected) / total_bytes);
    }
    // free memory
    bm_delete(&H_T);
    fclose(infile);
    fclose(outfile);

    return 1;
}
