#include <stdio.h>

int main(int argc, char *argv[])
{

    if (argc == 0)
    {
        FILE *userInput = stdin;
    }
    else if (argc == 1)
    {
        char *inputFile = argv[0];
        FILE *userInput = stdin;
        fprintf(getline(userInput).reverse());
    }

    else if (argc == 2)
    {
        char *inputFile = argv[0];

        FILE *userInput = stdin;
        readFile(userInput);
        char *outputFile = argv[1];

        // WRITE FILE
        FILE *WriteFile;
        WriteFile = fopen(outputFile, "w");
    }

    else
    {
        printf("wery broken c-code ah yes");
    }

    exit(1);
}

char readFile(inputFile)
{
    // OPEN FILE
    FILE *readFile;
    readFile = fopen(inputFile, "r");

    // READ FILE
    getline(&buffer, &size, stdin);

    // REVERSE LINES

    return (0)
}
