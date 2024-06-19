#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    if (fork() == 0)
    {
        while (1)
        {
            printf("%-5d : George\n", getpid());
            sleep(1);
        }
    }
    else
    {
        if (fork() == 0)
        {
            while (1)
            {
                printf("%-5d : Mary\n", getpid());
                sleep(2);
            }
        }
        else
        {
            while (1)
            {
                printf("%-5d----------------\n", getpid());
                sleep(1);
            }
        }
    }
}