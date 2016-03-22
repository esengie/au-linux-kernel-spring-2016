#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include "../vsd_driver/vsd_ioctl.h"

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "size_get") == 0) {
        FILE *f = fopen("/dev/vsd", "r+");
        if (f == 0) {
            printf("Device not found.\n");
            return EXIT_FAILURE;
        }

        vsd_ioctl_get_size_arg_t vsd_ioctl_get_size_arg;
        int res = ioctl(fileno(f), VSD_IOCTL_GET_SIZE, &vsd_ioctl_get_size_arg);
        fclose(f);

        if (res != 0) {
            printf("Ioctl error code = %d.\n", res);
            return EXIT_FAILURE;
        }

        printf("Current size = %d\n", (int)vsd_ioctl_get_size_arg.size);
        return EXIT_SUCCESS;
    } 
    
    if (argc == 3 && strcmp(argv[1], "size_set") == 0) {
        FILE * f = fopen("/dev/vsd", "r+");
        if (f == 0) {
            printf("Device not found.\n");
            return EXIT_FAILURE;
        }
        errno = 0;

        char *end = argv[2] + strlen(argv[2]);
        int nsize = strtol(argv[2], &end, 10);

        if (errno || nsize == 0) {
            printf("Can't parse number.\n");
            printf("%s\n", strerror(errno));
            fclose(f);
            return EXIT_FAILURE;
        }

        vsd_ioctl_set_size_arg_t vsd_ioctl_set_size_arg;
        vsd_ioctl_set_size_arg.size = nsize;
        int res = ioctl(fileno(f), VSD_IOCTL_SET_SIZE, &vsd_ioctl_set_size_arg);
        fclose(f);

        if (res != 0) {
            printf("Ioctl error code = %d.\n", res);
            return EXIT_FAILURE;
        }
        printf("Size changed to %d\n", (int)vsd_ioctl_set_size_arg.size);
        
    } else {
        printf("Usage rules:\n");
        printf("'vsd_userspace size_get' prints current buffer size\n");
        printf("'vsd_userspace size_set NUM' sets buffer size\n");
    }
}