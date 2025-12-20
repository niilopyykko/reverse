#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("my-cat: file1 [file2 ...]\n");
        exit(0);
    }
    for (size_t i = 1; i <= argc - 1; i++)
    {
        FILE *fp = fopen(argv[i], "r");

        if (fp == NULL)
        {
            printf("cannot open file\n");
            exit(1);
        }
        do
        {
            char buffer[100];
            if ((fgets(buffer, 100, fp)) != NULL) // NULL CHECK FOR EMPTLY LINE IN FILE
            {
                printf("%s", buffer);
            }
        } while (!feof(fp));
        fclose(fp);
    }
    return (0);
}