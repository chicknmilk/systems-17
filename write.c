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
  lseek(fd, -*data, SEEK_END);

  char line[1024];
  int str = read(fd, line, 1024);
  line[*data] = '\0';

  // print the last line
  printf("The previous entered line is: %s\n", data);


  // use fgets to read a string from stdin
  char new_data[1024];

  printf("Enter new line: ");
  fgets(new_data, 1024, stdin);

  // append new_data to the end of the file
  lseek(fd, 0, SEEK_END);
  write(fd, new_data, strlen(new_data));
  *data = strlen(new_data);
  printf("wrote %s to file\n", new_data);


  shmdt(data);
  sb.sem_op = 1;
  semop(semd, &sb, 1);

  return 0;
}