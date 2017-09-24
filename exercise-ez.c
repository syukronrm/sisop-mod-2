#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void run_a() {
  int i;
  for (i = 0; i < 100; i++) {
    printf("%d\n", i);
  }
}

void run_b() {
  int i;
  for (i = 100; i < 200; i++) {
    printf("%d\n", i);
  }
}

int main() {
  pid_t child_id;

  child_id = fork();
  
  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    run_a();
  } else {
    run_b();
  }
}
