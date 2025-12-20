#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// TEXT COLOR //used to make my-grep more like the real thing
#define TEXT_WHITE "\x1b[37m"
#define TEXT_RED "\x1b[31m"
#define TEXT_MAGENTA "\x1b[35m"
#define TEXT_CYAN "\x1b[36m"

// Source for information: https://c-for-dummies.com/blog/?p=5270

// TEXT BOLD //used to make my-grep more like the real thing
#define TEXT_BOLD "\e[1m"
#define TEXT_BOLD_OFF "\e[m"
// Source for information: https://stackoverflow.com/questions/71274207/how-to-bold-text-in-c-program

int main(int argc, char *argv[])
{
    char *buffer = 0;    // for getline
    size_t bufsize = 32; // for getline
    FILE *fp;

    if (argc <= 1) // eror
    {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }

    char *hakusana = argv[1]; // the word to search

    if (argc == 2) // if only 2 arguments
    {
        while (getline(&buffer, &bufsize, stdin) != -1) // read user input
        {
            if (strstr(buffer, hakusana) != 0) // man strstr //compare string in buffer and searchterm
            {
                printf(TEXT_BOLD TEXT_RED "%s" TEXT_BOLD_OFF TEXT_WHITE, buffer); // should not print whole buffer red, but only searchterm...too lazy to fix
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

        while (getline(&buffer, &bufsize, fp) != -1)
        {
            if (strstr(buffer, hakusana) == NULL) // make not matching lines not print
            {
                continue;
            }
            char *pos = buffer;
            char *match;
            if (hakusana[0] == '\0') // empty string match all lines, just like real grep
            {
                printf("%s", buffer);
            }
            else
            {
                char *filename = ""; // HERE we add the file identification if multiple files are to be searched
                if (argc > 3)
                {
                    filename = argv[i];
                    printf(TEXT_MAGENTA "%s" TEXT_CYAN ":" TEXT_WHITE,
                           filename);
                }
                while ((match = strstr(pos, hakusana)) != NULL) // man strstr
                {

                    // match - pos returns long int witch is cast into int, then it is just some number of position lenght to be printed
                    printf("%.*s", (int)(match - pos), pos);                            // leading words,
                    printf(TEXT_BOLD TEXT_RED "%s" TEXT_BOLD_OFF TEXT_WHITE, hakusana); // search term in red and bold,
                    pos = match + strlen(hakusana);                                     // increment index position indicator by found word leght
                }

                printf("%s", pos); // following words after no more matches
            }
        }
        fclose(fp);
    }
    free(buffer);
    return (0);
}
