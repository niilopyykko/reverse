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

typedef struct
{
    // FOR INPUT
    char *data;
    size_t start;
    size_t end;

    // worker buffer // FOR OUTPUT
    char *buff;
    size_t buffsize;

} chunk;

void *worker(void *arg)
{
    chunk *chunk = arg; // worker input back to chuck structure
    int previousCharacter = 0;
    int counter = 0;

    for (size_t i = chunk->start; i < chunk->end; i++)
    {
        int currentCharacter = chunk->data[i];

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
        // memory addressing is needed for linking two or more chunks together in seperate worker instances
        // EXAMPLE: worker1 [aaaabb], worker2 [bbaaaa] should ultimately return [4a2b] and [2b4a]
        // this would be combined to [4a4b4a]
        {
            memcpy(chunk->buff + chunk->buffsize, &counter, sizeof(int)); // last matching character or only one match
            chunk->buffsize += sizeof(int);                               // clear buffer size
            chunk->buff[chunk->buffsize++] = previousCharacter;           // reset fist item of buffer to last char

            previousCharacter = currentCharacter; // save as above, but for local variables
            counter = 1;                          // save as above, but for local variables
        }
    }
    if (counter > 0)
    {
        memcpy(chunk->buff + chunk->buffsize, &counter, sizeof(int));
        chunk->buffsize += sizeof(int);
        chunk->buff[chunk->buffsize++] = previousCharacter;
    }
    return NULL;
}
int main(int argc, char *argv[])
{
    for (size_t i = 1; i < argc; i++)
    {
        int fd;
        char *addr;
        struct stat sb;

        int availableThreads = get_nprocs(); // THIS TELLS THE PARALLELLISM^TM system how many cores/threads is available
        pthread_t workers[availableThreads];
        chunk chunks[availableThreads];

        fd = open(argv[i], O_RDONLY);
        if (fd == -1)
        {
            perror("open");
            continue;
        }

        if (fstat(fd, &sb) == -1) /* To obtain file size */
        {
            perror("fstat");
            continue;
        }

        size_t filesize = sb.st_size;
        if (filesize == 0) // check for empty file
        {
            close(fd);
            continue;
        }
        addr = mmap(NULL, filesize, PROT_READ,
                    MAP_PRIVATE, fd, 0);
        if (addr == MAP_FAILED)
        {
            perror("mmap");
            close(fd);
            continue;
        }

        size_t chunk_size = filesize / availableThreads;

        for (size_t i = 0; i < availableThreads; i++)
        {
            chunks[i].data = addr;
            chunks[i].start = i * chunk_size;
            if (i == availableThreads - 1) // IF LAST WORKER END IS END OF FILE
                chunks[i].end = filesize;
            else
                chunks[i].end = chunks[i].start + chunk_size;                               // ELSE END OF CHUNK IS SIZE OF CHUNK FROM CHUNK START POSITION
            chunks[i].buff = malloc((chunks[i].end - chunks[i].start) * (sizeof(int) + 1)); // worst case scenario size for chunk lenght
            chunks[i].buffsize = 0;
            pthread_create(&workers[i], NULL, worker, &chunks[i]);
        }

        for (size_t i = 0; i < availableThreads; i++)
        {
            pthread_join(workers[i], NULL);
            fwrite(&chunks[i].buffsize, sizeof(int), 1, stdout);
            fwrite(&chunks[i].buff, sizeof(int), 1, stdout);
            free(chunks[i].buff);
        }

        munmap(addr, filesize);
        close(fd);
    }
    return (0);
}