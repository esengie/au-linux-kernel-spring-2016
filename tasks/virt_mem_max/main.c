#include <sys/stat.h>
#include <sys/mman.h> 
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(){
    size_t mem_size = -1;
    size_t res= 0;
    char *mem; 
    size_t cnt = 0;

    while (mem_size > 0){
        cnt ++;
        mem = mmap(0, mem_size,
            PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
        if (mem == MAP_FAILED) mem_size /= 2;
        else res += mem_size;
    }
    printf ("available memory %zu\n", res);
    printf("%zu\n", cnt);
    return 0;
}
