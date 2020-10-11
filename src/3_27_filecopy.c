/*************************
 * Author:    TKONIY
 * Date:      2020/10/11
 * Platform:  WSL-ubuntu18.04
 * Compiler:  gcc7.5.0
 ************************/ 

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define WRITE_END 1
#define READ_END 0
#define BUF_SIZE 1024

void fail(const char* msg) {
  fprintf(stderr, "Faild: %s\n", msg);
  exit(-1);
}


int main(int argc, char** argv) {
  if (argc <= 2) fail("need 2 args.");

  int fd[2];
  if (pipe(fd) == -1) fail("pipe error.");

  pid_t pid = fork();
  if (pid < 0) {
    fail("fork error.");
  } else if (pid == 0) { /* child */
    int out_fd = open(argv[2], O_WRONLY | O_CREAT, 666);
    if (out_fd == -1) fail("open output file error.");

    char buf[BUF_SIZE];
    int n;
    close(fd[WRITE_END]);
    while ((n = read(fd[READ_END], buf, BUF_SIZE)) > 0) {
      write(out_fd, buf, n);
    }
    close(fd[READ_END]);
    close(out_fd);
    
  } else { /* parent */
    int in_fd = open(argv[1], O_RDONLY);
    if (in_fd == -1) fail("open input file error.");

    char buf[BUF_SIZE];
    int n;
    close(fd[READ_END]);
    while ((n = read(in_fd, buf, BUF_SIZE)) > 0) {
      write(fd[WRITE_END], buf, n);
    }
    close(fd[WRITE_END]);
    close(in_fd);
  }
  return 0;
}
