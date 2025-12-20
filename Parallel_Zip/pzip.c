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
    chunk *chunk = arg;        // worker input back to chunk structure
    int previousCharacter = 0; // init of counters
    int counter = 0;           // init of counters

    for (size_t i = chunk->start; i < chunk->end; i++) // from start of chunk to end of chunk
    {
        int currentCharacter = chunk->data[i]; // assing one letter

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

        // Combining multiple files is done in main thread, because splitting up multiple files to multiple workers and making it print in order would be a nightmare
        {
            memcpy(chunk->buff + chunk->buffsize, &counter, sizeof(int)); // last matching character or only one match
            chunk->buffsize += sizeof(int);                               // clear buffer size
            chunk->buff[chunk->buffsize++] = previousCharacter;           // reset fist item of buffer to last char

            previousCharacter = currentCharacter; // save as above, but for local variables
            counter = 1;                          // save as above, but for local variables
        }
    }
    if (counter > 0) // same as above but for end of chunk
    {
        memcpy(chunk->buff + chunk->buffsize, &counter, sizeof(int));
        chunk->buffsize += sizeof(int);
        chunk->buff[chunk->buffsize++] = previousCharacter;
    }
    return NULL;
}
int main(int argc, char *argv[])
{
    // variables for consolidating 2 files with common last/fist character
    int pendingAmount = 0;
    unsigned char pendingChar = 0;
    int isPending = 0; // ah yes boolean (should be true or false)

    for (size_t i = 1; i < argc; i++) // for loop for every file
    {
        int fd;         // file
        char *addr;     // filedata
        struct stat sb; // used for mmap

        int availableThreads = get_nprocs(); // THIS TELLS THE PARALLELLISM^TM system how many cores/threads is available
        pthread_t workers[availableThreads];
        chunk chunks[availableThreads];

        fd = open(argv[i], O_RDONLY);
        if (fd == -1)
        {
            perror(argv[i]);
            close(fd);
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
        if (availableThreads > filesize) // limit threads to files to prevent empty threads
            availableThreads = filesize;

        addr = mmap(NULL, filesize, PROT_READ,
                    MAP_PRIVATE, fd, 0); // mmap man pages should explain this, because I can't
        if (addr == MAP_FAILED)
        {
            perror("mmap");
            close(fd);
            continue;
        }

        size_t chunk_size = filesize / availableThreads; // split data into chunks depending on system resources

        for (size_t i = 0; i < availableThreads; i++) // for every thread
        {
            chunks[i].data = addr;            // give chunk address of read data
            chunks[i].start = i * chunk_size; // give chunk a place to start
            if (i == availableThreads - 1)    // IF LAST WORKER END IS END OF FILE
                chunks[i].end = filesize;     // last worker
            else                              // not last worker
                chunks[i].end = chunks[i].start + chunk_size;
            chunks[i].buff = malloc((chunks[i].end - chunks[i].start) * (sizeof(int) + 1)); // worst case scenario size for chunk lenght plus some(maybe)
            chunks[i].buffsize = 0;                                                         // reset
            pthread_create(&workers[i], NULL, worker, &chunks[i]);                          // spawn worker
        }

        for (size_t i = 0; i < availableThreads; i++)
        {
            pthread_join(workers[i], NULL); // make worker return
        }

        for (size_t i = 0; i < availableThreads; i++)
        {
            char *character = chunks[i].buff;                     // start of buffer
            char *nextCharacter = character + chunks[i].buffsize; // end of buffer

            while (character < nextCharacter) // while not at end
            {
                int count;                      // amount of characters
                unsigned char currentCharacter; // what character

                memcpy(&count, character, sizeof(int));
                character += sizeof(int);

                currentCharacter = *(unsigned char *)character;
                character += 1; // advance "reading head"

                if (isPending && currentCharacter == pendingChar) // if character is the same as previous
                {
                    pendingAmount += count; // add one more
                }
                else // if character is not the same
                {
                    if (isPending) // but there was 2 or more of the same character, we print the pending characters
                    {
                        fwrite(&pendingAmount, sizeof(int), 1, stdout);
                        fwrite(&pendingChar, 1, 1, stdout);
                    }
                    pendingAmount = count;          // reset
                    pendingChar = currentCharacter; // change pending char to first char of this type (example aa[e]eee) this would have just written 2a and pending char would be [e]
                    isPending = 1;                  // ah yes boolean (should be true or false)
                }
            }

            free(chunks[i].buff);
        }

        munmap(addr, filesize);
        close(fd);
    }
    if (isPending) // takes care of last char of last file...maybe
    {
        fwrite(&pendingAmount, sizeof(int), 1, stdout);
        fwrite(&pendingChar, 1, 1, stdout);
    }
    return (0);
}