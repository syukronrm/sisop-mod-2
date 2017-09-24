#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;

  child_id = fork();
  
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    // this is child
    
    char *argv[] = {"mkdir", "sample-dir", NULL};
    execv("/bin/mkdir", argv);
  } else {
    // this is parent
  
    char *argv[] = {"touch", "sample-touch.txt", NULL};
    execv("/usr/bin/touch", argv);
  }
}
