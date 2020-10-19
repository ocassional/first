#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__)
#define OS_WIN
#endif
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#ifdef OS_WIN
#define _CRT_SECURE_NO_WARNINGSs
#define REOPEN_PATH "CONIN$"
#define isatty _isatty
#define dup _dup
#define fdopen _fdopen
#define fileno _fileno
#else
#define REOPEN_PATH "/dev/tty"
#endif

#define BUF_SIZE 4096

char* keyboardInput()
{
    int input;
    size_t buf_size = BUF_SIZE;
    size_t index = 0;
    char* str = malloc(buf_size * sizeof(char));

    void* temp = NULL;
    while (str != NULL && (input = getchar()) && input != EOF)
    {
        // if allocated buffer completely used
        if (index == buf_size * sizeof(char))
        {   
            // extend the allocation by another BUF_SIZE
            buf_size += BUF_SIZE;
            if ((temp = realloc(str, buf_size * sizeof(char))) != NULL)
                str = (char*)temp;
            else
            {
                free(str);
                printf("realloc failed\n");
                return NULL;
            }
        }
        str[index++] = input;
    }

    if (feof(stdin)) printf("EOF occurred\n");
    if (ferror(stdin)) printf("Input error occurred\n");

    if (str != NULL)
    {
        str[index] = '\0';
        temp = realloc(str, strlen(str) + 1);
        if (temp == NULL)
        {
            free(str);
            printf("realloc failed\n");
            return NULL;
        }
        str = temp;
    }
    else return NULL;

    return str;
}

void readIndirection(FILE* fp, FILE* input)
{
    char s[1024] = { 0 };
    while(fread(s, sizeof(s), 1, input))
    {
        fwrite(s, sizeof(s), 1, fp);
    }

    if (feof(stdin)) printf("EOF occurred\n");
    if (ferror(stdin)) printf("Input error occurred\n");
}

int getFileName(char* fileName)
{
    if (fileName && *fileName == '\0')
    {
        printf("Enter the file name\n");
        (void)scanf(" %[^\n]%*c", fileName);
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[])
{
    FILE* fOut = NULL;
    char fileName[256] = { 0 };
    if (!isatty(fileno(stdin)))
    {
        int fd = dup(fileno(stdin));
        if (freopen(REOPEN_PATH, "r", stdin))
        {
            (void)getFileName(fileName);
            fOut = fopen(fileName, "w");
            if (fOut)
            {
                FILE* input = fdopen(fd, "r");
                if (input)
                {
                    readIndirection(fOut, input);
                    fclose(input);
                }
                else
                {
                    printf("fdopen failed\n");
                }

                fclose(fOut);
            }
            else
            {
                printf("fopen Failed\n");
            }
        }
        else
        {
            printf("freopoen failed\n");
        }
    }

    int retVal = getFileName(fileName);
    printf("Please enter the text followed by newline, press Ctrl D or Ctrl Z to exit\n");

    char* str = keyboardInput();

    if (str)
    {
        char mode[2] = { "w" };
        if (!retVal)
            strcpy(mode, "a");

        fOut = fopen(fileName, mode);
        if (fOut)
        {
            fputs(str, fOut);
            fclose(fOut);
        }
        else
        {
            printf("fopen failed\n");
        }
        free(str);
    }

    return 0;
}
