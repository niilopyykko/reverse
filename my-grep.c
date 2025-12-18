#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    char *buffer = 0;    // for getline
    size_t bufsize = 32; // for getline
    FILE *fp;

    if (argc <= 1)
    {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }

    char *hakusana = argv[1];

    if (argc == 2)
    {
        while (getline(&buffer, &bufsize, stdin) != -1)
        {
            if (strstr(buffer, hakusana) != 0) // man strstr
            {
                printf("%s", buffer);
            }
        }
    }
    for (size_t i = 2; i <= argc - 1; i++)
    {
        fp = fopen(argv[i], "r");

        if (fp == NULL)
        {
            printf("my-grep: cannot open file\n");
            exit(1);
        }

        while (!feof(fp))
        {
            getline(&buffer, &bufsize, fp);
            if (strstr(buffer, hakusana) != 0) // man strstr
            {
                printf("%s", buffer);
            }
        }
        free(buffer);
        fclose(fp);
    }

    return (0);
}
