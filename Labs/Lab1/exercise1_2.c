#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    for (int i = 0; i < 3; i++)
        fork();
}