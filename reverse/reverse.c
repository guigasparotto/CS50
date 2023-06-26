#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "wav.h"

bool checkFormat(WAVHEADER header);
int getBlockSize(WAVHEADER header);

// This is part of the Reverse challenge from the online course CS50 from Harvard
// Details can be found at https://cs50.harvard.edu/x/2023/psets/4/reverse/
// The program below takes 2 arguments, the first is an input file in WAV format
// and the second is the output file - it reads the input and reverses it, saving
// the new file in the output file

int main(int argc, char *argv[])
{
    if(argc != 3) {
        fprintf(stderr, "Usage: %s arg1 arg2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *infile = argv[1];
    char *outfile = argv[2];

    // Open input file for reading
    FILE *inputFile = fopen(infile, "r");
    if (inputFile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return EXIT_FAILURE;
    }

    // Read header and validate its format
    WAVHEADER wavheader;
    fread(&wavheader, sizeof(WAVHEADER), 1, inputFile);
    if (!checkFormat(wavheader)) {
        fclose(inputFile);
        fprintf(stderr, "File is not in the correct format: %s.\n", infile);
        return EXIT_FAILURE;
    }

    // Open output file for writing
    FILE *outputFile = fopen(outfile, "w");
    if (outputFile == NULL)
    {
        fclose(inputFile);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return EXIT_FAILURE;
    }

    // Write header to file
    long headerResult = (long) fwrite(&wavheader, sizeof(WAVHEADER), 1, outputFile);
    if (headerResult != 1) {
        fclose(inputFile);
        fprintf(stderr, "Failed to write WAV header to file.\n");
        return EXIT_FAILURE;
    }

    // Write reversed audio to files

    // Move the pointer to the end of the file and use ftell to get the
    // last position, which will be equal to the size of the file
    fseek(inputFile, 0, SEEK_END);
    long sizeOfFile = ftell(inputFile);
    if (sizeOfFile == -1L) {
        fclose(inputFile);
        fclose(outputFile);
        fprintf(stderr, "Failed to determine size of file.\n");
        return EXIT_FAILURE;
    }

    // calculate the number of blocks of audio in the file - we can think of a block
    // as a unit of auditory data. Then allocate the required memory for it
    int blockSize = getBlockSize(wavheader);
    long blockCount = (long) ((sizeOfFile - sizeof(WAVHEADER)) / blockSize);
    DWORD* blocks = malloc(blockCount * sizeof(DWORD));
    if (blocks == NULL) {
        fclose(inputFile);
        fclose(outputFile);
        fprintf(stderr, "Failed to allocate memory for blocks.\n");
        return EXIT_FAILURE;
    }

    // set the pointer to the last byte of the file, to read it backwards
    fseek(inputFile, -1, SEEK_END);
    int offset = blockSize;
    for (long i = 0; i < blockCount; i++) {
        // At each iteration, offset is moved exactly one block size away from
        // the last byte, moving forward until the header is reached. The values
        // are copied to the array of blocks
        if (fseek(inputFile, -offset, SEEK_END) != 0) {
            fprintf(stderr, "fseek failed on iteration %ld\n", i);
            // This will print a string explaining the error related to errno
            perror("Error");
            break;
        }

        if (fread(&blocks[i], sizeof(DWORD), 1, inputFile) != 1) {
            fprintf(stderr, "fread failed on iteration %ld\n", i);
            perror("Error");
            break;
        }

        offset += blockSize;
    }

    fclose(inputFile);

    // Move back to the beginning of the audio data, just after the header,
    // then copy the array to the new file. Given the array was populated in
    // reverse order, no need to do any other changes
    fseek(outputFile, sizeof(WAVHEADER), SEEK_SET);
    long blocksWritten = (long) fwrite(blocks, sizeof(DWORD), blockCount, outputFile);

    fclose(outputFile);
    free(blocks);

    if (blocksWritten != blockCount) {
        fprintf(stderr, "Incorrect number of blocks written to file.\n");
        return EXIT_FAILURE;
    }
}

bool checkFormat(WAVHEADER header)
{
    char f[] = "WAVE";

    for (int i = 0; i < 4; i++) {
        if (header.format[i] != f[i]) {
            return false;
        }
    }

    return true;
}

int getBlockSize(WAVHEADER header)
{
    return (int) (header.numChannels * (header.bitsPerSample / 8));
}