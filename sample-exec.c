#include <stdio.h>
#include <unistd.h>

int main () {
  char *argv[10] = {"mkdir", "sisop", NULL};
  
  execvp("mkdir", argv);

  printf("This line will not be executed\n");

  return 0;
}
