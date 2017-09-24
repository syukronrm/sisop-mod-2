#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;
  int status;

  child_id = fork();

  if (child_id == 0) {
    // this is child

    char *argv[4] = {"mkdir", "-p", "sample-dir", NULL};
    execv("/bin/mkdir", argv);
  } else {
    // this is parent

    // the parent waits for all the child processes
    while ((wait(&status)) > 0);

    char *argv[3] = {"touch", "sample-dir/sample-touch.txt", NULL};
    execv("/usr/bin/touch", argv);
  }
}
