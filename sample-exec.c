#include <stdio.h>
#include <unistd.h>

int main () {
  
  // argv[n] = { {your-program-name}, {argument[1]}, {argument[2]},.....,{argument[n-2]}, NULL }
  char *argv[4] = {"mkdir:make-directory", "-l", "/", NULL};
  
  execvp("ls", argv);

  printf("This line will not be executed\n");

  return 0;
}
