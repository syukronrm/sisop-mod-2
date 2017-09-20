https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet

http://advancedlinuxprogramming.com/alp-folder/alp-ch03-processes.pdf

http://www.linuxzasve.com/preuzimanje/TLCL-09.12.pdf

# Modul 2
Proses dan Daemon

## 1 Process

### 1.1 Process
TL; DR.

Program yang sedang berjalan disebut dengan proses. 

Jika kamu mempunyai dua program yang tampil pada monitor, berarti terdapat dua proses yang sedang berjalan. Proses yang dijalankan pada terminal (shell) akan menghentikan proses shell, kemudian setelah proses selesai, shell akan dijalankan kembali.

Jalankan:
```
$ xlogo
```

### 1.2 Process ID
Setiap proses memiliki dikenali dengan _Process ID_/_pid_. Setiap proses memiliki _Parent Process ID_/_ppid_, kecuali proses `init` atau `systemd`. 

Jalankan: 
```
$ pstree
```
```
init-+-acpid
     |-apache2---7*[apache2]
     |-atd
     |-cron
     |-dbus-daemon
     |-dockerd-+-docker-containe---10*[{docker-containe}]
     |         `-10*[{dockerd}]
     |-fail2ban-server---2*[{fail2ban-server}]
     |-freshclam
```

__Explanation__
  - [pstree](https://linux.die.net/man/1/pstree): ...

Contoh program `demo-process.c`
```C
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("The process ID is %d\n", (int) getpid());
  printf("The parent process ID is %d\n", (int) getppid());
  return 0;
}
```
Hasil
```
The process ID is 8295
The parent process ID is 29043
```
__Explanation__
  - getpid(): ...
  - getppid(): ...


### 1.3 Melihat Proses
Jalankan program

```bash
$ ps -e -o pid,ppid,command
```
```
  PID  PPID COMMAND
    1     0 /sbin/init splash
    2     0 [kthreadd]
    6     2 [ksoftirqd/0]
    7     2 [rcu_sched]
    8     2 [rcu_bh]
    9     2 [migration/0]
   10     2 [lru-add-drain]
   11     2 [watchdog/0]
 .......
 (long list)
 .......
 3760  2684 /usr/lib/x86_64-linux-gnu/notify-osd
 3789  2684 /opt/google/chrome/chrome
```

__Explanation__
  - ps: ...
  - -e: ...
  - -o: ...


### 1.4 Membunuh Proses
Membunuh proses menggunakan `$ kill {pid}`

Contoh: 
```
$ kill 3789
```

### 1.5 Membuat Proses
Proses dapat dibuat menggunakan dua cara (pada C), yaitu dengan `system()` atau `fork()` & `exec()`

#### 1.5.1 Menggunakan `system()`
   
Ketika `system()` dijalankan, ia akan memanggil standard shell (`/bin/bash`) dan menjalankan perintah yang diminta.

Contoh

```C
#include <stdlib.h>

int main() {
  int return_value;
  return_value = system("ls -l /");
  return return_value;
}
```
   
Hasil
   
```
total 156
drwxr-xr-x   2 root root  4096 Sep 14 06:35 bin
drwxr-xr-x   4 root root  4096 Sep 20 00:24 boot
drwxrwxr-x   2 root root  4096 Agu 14 14:05 cdrom
drwxr-xr-x   3 root root  4096 Sep 12 19:11 data
(long list)
```

#### 1.5.2 Menggunakan `fork()` dan `exec()`
   
TL;DR.  
`fork()` digunakan untuk menduplikasi program yang sedang berjalan.  
`exec()` digunakan untuk mengganti program yang sedang berjalan dengan program yang baru.  

#### A. `fork()` explained

Ketika `fork()` dijalankan, proses baru yang disebut _child process_ akan dibuat. _Parent process_ tetap berjalan dan _child process_ mulai dibuat dan berjalan ketika function `fork()` dipanggil.

```C
int main() { 
                            pid: 23, ppid: 10 
                             [Main process]
                                 |
  fork();              > Child process created <
                                 /\
                               /    \
                             /        \
               pid: 23, ppid: 10    pid: 30, ppid: 23
                [Parent Process]    [Child Process]

  return 0;
}
```

Real code:
```C
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_pid;

  printf("The main program PID is %d\n\n", (int) getpid());

  child_pid = fork();
  if (child_pid != 0) {
    printf("This is the parent process, with PID %d\n", (int) getpid());
    printf("The child's PID is %d\n", (int) child_pid);
  } else {
    printf("This is the child process, with PID %d\n", (int) getpid());
  }

  return 0;
}
```
__Explanation__
  - 

Lengkapi guys

#### B. `exec()` explained

Diisi ya penjelasannya  
Halaman 6 http://advancedlinuxprogramming.com/alp-folder/alp-ch03-processes.pdf

#### C. `fork()` and `exec()` explained!
__Permasalahan:__  
Bagaimana cara membuat program yang menjalankan suatu proses tanpa menghentikan program?

__Contoh permasalahan:__  
Bagaimana cara menjalankan `ls -l /`, lalu menjalankan `mkdir ~/sisop` dalam satu program?

__Solusi:__  
Gunakan `fork()` dan `exec()`!

Diisi ya penjelasannya  
Halaman 6 http://advancedlinuxprogramming.com/alp-folder/alp-ch03-processes.pdf

# Appendix
### Useful things
#### Get all libraries documentation (and functions)
```
apt-get install manpages-posix-dev

man {anything-you-want-to-know}
man fopen
man fclose
man unistd.h
```
