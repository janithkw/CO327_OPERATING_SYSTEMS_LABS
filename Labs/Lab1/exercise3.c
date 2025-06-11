#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int pid;
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    {
        sleep(1); // delay child
        puts("This is the child process");
    }
    else
    {
        wait(NULL); // wait for child to finish
        puts("This is the parent process");
    }
    return 0;
}