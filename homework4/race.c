#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 10
#define LOOPS 1000000

int account_balance = 1000;

void *deposit(void *thread_id)
{
    for (int i = 0; i < LOOPS; ++i)
    {
        account_balance += 100;
    }
    pthread_exit(NULL);
}

void *withdraw(void *thread_id)
{
    for (int i = 0; i < LOOPS; ++i)
    {
        account_balance -= 100;
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];

    // Create deposit threads
    for (long t = 0; t < NUM_THREADS / 2; ++t)
    {
        if (pthread_create(&threads[t], NULL, deposit, (void *)t) != 0)
        {
            perror("pthread_create");
            exit(1);
        }
    }

    for (long t = NUM_THREADS / 2; t < NUM_THREADS; ++t)
    {
        if (pthread_create(&threads[t], NULL, withdraw, (void *)t) != 0)
        {
            perror("pthread_create");
            exit(1);
        }
    }

    for (long t = 0; t < NUM_THREADS; ++t)
    {
        if (pthread_join(threads[t], NULL) != 0)
        {
            perror("pthread_join");
            exit(1);
        }
    }

    printf("Final account balance: %d\n", account_balance);

    return 0;
}
