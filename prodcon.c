/// @file prodcon.c
/// @brief - This is the implmentation of a producer consumer system utilizing semaphores and mutex locks.
/// @author Tyler Edwards - tk.edwards2003@gmail.com

#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

#include "buffer.h"

BUFFER_ITEM buffer[BUFFER_SIZE];
int pos = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int insert_item(BUFFER_ITEM item)
{
    pthread_mutex_lock(&mutex);

    if(pos >= BUFFER_SIZE || pos < 0)
    {
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    else
    {
        buffer[pos] = item;
        pos++;
        pthread_mutex_unlock(&mutex);
        return 0;
    }
}

int remove_item(BUFFER_ITEM *item)
{
    sem_wait(&full);
    pthread_mutex_lock(&mutex);

    if(pos == 0)
    {
        pthread_mutex_unlock(&mutex);
        return -1;
    }

    pthread_mutex_lock(&mutex);
    *item = buffer[0];

    for(int i = 1; i < pos; i++)
    {
        buffer[i - 1] = buffer[i];
    }

    for(int j = 0; j < 30; j++)
    {
        buffer[pos - 1].data[j] = 0;
    }

    pos--;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    return 0;
}

uint16_t checksum(uint8_t* buffer, int count)
{
    register uint32_t sum = 0;
    uint16_t *buf = (uint16_t *) buffer;
    while(count > 1)
    {
        sum = sum + *(buf)++;
        count = count - 2;
    }

    if (count > 0)
    {
        sum = sum + *buffer;
    }

    while (sum>>16)
    {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return(~sum);
}

void *producer()
{
    BUFFER_ITEM item;

    do
    {
        for(int i = 0; i < 30; i++)
        {
            item.data[i] = rand();
        }

        item.cksum = checksum(item.data, 30);

        if (insert_item(item) == 0)
        {
            printf("Producer produced %d\n", item.cksum);
        }

        sleep(1);
    } while (1);
}

void *consumer()
{
    BUFFER_ITEM item;

    do
    {
        sleep(1);

        if (remove_item(&item) == 0)
        {
            printf("Consumer consumed %d\n", item.cksum);
        }

    } while (1);
}


int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        printf("Wrong number of arguments.");
        exit(1);
    }

    int sleepTime = atoi(argv[1]);
    int producers = atoi(argv[2]);
    int consumers = atoi(argv[3]);

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t producer_threads[producers];
    pthread_t consumer_threads[consumers];

    for (int i = 0; i < producers; i++)
    {
        pthread_create(&producer_threads[i], NULL, &producer, NULL);
    }

    for (int j = 0; j < consumers; j++)
    {
        pthread_create(&consumer_threads[j], NULL, &consumer, NULL);
    }

    sleep(sleepTime);

    for (int i = 0; i < producers; i++)
    {
        pthread_cancel(producer_threads[i]);
    }

    for (int j = 0; j < consumers; j++)
    {
        pthread_cancel(consumer_threads[j]);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    exit(1);
}