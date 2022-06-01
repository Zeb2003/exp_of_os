#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N1 3
#define N2 4
#define M 10
//定义三个生产者 四个消费者以及十个缓冲区

int in = 0;
int out = 0;

int buff[M] = {0};

sem_t empty_sem;
sem_t full_sem;
pthread_mutex_t mutex;

int product_id = 0;
int consumer_id = 0;

int data;
FILE *fp;

void * producer()
{
    int id = ++product_id;
    while(1)
    {
        sleep(1);
        sem_wait(&empty_sem);
        pthread_mutex_lock(&mutex);
        if(fscanf(fp, "%d", &data)==EOF)
        {
            fseek(fp, 0, SEEK_SET);
            fscanf(fp, "%d", &data);
        }//在文件中读取数据
        in = in % M;
        buff[in] = data;
        printf("Producer [%d] produces product\t %d\t in position %d\t Now the buffer: %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\n",
               id, buff[in], in, buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7], buff[8], buff[9]);
               //打印出几号生产者在几号位生产出了什么以及当前缓冲区的情况
        ++in;
        pthread_mutex_unlock(&mutex);//unlock与lock必须成对出现
        sem_post(&full_sem);//post与wait必须成对出现
    }
}

void *consumer()
{
    int id = ++consumer_id;
    while(1)
    {
        sleep(1);
        sem_wait(&full_sem);
        pthread_mutex_lock(&mutex);

        out = out % M;
        printf("Consumer [%d] takes product\t %d\t in position %d\t Now the buffer: %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\t %d\n",
               id, buff[out], out, buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7], buff[8], buff[9]);
               //打印出几号消费者在几号位消费了什么以及当前缓冲区的情况
        buff[out] = 0;
        ++out;

        pthread_mutex_unlock(&mutex);//unlock与lock必须成对出现
        sem_post(&empty_sem);//post与wait必须成对出现
    }
}

int main()
{
    pthread_t id1[N1];
    pthread_t id2[N2];

    int i;
    int ret1[N1];
    int ret2[N2];

    int ini1 = sem_init(&empty_sem, 0, M);
    int ini2 = sem_init(&full_sem, 0, 0);
    if(ini1 && ini2 != 0)
    {
        printf("Semaphore initialization failed\n");
        exit(1);
    }//信号量初始化失败的情况

    int ini3 = pthread_mutex_init(&mutex, NULL);
    if(ini3 != 0 )
    {
        printf("Mutex initialization failed\n");
        exit(1);
    }//互斥量初始化失败的情况

    fp = fopen("./data.txt", "r");
    if(fp == NULL)
        exit(1);//打开数据文件

    for(i = 0; i < N1; i++)
    {
        ret1[i] = pthread_create(&id1[i], NULL, producer, (void*)(&i));
        if(ret1[i] != 0)
        {
            printf("Producer %d created failed\n", i);
            exit(1);
        }
    }//生产未成功的情况

    for(i = 0; i< N2; i++)
    {
        ret2[i] = pthread_create(&id2[i], NULL, consumer, NULL);
        if(ret2[i] != 0)
        {
            printf("Consumer %d created failed\n", i);
            exit(1);
        }
    }//消费未成功的情况

    for(i = 0; i < N1; i++) {pthread_join(id1[i], NULL);}
    for(i = 0; i < N2; i++) {pthread_join(id2[i], NULL);}

    exit(0);
}
