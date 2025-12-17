#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void fileHandling(FILE *in, FILE *out);

int main(int argc, char *argv[])
{
    // input and output filenames
    char *inputFileName = argv[1];
    char *outputFileName = argv[2];

    FILE *in = stdin;
    FILE *out = stdout;

    // IF NO ARGUMENTS
    if (argc == 1)
    {
        in = stdin;
    }

    // IF ONLY 1 ARGUMENT //lol also works for 3 argument file read
    if (argc >= 2)
    {
        in = fopen(inputFileName, "r");
        if (in == 0)
        // IF FILE OPEN FAIL
        {
            fprintf(stderr, "error: cannot open file 'input.txt'");
            exit(1);
        }
    }

    // IF 2 ARGUMENTS
    if (argc == 3)
    {
        // COMPARE INPUT OUTPUT FILENAMES
        if (strcmp(inputFileName, outputFileName) == 0)
        {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
        out = fopen(outputFileName, "w");
        // IF FILE OPEN FAIL
        if (out == 0)
        {
            fprintf(stderr, "error: cannot open file 'input.txt'");
            exit(1);
        }
    }

    // IF MORE THAN 2 ARGUMENTS
    else if (argc > 3)
    {
        printf("usage: reverse <input> <output>");
        exit(1);
    }

    // ELSE
    else
    {
        printf("wery broken c-code ah yes");
        fprintf(stderr, "whatever the error message is\n");
    }
    fileHandling(in, out);
    fclose(in);
    fclose(out);

    return (0);
}

void fileHandling(FILE *in, FILE *out)
{
    char *buffer = 0;
    size_t bufsize = 32;

    // write in reverse
    while (getline(&buffer, &bufsize, in) != -1)
    {
        fprintf(out, "%s", buffer);
    }
    free(buffer);
    return;
}
