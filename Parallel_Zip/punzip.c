#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    FILE *readFile;

    int amount = 0;
    char character;

    if (argc < 2)
    {
        printf("my-unzip: file1 [file2 ...]\n");
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
        while (fread(&amount, sizeof(int), 1, readFile) &&
               fread(&character, sizeof(char), 1, readFile))
        {
            for (size_t i = 0; i < amount; i++)
            {
                printf("%c", character);
            }
        }
        fclose(readFile);
    }
    return (0);
}