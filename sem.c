#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

int ring[64];
sem_t blank_sem;
sem_t data_sem;

void *Producter()
{
    int step = 0;
    int data = 0;
    while(1)
    {
        sleep(1);
        sem_wait(&blank_sem);//blank_sem×ÊÔ´ -1
        data = rand()%1000;
        ring[step] = data;
        step++;
        step %= 64;
        printf("prodecter done...:%d\n", data);
        sem_post(&data_sem);//
    }
}

void *Comsumer()
{        
    int step = 0;
    int data = 0;
    while(1)
    {
        sem_wait(&data_sem);
        data = ring[step];
        step++;
        step %= 64;
        printf("consumer done...:%d\n", data);
        sem_post(&blank_sem);
    }

}

int main()
{
    sem_init(&blank_sem, 0, 64);
    sem_init(&data_sem, 0, 0);
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, Producter, NULL);
    pthread_create(&tid2, NULL, Comsumer, NULL);


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    sem_destroy(&blank_sem);
    sem_destroy(&data_sem);
    return 0;
}
