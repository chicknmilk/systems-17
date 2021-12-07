#include "write.h"

int main() {
  
  // GRAB THE SEMAPHORE!
  int semd = semget(SEM_KEY, 0, 0);

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_op = -1;
  sb.sem_flg = SEM_UNDO;

  semop(semd, &sb, 1);

  // read data from shared memory
  int shmd = shmget(SHM_KEY, 0, 0);
  char *data = shmat(shmd, 0, 0);


  // open file and print out the last line
  int file = open("tmp.txt", O_RDWR | O_APPEND);

  // print out the last line of the file
  char *line = malloc(sizeof(char) * 100);
  int i = 0;
  while (data[i] != '\n') {
    line[i] = data[i];
    i++;
  }
  line[i] = '\0';
  printf("%s\n", line);
  

}