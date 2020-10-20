#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int fd;
    char ch;

    if (2 != argc)
    {
        fprintf(stderr, "usage: %s <output_file>\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_CREAT|O_TRUNC|O_WRONLY, 0644)) < 0) 
    {
        fprintf(stderr, "failed to open file : %s\n", argv[1]);
        exit(1);
    }
    while(read(STDIN_FILENO, &ch, 1) > 0)
    {
        write(fd, &ch, sizeof(ch));
    }
    close(fd);

    return 0;
} 
