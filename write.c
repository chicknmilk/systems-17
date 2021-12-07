#include "write.h"

int main() {
  
  // GRAB THE SEMAPHORE!
  int semd = semget(SEM_KEY, 1, 0);

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  sb.sem_flg = SEM_UNDO;

  semop(semd, &sb, 1);

  // read data from shared memory
  int shmd = shmget(SHM_KEY, 0, 0);
  char *data = shmat(shmd, 0, 0);


  // open file
  int fd = open("temp.txt", O_RDWR | O_APPEND);

  // use lseek to move to the end of the file
  lseek(fd, *data, SEEK_END);

  char line[1024];
  int str = read(fd, line, 1024);
  line[*data] = '\0';

  //print the last line
  printf("%s\n", line);


}