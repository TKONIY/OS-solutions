/***************************************
 * Author:    TKONIY
 * Date:      2020/11/01
 * Platform:  WSL-ubuntu18.04
 * Compiler:  gcc7.5.0
 ***************************************/

#include <stdio.h>
#define N_FRAME 7     // 1~7
#define RANGE_PAGE 9  // 0~9

// utils
void InitFrames(int frame[N_FRAME]) {
  for (int i = 0; i < N_FRAME; ++i) frame[i] = -1;
}

void PrintFrames(int page_number, int frame[N_FRAME]) {
  printf("%3d : ", page_number);
  for (int i = 0; i < N_FRAME; ++i) printf("%3d ", frame[i]);
  printf("\n");
}

// core
int LRU(const int* ref, int n) {
  printf("running LRU:\n");
  int frame_stack[N_FRAME];
  InitFrames(frame_stack);
  int top = 0, n_page_fault = 0;
  for (int i = 0; i < n; ++i) {
    int page_number = ref[i], foundi = 0;
    for (; foundi < top; ++foundi) {
      if (page_number == frame_stack[foundi]) break;
    }
    //将foundi <= i < top 的元素全部下移
    if (foundi == top) {
      n_page_fault++;
      if (top == N_FRAME) {  //栈已满, 置换栈底元素
        foundi = 0;
      } else {
        // frame_stack[top++] = page_number;
        top++;
      }
    }
    //将foundi的元素(page_number)移到栈顶
    for (; foundi + 1 < top; foundi++) {
      frame_stack[foundi] = frame_stack[foundi + 1];
    }
    frame_stack[foundi] = page_number;

    PrintFrames(page_number, frame_stack);
  }
  return n_page_fault;
}

int FIFO(const int* ref, int n) {
  printf("running FIFO:\n");

  int frame_queue[N_FRAME];  //所有的frame是一个队列
  InitFrames(frame_queue);

  int front = 0, n_page_fault = 0;
  for (int i = 0; i < n; ++i) {
    int page_number = ref[i], found = 0;
    for (int j = 0; j < N_FRAME; ++j) {
      if (page_number == frame_queue[j]) {
        found = 1;
        break;
      }
    }
    if (!found) {
      frame_queue[front] = page_number;
      front = (front + 1) % N_FRAME;  // page faults
      n_page_fault++;
    }
    PrintFrames(page_number, frame_queue);
  }
  return n_page_fault;
}

int main() {
  int ref[] = {1, 2, 3, 4, 9, 10, 19, 2, 10, 6, 19, 10, 1, 30, 3, 6};

  printf("page fault = %d\n", LRU(ref, sizeof(ref) / sizeof(ref[0])));
  printf("page fault = %d\n", FIFO(ref, sizeof(ref) / sizeof(ref[0])));
}