# Compiling C Program

`my-program.c`
```C
#include <stdio.h>

int main(){
  printf("Hello World!");
  return 0;
}
```

Compiling

`gcc -o {outfile} {infile}`

```bash
gcc -o program my-program.c
```

Execute
```
./program
```
