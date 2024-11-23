#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>

#define __rdtsc()	({uint32_t tickl,tickh; \
			asm volatile ("rdtsc":"=a"(tickl), "=d"(tickh));\
			((uint64_t)tickh << 32)|tickl; })
extern int errno;
extern size_t getSize(int fd);
extern int alloc_SRC_and_DEST(char** SRC, char** DEST,size_t size);

extern void old_memcpy(char* DEST, char* SRC, size_t size);
extern void movs_memcpy(char* DEST, char* SRC, size_t size);
extern void custom_memcpy(char* DEST, char* SRC, size_t size);

int main(int argc, char *argv[])
{
    unsigned long time1,time2;
    int i=0,fd;
    char *SRC=NULL ,*DEST=NULL;
    size_t ret=0, file_size=0, alloc_size=0;

    if( argc == 2 ) {
	printf("Input File Name: %s\n", argv[1]);
    }else{
	printf("An Usage Example: %s /proc/cpuinfo\n",argv[0]);
	exit(-1);
    }
    fd = open(argv[1],O_RDONLY);

    if (fd == -1){
	strerror(errno); exit(-1);
    }
    file_size = getSize(fd);
    printf("File size is %lu bytes.\n",file_size);
#define ITER (40960)
    alloc_size = file_size * ITER;

    if(alloc_SRC_and_DEST(&SRC, &DEST,alloc_size) == -1)
	exit(-1);
    printf("Allocated size is %lu bytes.\n",alloc_size);
    printf("SRC Addr. : %p\n",SRC);
    printf("DEST Addr. : %p\n",DEST);

    for(int i=0;i<ITER;i++){
	if(read(fd,SRC+(i*file_size),file_size) == -1){
	    strerror(errno); exit(-1);
	}
	asm volatile("mfence;":::);
	if(lseek(fd,0L,SEEK_SET) == -1){
	    strerror(errno); exit(-1);
	}
    }

    if(close(fd) == -1){
	strerror(errno); exit(-1);
    }

    time1 = __rdtsc();
#ifdef OLD_MEMCPY 
    old_memcpy(DEST,SRC,alloc_size);
#elif CUR_MEMCPY
    memcpy(DEST,SRC,alloc_size);
#elif MOVS_MEMCPY
    movs_memcpy(DEST,SRC,alloc_size);
#elif CUSTOM_MEMCPY
    custom_memcpy(DEST,SRC,alloc_size);
#else
    time2 = __rdtsc();
    printf("PLEASE CHECK MACRO\n");
    free(SRC);free(DEST);
    exit(-1);
#endif
    time2 = __rdtsc();
    printf("Elapsed cycles are %lu\n", time2 - time1);
    printf("Checking integrity: %s\n",(!memcmp(SRC,DEST,alloc_size)?"PASS":"FAILED"));
    free(SRC);free(DEST);
    return 0;
}
