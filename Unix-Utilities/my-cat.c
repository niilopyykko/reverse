#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
    if (argc <= 1) // error message
    {
        printf("my-cat: file1 [file2 ...]\n");
        exit(0);
    }
    for (size_t i = 1; i <= argc - 1; i++) // loop through files
    {
        FILE *fp = fopen(argv[i], "r"); // open file

        if (fp == NULL) // error opening file
        {
            printf("cannot open file\n");
            exit(1);
        }
        do
        {
            char buffer[100];                     // smol buffer for read (safe bc. fgets reads only 100)
            if ((fgets(buffer, 100, fp)) != NULL) // NULL CHECK FOR EMPTLY LINE IN FILE
            {
                printf("%s", buffer); // print the 100 char in buffer
            }
        } while (!feof(fp)); // check if end of file
        fclose(fp); // close file
    }
    return (0);
}