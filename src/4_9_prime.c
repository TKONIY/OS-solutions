/*************************
 * Author:    TKONIY
 * Date:      2020/10/11
 * Platform:  WSL-ubuntu18.04
 * Compiler:  gcc7.5.0
 ************************/

#include <malloc.h>
#include <math.h>
#include <memory.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* print_primes(void* params) {
  int max_n = *(int*)params;
  /*此处不是重点, 仅使用大一学生方法实现,欢迎修改*/
  for (int i = 2; i <= max_n; ++i) {
    int is_prime = 1;
    for (int j = 2; j <= sqrt(i); ++j) {
      if (!(i % j)) {
        is_prime = 0;
        break;
      }
    }
    if (is_prime) printf("%d ", i);
  }
  pthread_exit(0);
}

int main(int argc, char** argv) {
  int max_n;
  if (argc == 1 || (max_n = atoi(argv[1])) <= 0) {
    printf("Please input a positive number.\n");
    return -1;
  }
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_t tid;
  pthread_create(&tid, &attr, print_primes, &max_n);
  pthread_join(tid, NULL);
  return 0;
}