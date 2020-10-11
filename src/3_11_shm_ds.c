/*************************
 * Author:    TKONIY
 * Date:      2020/10/11
 * Platform:  WSL-ubuntu18.04
 * Compiler:  gcc7.5.0
 ************************/ 

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define N_ATTACH 50

void print_shm_ds(int segment_id) {
  struct shmid_ds shmbuffer;
  shmctl(segment_id, IPC_STAT, &shmbuffer);
  printf(
      "Segment ID-----------%d\n"
      "Key------------------%d\n"
      "Mode-----------------%hu\n"
      "Owner UID------------%hu\n"
      "Size-----------------%ld\n"
      "Number of attaches---%ld\n",
      segment_id, shmbuffer.shm_perm.__key, shmbuffer.shm_perm.mode,
      shmbuffer.shm_perm.uid, shmbuffer.shm_segsz, shmbuffer.shm_nattch);
}

int main() {
  /* get a new segment */
  int seg_id = shmget(IPC_PRIVATE, 1024, S_IRUSR | S_IWUSR);
  /* create N_ATTACH attach (Just for testing)*/
  for (int i = 0; i < N_ATTACH; ++i) shmat(seg_id, NULL, 0);
  /* print info */
  print_shm_ds(seg_id);
  /* remove shm */
  shmctl(seg_id, IPC_RMID, NULL);
  return 0;
}