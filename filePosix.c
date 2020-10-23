#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 1024
int createFile(const char* fileName)
{
    int fd;
    char ch[BUF_SIZE];

    if(!fileName)
    {
        fprintf(stderr, "NULL fileName passed\n");
        return -1;
    }

    if ((fd = open(fileName, O_CREAT|O_TRUNC|O_WRONLY, 0644)) < 0)
    {
        fprintf(stderr, "Error[%s, line: %d]: Failed to open file : %s\n", __FILE__, __LINE__, fileName);
        return -1;
    }
    ssize_t bytes_read;
    while( (bytes_read = read(STDIN_FILENO, &ch, sizeof(ch))) > 0)
    {
        ssize_t retVal = write(fd, &ch, bytes_read);
        
        if(retVal != bytes_read)
        {
            fprintf(stderr, "Error[%s, line: %d]: write failed, Errno <%d>: %s\n", __FILE__, __LINE__, errno, strerror(errno));
            close(fd);
            return -1;
        }
    }

    if(bytes_read < 0)
    {
        fprintf(stderr, "Error[%s, line: %d]: read failed, Errno <%d>: %s\n", __FILE__, __LINE__, errno, strerror(errno));
        close(fd);
        return -1;
    }
    return close(fd);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        exit(1);
    }
    
    // some validation around fileName can go here before passing it to the createFile
    if (-1 == createFile(argv[1]))
    {
        fprintf(stderr, "File creation failed\n");
    }
    return 0;
}
