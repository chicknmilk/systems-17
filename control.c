#include "control.h"

int main(int argc, char *argv[])) {

  if (argv[1] == NULL) {
    printf("add flag (either 'c' or 'r'\n", argv[0]);
    return 0;
  }

  printf("%s\n", argv[1]);

  return 0;
}