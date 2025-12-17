#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    // input and output filenames
    char *inputFileName = argv[1];
    char *outputFileName = argv[2];

    FILE *in = stdin;
    FILE *out = stdout;

    FILE *userInput = stdin;

    // IF NO ARGUMENTS
    if (argc == 1)
    {
        userInput = stdin;
    }

    // IF ONLY 1 ARGUMENT //lol also works for 3 argument file read
    else if (argc >= 2)
    {
        in = fopen(inputFileName, "r");
        if (in == 0)
        {
            prinf("error: cannot open file 'input.txt'");
            exit(1);
        }
        fileHandling(FILE * in, FILE * out)
    }

    // IF 2 ARGUMENTS
    else if (argc == 3)
    {

        inputFile = fopen(inputFileName, "r");
        readFile(userInput);

        // WRITE FILE
        FILE *WriteFile;
        WriteFile = fopen(outputFile, "w");
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

    exit(0);
}

void fileHandling(FILE *input, FILE *out)
{
    // OPEN FILE
    FILE *readFile;
    readFile = fopen(inputFileName, "r");

    // READ FILE
    getline(&buffer, &size, stdin);

    // REVERSE LINES

    return (0);
}
