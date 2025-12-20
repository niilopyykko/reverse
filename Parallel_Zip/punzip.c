#define _GNU_SOURCE
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

pthread_mutex_t output_lock = PTHREAD_MUTEX_INITIALIZER; // lock for serial write

typedef struct
{
    const char *filename;
} fileSpecs; // struct for passing filename between worker and main

void *worker(void *arg)
{
    fileSpecs *file = arg; // worker input back to chuck structure
    FILE *readFile = fopen(file->filename, "rb");

    if (!readFile)
    {
        perror("punzip");
        return NULL;
    }

    int amount = 0;
    char character;

    while (fread(&amount, sizeof(int), 1, readFile) &&
           fread(&character, sizeof(char), 1, readFile))
    {
        pthread_mutex_lock(&output_lock); // lock thread so print is in order
        for (size_t i = 0; i < amount; i++)
        {
            printf("%c", character);
        }
        pthread_mutex_unlock(&output_lock); // unlock print for next file
    }
    fclose(readFile);
    return NULL;
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("my-unzip: file1 [file2 ...]\n");
        return 1;
    }
    pthread_t workers[argc - 1]; // one thread for each file
    fileSpecs tasks[argc - 1];   // list of tasks with filename from struct

    for (size_t i = 1; i < argc; i++)
    {
        tasks[i - 1].filename = argv[i];                              // take arguments as filename to structs
        pthread_create(&workers[i - 1], NULL, worker, &tasks[i - 1]); // create worker "processes" for each file
    }
    for (size_t i = 0; i < argc - 1; i++)
    {
        pthread_join(workers[i], NULL); //"return" of worker
    }
    return (0);
}