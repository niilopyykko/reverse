#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    FILE *readFile;

    int previousCharacter = EOF;
    int currentCharacter = 0;
    int counter = 0;

    if (argc < 2)
    {
        printf("my-zip: file1 [file2 ...]\n");
        return 1;
    }

    for (size_t i = 1; i < argc; i++)
    {
        readFile = fopen(argv[i], "r");

        if (readFile == NULL)
        {
            printf("my-unzip: cannot open file\n");
            exit(1);
        }
        while ((currentCharacter = fread(currentCharacter, 1, 1, readFile)) != EOF)
        {
            if (counter == 0)
            {
                previousCharacter = currentCharacter;
                counter = 1;
            }
            else if (previousCharacter == currentCharacter)
            {
                counter++;
            }
            else
            {
                fprintf(&counter, sizeof(int), 1, stdout);
                fprintf(&previousCharacter, sizeof(char), 1, stdout);
                previousCharacter = currentCharacter;
                counter = 1;
            }
        }
        fclose(readFile);
    }
    if (counter > 0)
    {
        fprintf(&counter, sizeof(int), 1, stdout);
        fprintf(&previousCharacter, sizeof(char), 1, stdout);
    }
    return (0);
}