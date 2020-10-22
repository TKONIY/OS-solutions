/***************************************
 * Author:    TKONIY
 * Date:      2020/10/22
 * Platform:  WSL-ubuntu18.04
 * Compiler:  gcc7.5.0
 ***************************************/
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/select.h>

// utils
void sleep_us(long s, long us) {
  struct timeval inteval;
  inteval.tv_sec = s;
  inteval.tv_usec = us;
  select(0, NULL, NULL, NULL, &inteval);
}

pthread_t create_thread(int detach_state, void* (*func)(void*)) {
  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, detach_state);
  pthread_create(&tid, &attr, func, NULL);
  return tid;
}

// barrier_point implementation
int N = 0, count = 0;
sem_t mutex;  // 互斥锁, 保护对N的访问
sem_t run;    // 信号量, 可以前进

int init(int n) {
  if (n < 0) return -1;
  N = n;
  count = 0;
  if (-1 == sem_init(&mutex, 0, 1)) return -1;
  if (-1 == sem_init(&run, 0, 0)) return -1;
}

int barrier_point() {
  if (count < 0 || N < 0) return -1;
  sem_wait(&mutex);
  if (count < N - 1) {  //第N个到达的时候count=N-1
    count++;
    // printf("block %d process\n", count);  // debug
    sem_post(&mutex);
    sem_wait(&run);
  } else {
    // printf("release all\n"); // debug
    while (count > 0) {  // wake up others
      sem_post(&run);
      count--;
    }
    sem_post(&mutex);  // wake up myself
  }
}

// threads using barrier for testing
void* thread(void* args) {
  while (1) {
    printf("before barrier\n");
    barrier_point();
    printf("after barrier\n");
    sleep_us(0, 10000);
  }
}

int main() {
  init(5);
  while (1) {
    create_thread(PTHREAD_CREATE_DETACHED, thread);
    sleep_us(0, 1000);
  }
}