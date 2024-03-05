# Hamming Codes

## Program Description
This program encodes and decodes files using a Hamming(8, 4) systematic code generator and decoder to correct for erroneous bit flips during storage of data.

## Program Build
build the program by typing `make`

## Run Encode Program
run the encoding program by typing `./encode [-h] [-i infile] [-o outfile]`
1. `-h` Help message. Prints the usage of the program
2. `-i infile` specifies the input file
3. `-o outfile` specifies teh output file

## Run Decode Program
run the encoding program by typing `./decode [-h] [-v] [-i infile] [-o outfile]`
1. `-h` Help message. Prints the usage of the program
2. `-i infile` specifies the input file
3. `-o outfile` specifies the output file

## scan-build
run with scan-build by typing `make scan-build`

### Errors Reported by scan-build (not truly problems)
the errors that have been reported include:

1.  `/usr/bin/ld: bv.o: relocation R_X86_64_32 against '.rodata.str1.1' can not be used when making a PIE object; recompile    with -fPIE `
    `/usr/bin/ld: bm.o: relocation R_X86_64_32 against '.rodata.str1.1' can not be used when making a PIE object; recompile with -fPIE `
    `collect2: error: ld returned 1 exit status`
