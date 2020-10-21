#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 1024

int main(int argc, char **argv)
{
    int fd;
    char ch[BUF_SIZE];

    if (2 != argc)
    {
        fprintf(stderr, "Error[%s, line: %d]: Usage: %s <output_file>\n", __FILE__, __LINE__, argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_CREAT|O_TRUNC|O_WRONLY, 0644)) < 0) 
    {
        fprintf(stderr, "Error[%s, line: %d]: Failed to open file : %s\n", __FILE__, __LINE__, argv[1]);
        exit(1);
    }
    ssize_t bytes_read;
    while( (bytes_read = read(STDIN_FILENO, &ch, sizeof(ch))) > 0)
    {
        ssize_t retVal = write(fd, &ch, bytes_read);
        if(retVal != bytes_read)
        {
            fprintf(stderr, "Error[%s, line: %d]: Errno <%d>: %s\n", __FILE__, __LINE__, errno, strerror(errno));
            exit(1);
        }
    }

    if(bytes_read < 0)
    {
        fprintf(stderr, "Error[%s, line: %d]: Errno <%d>: %s\n", __FILE__, __LINE__, errno, strerror(errno));
    }
    close(fd);

    return 0;
} 
