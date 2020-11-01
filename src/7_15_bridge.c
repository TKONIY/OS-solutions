/***************************************
 * Author:    TKONIY
 * Date:      2020/11/01
 * Platform:  WSL-ubuntu18.04
 * Compiler:  gcc7.5.0
 ***************************************/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int waiting_north = 0;
int waiting_south = 0;
int on_bridge = 0;
int prev = 0; /*south*/
pthread_cond_t ok_to_cross;
pthread_mutex_t mutex; /*monitor's global mutex*/

void bridge_init() {
  waiting_north = 0;
  waiting_south = 0;
  on_bridge = 0;
  prev = 0; /*south*/
  pthread_cond_init(&ok_to_cross, NULL);
  pthread_mutex_init(&mutex, NULL);
}

void bridge_enter_south() {
  pthread_mutex_lock(&mutex);
  waiting_south++;
  while (on_bridge || (prev == 0 && waiting_north > 0)) {
    pthread_cond_wait(&ok_to_cross, &mutex);
  }
  on_bridge = 1;
  prev = 0;
  waiting_south--;
  pthread_mutex_unlock(&mutex);
}

void bridge_enter_north() {
  pthread_mutex_lock(&mutex);
  waiting_north++;
  while (on_bridge || (prev == 1 && waiting_south > 0)) {
    pthread_cond_wait(&ok_to_cross, &mutex);
  }
  on_bridge = 1;
  prev = 1;
  waiting_north--;
  pthread_mutex_unlock(&mutex);
}

void bridge_leave_south() {
  pthread_mutex_lock(&mutex);
  on_bridge = 0;
  pthread_cond_broadcast(&ok_to_cross);
  pthread_mutex_unlock(&mutex);
}

void bridge_leave_north() {
  pthread_mutex_lock(&mutex);
  on_bridge = 0;
  pthread_cond_broadcast(&ok_to_cross);
  pthread_mutex_unlock(&mutex);
}

/*testing*/
void* test_south(void* p) {
  while (1) {
    bridge_enter_south();
    printf("\nsouth crossing\n");
    bridge_leave_south();
  }
}
void* test_north(void* p) {
  while (1) {
    bridge_enter_north();
    printf("\nnorth crossing\n");
    bridge_leave_north();
  }
}

int main() {
  pthread_t pid;
  for (int i = 0; i < 5; ++i) pthread_create(&pid, NULL, test_south, NULL);
  for (int i = 0; i < 5; ++i) pthread_create(&pid, NULL, test_north, NULL);
  sleep(5);
  return 0;
}