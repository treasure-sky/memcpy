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

    *SRC = malloc(file_size);
    if (*SRC == NULL)
    {
        perror("SRC malloc 실패");
        ret = -1;
        return ret;
    }

    *DEST = malloc(file_size);
    if (*DEST == NULL)
    {
        perror("DEST malloc 실패");
        free(*SRC); // SRC는 이미 할당된 상태이므로 이를 해제해줌.
        ret = -1;
        return ret;
    }

    return 0;
}

size_t getSize(int fd)
{
    size_t file_size = 0;

    struct stat st;
    if (fstat(fd, &st) == -1)
    {
        perror("fstat 실패");
        return -1;
    }

    file_size = (size_t)st.st_size;
    return file_size;
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
