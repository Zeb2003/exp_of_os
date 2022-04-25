#include<stdio.h>
#include<unistd.h>
#include<linux/wait.h>
#include<linux/sem.h>
#include<semaphore.h>
#include<stdlib.h>
#include<pthread.h>

#define PHILOSOPHER_NUM 5
#define CHOPSTICK_NUM 5

pthread_mutex_t chopstick[PHILOSOPHER_NUM];

void think(){
  int num = random() % 5;
  sleep(num);
}

void eat(int num){
  while(1){
    printf("%d is thinking...\n", num);
    printf("%d wants to eat\n", num);

    int wantChops[2] = {num, (num + 1) % 5};

    if(num % 2 == 0){
      int tmp = wantChops[0];
      wantChops[0] = wantChops[1];
      wantChops[1] = tmp;
    }

    pthread_mutex_lock(chopstick + wantChops[0]);
    pthread_mutex_lock(chopstick + wantChops[1]);
    printf("%d is eating with %d, %d chopsticks\n",
            num, wantChops[0], wantChops[1]);

    pthread_mutex_unlock(chopstick + wantChops[0]);
    pthread_mutex_unlock(chopstick + wantChops[1]);
    printf("%d finishes eating\n", num);

    think();
  }
}

int main(int argc, char* argv[]){
  srand((unsigned)time(NULL));

  pthread_t philosopher[PHILOSOPHER_NUM];

  for(int i = 0; i < CHOPSTICK_NUM; i++){
    pthread_mutex_init(chopstick + i, NULL);
  }

  for(long i = 0; i < PHILOSOPHER_NUM; i++){
    pthread_create(philosopher + i, NULL, (void*)&eat, (void*)i);
  }

  for(int i = 0; i < PHILOSOPHER_NUM; i++){
    pthread_join(philosopher[i], NULL);
  }

  for(int i = 0; i < CHOPSTICK_NUM; i++){
    pthread_mutex_destroy(chopstick + i);
  }
  return 0;
}
