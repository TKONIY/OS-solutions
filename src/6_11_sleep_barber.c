/***************************************
 * Author:    TKONIY
 * Date:      2020/10/21
 * Platform:  WSL-ubuntu18.04
 * Compiler:  gcc7.5.0
 *
 * 参考
 *https://blog.csdn.net/Wind__Chaser/article/details/88926639?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase
 * 思考:
 * 1. sem_post(barbers) 和 sem_post(mutex) 能否交换顺序?
 * 2. sem_post(customers) 和 sem_post(mutex) 能否交换顺序?
 *
 * 个人认为可以。
 ***************************************/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/select.h>  //使用select实现微秒级定时器
#include <unistd.h>

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

#define CHAIRS 5  // num of chairs

sem_t customers;  // 0 resource of waiting customers
sem_t barbers;    // 0 resource of barber ( sleep at the begining)
sem_t mutex;      // 1 lock the waiting counter
int waiting = 0;  // 0 count the waiting number

void cut_hair() {
  printf("waiting=%d barber cut a customer\n", waiting);
  sleep_us(1, 0);
}

void get_haircut() {
  printf("waiting=%d a customer get haircut\n", waiting);
  sleep_us(1, 0);
}

void leave() { printf("waiting=%d a customer leaved\n", waiting); }

void* barber(void* arg) {
  while (1) {
    sem_wait(&customers);  // waiting for a customer, sleep
    sem_wait(&mutex);      // lock "waiting"
    waiting--;             // serve a waiting customer
    sem_post(
        &barbers);     // notify someone to get in barber room and wait for him
    sem_post(&mutex);  // unlock "waiting"
    cut_hair();        // serve the customer
  }
}
void* customer(void* arg) {
  sem_wait(&mutex);  // lock "waiting"
  if (waiting < CHAIRS) {
    waiting++;             // increase the waiting count
    sem_post(&customers);  // signal a customer
    sem_post(&mutex);      // unlock "waiting"
    sem_wait(&barbers);  // wait for the barber's notification, ready to get in
                         // barber room
    get_haircut();       // get hair cut
  } else {
    sem_post(&mutex);  // unlock "waiting"
    leave();           // leave the babershop
  }
}

int main() {
  sem_init(&customers, 0, 0);
  sem_init(&barbers, 0, 0);
  sem_init(&mutex, 0, 1);
  create_thread(PTHREAD_CREATE_DETACHED, barber);  // create a barber
  while (1) {
    create_thread(PTHREAD_CREATE_DETACHED,
                  customer);  // create a stream of customers
    sleep_us(0, 700000);
  }
}