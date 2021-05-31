#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int counter =0;
    int fd = open("loremipsum.txt", O_RDONLY);
    struct stat s;
    size_t size;
    int status = fstat(fd, &s);
    size = s.st_size;


    char *ptr = mmap(0, size, PROT_READ, MAP_SHARED, fd,0);
    if (ptr == MAP_FAILED)
    {
        printf("Mapping failed\n");
        return 1;
    }

    close(fd);

    int i;
    for (i =0; i < size; i++)
    {
        if('a' == ptr[i])
        {
            counter++;
        }
    }

    printf("Number of times 'a' occurred = %d\n", counter);


    status = munmap(ptr, size);
    if (status != 0)
    {
        printf("Unmapping failed\n");
        return 1;
    }

    return 0;
}
