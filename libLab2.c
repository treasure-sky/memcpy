#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

void old_memcpy(char *DEST, char *SRC, size_t size)
{
    char *d = DEST;
    const char *s = SRC;
    while (size--)
        *d++ = *s++;
}

int alloc_SRC_and_DEST(char **SRC, char **DEST, size_t file_size)
{
    int ret = 0;

    /*
     *  write your code
     */
    return ret;
}

size_t getSize(int fd)
{
    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        perror("fstat 실패");
        return -1;
    }
    return (size_t)st.st_size;
}

void movs_memcpy(char *DEST, char *SRC, size_t size)
{
    /*
     *  write your code
     */
}

void custom_memcpy(char *DEST, char *SRC, size_t size)
{
    /*
     *  write your code
     */
}
