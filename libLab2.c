#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <immintrin.h>

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

    // 32비트 정렬하여 메모리 할당
    *SRC = aligned_alloc(32, file_size);
    if (*SRC == NULL)
    {
        perror("memmory allocation error(SRC)");
        ret = -1;
        return ret;
    }

    *DEST = aligned_alloc(32, file_size);
    if (*DEST == NULL)
    {
        perror("memmory allocation error(DEST)");
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
        perror("lseek error(now_p)");
        return -1;
    }

    // 파일 포인터 맨 앞으로 이동
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek error(file_pointer to start)");
        return -1;
    }

    // 512바이트씩 읽어가며 파일사이즈 계산
    char buffer[512];
    size_t bytesRead;
    size_t file_size = 0;

    while (1)
    {
        bytesRead = read(fd, buffer, 512);
        if (bytesRead > 0)
        {
            file_size += bytesRead;
        }
        else
        {
            break;
        }
    }

    // 파일 포인터를 기존 위치로 백업
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("lseek error(backup)");
        return -1;
    }

    return (size_t)file_size;
}

void movs_memcpy(char *DEST, char *SRC, size_t size)
{
    __asm__ __volatile__(
        "rep movsb"
        :
        : "D"(DEST), "S"(SRC), "c"(size)
        : "memory");
}

__attribute__((target("avx2"))) void custom_memcpy(char *DEST, char *SRC, size_t size)
{
    // loop unrolling을 통해 128바이트 단위로 복사
    while (size >= 128)
    {
        // 다음 데이터를 prefetch
        _mm_prefetch((const char *)(SRC + 128), _MM_HINT_T0);
        _mm_prefetch((const char *)(DEST + 128), _MM_HINT_T0);

        // AVX2 명령어를 사용
        __m256i data0 = _mm256_load_si256((__m256i *)(SRC));
        __m256i data1 = _mm256_load_si256((__m256i *)(SRC + 32));
        __m256i data2 = _mm256_load_si256((__m256i *)(SRC + 64));
        __m256i data3 = _mm256_load_si256((__m256i *)(SRC + 96));

        _mm256_store_si256((__m256i *)(DEST), data0);
        _mm256_store_si256((__m256i *)(DEST + 32), data1);
        _mm256_store_si256((__m256i *)(DEST + 64), data2);
        _mm256_store_si256((__m256i *)(DEST + 96), data3);

        DEST += 128;
        SRC += 128;
        size -= 128;
    }

    // 128바이트 보다 작은 나머지를 32바이트 단위로 복사
    while (size >= 32)
    {
        // 다음 데이터를 prefetch
        _mm_prefetch((const char *)(SRC + 32), _MM_HINT_T0);
        _mm_prefetch((const char *)(DEST + 32), _MM_HINT_T0);

        __m256i data = _mm256_load_si256((__m256i *)SRC);
        _mm256_store_si256((__m256i *)DEST, data);

        DEST += 32;
        SRC += 32;
        size -= 32;
    }

    // 남은 모든 데이터를 1바이트씩 복사
    while (size > 0)
    {
        *DEST = *SRC;
        DEST++;
        SRC++;
        size--;
    }
}
