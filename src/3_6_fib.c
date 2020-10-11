/*************************
 * Author:    TKONIY
 * Date:      2020/10/11
 * Platform:  WSL-ubuntu18.04
 * Compiler:  gcc7.5.0
 ************************/ 

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

void fib(int n) {
  if (n == 0) return;
  if (n == 1) {
    printf("0\n");
    return;
  }

  long long *dp = malloc((n + 1) * sizeof(long long));
  dp[0] = 0;
  dp[1] = 1;
  for (int i = 2; i < n; ++i) {
    dp[i] = dp[i - 1] + dp[i - 2];
  }
  for (int i = 0; i < n; ++i) {
    printf("%lld ", dp[i]);
  }
  printf("\n");
}

int main() {
  pid_t pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork Faild");
    exit(-1);
  } else if (pid == 0) {
    int n = 0;
    do {
      printf("please input n >=0:\n");
      scanf("%d", &n);
    } while (n < 0);
    fib(n);
  } else {
    wait(NULL);
  }
}
