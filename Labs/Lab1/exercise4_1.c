#include <unistd.h>
#include <stdio.h>

int main(char argc, char **argv)
{
    execl("/bin/ls", "-l", argv[1], NULL);
    puts("Program ls has terminated");
}