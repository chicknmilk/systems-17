#include "control.h"

int main(int argc, char *argv[]) {

  if (argv[1] == NULL) {
    printf("add flag (either '-c' or '-r')\n");
    return 0;
  }

  if (strcmp(argv[1], "-c") == 0) {
    create();
  }
  else if (strcmp(argv[1], "-r") == 0) {
    rem();
  }

  return 0;
}

void create() {
  printf("creating file\n");

  int semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  if (semd == -1) {
    semd = semget(SEM_KEY, 1, 0);
  }

  union semun us;
  us.val = 1;
  int r = semctl(semd, 0, SETVAL, us);
  printf("semctl returned %d\n", r);

  // create the shared memory segment
  int shmd = shmget(SHM_KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);
  if (shmd == -1) {
    shmd = shmget(SHM_KEY, 0, 0);
  }

  int file = open("tmp.txt", O_CREAT | O_EXCL | O_TRUNC, 0644);
  if (file == -1) {
    file = open("tmp.txt", O_RDWR);
  }

  close(file);

  printf("create success!\n");
}

void rem() {
  printf("removing file\n");

  int semd = semget(SEM_KEY, 0, 0);
  if (semd == -1) {
    semd = semget(SEM_KEY, 1, 0);
  }

  // remove the shared memory segment
  semctl(semd, IPC_RMID, 0);

  int shmd = shmget(SHM_KEY, 0, 0);
  if (shmd == -1) {
    shmd = shmget(SHM_KEY, 1, 0);
  }

  shmctl(shmd, IPC_RMID, 0);

  // remove the file
  int file = open("tmp.txt", O_RDONLY);
  if (file == -1) {
    printf("open failed, maybe create file?\n");
    return;
  }

  // print the file contents
  struct stat sb;
  stat("tmp.txt", &sb);
  char text[sb.st_size];

  read(file, text, sb.st_size);

  printf("Here's all the stuff in the file:\n");
  printf("%s\n", text);
}