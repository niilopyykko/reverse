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
        while (!feof(fp))
        {
            char buffer[100];
            fgets(buffer, 100, fp);
            printf("%s", buffer);
        }
        fclose(fp);
    }
    return (0);
}