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
        perror("malloc error: SRC");
        ret = -1;
        return ret;
    }

    *DEST = malloc(file_size);
    if (*DEST == NULL)
    {
        perror("malloc error: DEST");
        free(*SRC); // SRC는 이미 할당된 상태이므로 이를 해제해줌
        ret = -1;
        return ret;
    }

    return 0;
}

size_t getSize(int fd)
{
    // 현재 파일 포인터 저장
    off_t now_p = lseek(fd, 0, SEEK_CUR);
    if (now_p == -1)
    {
        perror("lseek error: now_p");
        return -1;
    }

    // 파일 포인터 끝으로 이동해서 크기 얻기
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1)
    {
        perror("lseek error: file_size");
        return -1;
    }

    // 파일 포인터를 기존 위치로 백업
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek error: backup");
        return -1;
    }

    return (size_t)file_size;
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
