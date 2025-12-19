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

    for (int i = chunk->start; i < chunk->end; i++)
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
            chunk->buff[chunk->buffsize] = previousCharacter;             // reset fist item of buffer to last char

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
        off_t offset, pa_offset;
        size_t length;
        struct stat sb;

        int availableThreads = get_nprocs(); // THIS TELLS THE PARALLELLISM^TM system how many cores/threads is available
        pthread_t workers[availableThreads];
        chunk chunks[availableThreads];

        {
            fprintf(stderr, "%s file offset [length]\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        fd = open(argv[1], O_RDONLY);
        if (fd == -1)
            perror("open");

        if (fstat(fd, &sb) == -1) /* To obtain file size */
            perror("fstat");

        offset = atoi(argv[2]);
        pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
        /* offset for mmap() must be page aligned */

        if (offset >= sb.st_size)
        {
            fprintf(stderr, "offset is past end of file\n");
            exit(EXIT_FAILURE);
        }

        if (argc == 4)
        {
            length = atoi(argv[3]);
            if (offset + length > sb.st_size)
                length = sb.st_size - offset;
            /* Can't display bytes past end of file */
        }
        else
        { /* No length arg ==> display to end of file */
            length = sb.st_size - offset;
        }

        addr = mmap(NULL, length + offset - pa_offset, PROT_READ,
                    MAP_PRIVATE, fd, pa_offset);
        if (addr == MAP_FAILED)
            perror("mmap");

        size_t chunk_size = length / availableThreads;

        for (size_t i = 1; i < availableThreads; i++)
        {
            chunks[i].data = addr;
            chunks[i].start = i * chunk_size;
            chunks[i].end = (i == availableThreads - 1);
            chunks[i].buff = malloc(chunks[i].end - chunks[i].start + 1); // worst case scenario size for chunk lenght
            chunks[i].buffsize = 0;
            pthread_create(&workers[i], NULL, worker, &chunks[i]);
        }

        for (size_t i = 1; i < availableThreads; i++)
        {
            pthread_join(workers[i], NULL);
            write(STDOUT_FILENO, chunks[i].buff, chunks[i].buffsize);
            free(chunks[i].buff);
        }

        munmap(addr, length + offset - pa_offset);
        close(fd);
    }
    return (0);
}