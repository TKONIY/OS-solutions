/*************************
 * Author:    TKONIY
 * Date:      2020/10/11
 * Platform:  WSL-ubuntu18.04
 * Compiler:  gcc7.5.0
 ************************/

#include <memory.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <wait.h>

#define MAX_SEQUENCE 10
typedef struct {
  long fib_sequence[MAX_SEQUENCE];
  int sequence_size;
} shared_data;

void fib(shared_data *data) {
  if (data->sequence_size == 0) return;
  data->fib_sequence[0] = 0;
  if (data->sequence_size == 1) return;
  data->fib_sequence[1] = 1;

  for (int i = 2; i <= data->sequence_size; ++i) {
    data->fib_sequence[i] =
        data->fib_sequence[i - 1] + data->fib_sequence[i - 2];
  }
}

int main(int argc, char **argv) {
  int n;
  if (argc == 1 || (n = atoi(argv[1])) < 0 || n >= MAX_SEQUENCE) {
    printf("please input %d > n >= 0.\n", MAX_SEQUENCE);
    return -1;
  }

  int seg_id = shmget(IPC_PRIVATE, sizeof(shared_data), S_IRUSR | S_IWUSR);
  shared_data *data = (shared_data *)shmat(seg_id, NULL, 0);

  data->sequence_size = n;
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork Faild");
    exit(-1);
  } else if (pid == 0) {
    fib(data);
  } else {
    wait(NULL);
    for (int i = 0; i < data->sequence_size; ++i) {
      printf("%ld ", data->fib_sequence[i]);
    }
  }

  shmdt(data);
  shmctl(seg_id, IPC_RMID, NULL);
  return 0;
}