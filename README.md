```
 ##     ##  #######  ########  ##     ## ##           #######  
 ###   ### ##     ## ##     ## ##     ## ##          ##     ## 
 #### #### ##     ## ##     ## ##     ## ##                 ## 
 ## ### ## ##     ## ##     ## ##     ## ##           #######  
 ##     ## ##     ## ##     ## ##     ## ##          ##        
 ##     ## ##     ## ##     ## ##     ## ##          ##        
 ##     ##  #######  ########   #######  ########    ######### 

      +-++-++-++-++-++-+ +-++-++-+ +-++-++-++-++-++-+
      |P||r||o||s||e||s| |d||a||n| |D||a||e||m||o||n|
      +-++-++-++-++-++-+ +-++-++-+ +-++-++-++-++-++-+

```

Proses dan Daemon

##### PraModul 2

Requirement:
1. Linux
2. gcc / g++

Tutorial compiling C code: [here](https://github.com/syukronrm/sisop-mod-2/blob/master/compiling-c-program.md)

## 1 Process

Objectives:
1. Peserta mengetahui proses
2. Peserta memahami bagaimana proses bekerja
3. Peserta memahami bagaimana cara membuat proses
4. Peserta mengerti jenis-jenis proses
5. Peserta bisa memahami proses
6. Peserta mampu membuat proses daemon

### 1.1 Process
TL; DR.

Setiap program yang sedang berjalan disebut dengan proses. Proses dapat berjalan secara _foreground_ atau _background_. 

Jalankan `$ ps -e` untuk melihat semua proses yang sedang berjalan.

### 1.2 Process ID
Proses-proses dapat diibaratkan seperti orang tua (_parent_) dengan anak (_child_) yang turun temurun.
- Setiap proses memiliki parent dan child.
- Setiap proses memiliki ID (_pid_) dan parent ID (_ppid_), kecuali proses `init` atau `systemd`.
- _ppid_ dari sebuah proses adalah ID dari parent proses tersebut. Perhatikan _ascii art_ dibawah:
```
 [Parent]           [Child]
+--------+        +--------+ 
| pid=7  |        | pid=10 |
| ppid=4 |------->| ppid=7 |
| bash   |        | nano   |  
+--------+        +--------+
```
Perhatikan, ppid dari proses `nano` adalah pid dari proses `bash`.

Untuk memvisualisasikan semua hierarki parent-child, jalankan
```
$ pstree | less
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
  - [pstree](http://man7.org/linux/man-pages/man1/pstree.1.html): pstree berfungsi untuk menampilkan suatu proses dalam bentuk tree. Tree dari proses yang ditampilkan memiliki pid atau init (jika pid dihilangkan) sebagai root. 

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
__Penjelasan__
  - getpid(): digunakan untuk memunculkan *process ID* dari proses yang dipanggil.
  - getppid(): digunakan untuk memunculkan *process ID* dari **parent** proses yang dipanggil


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
25793  9789 ps -e -o pid,ppid,command
```

__Penjelasan__
  - ps  : menampilkan rincian dari proses yang sedang berjalan.
  - -e  : memilih seluruh proses yang sedang berjalan.
  - -o  : format yang ditentukan user.


### 1.4 Membunuh Proses
Membunuh proses menggunakan `$ kill {pid}`

Contoh: 
```
$ kill 3789
```
terdapat beberapa macam signal yang digunakan dalam command kill, antara lain sebagi berikut :

| Signal name | Signal value  | Effect       |
| ------------|:-------------:| -------------|
| SIGHUP      | 1             | Hangup         |
| SIGINT      | 2             | Interrupt from keyboard  |
| SIGKILL     | 9             | Kill signal   |
| SIGTERM     | 15            | Termination signal
| SIGSTOP     | 17,19,23      | Stop the process

Secara default, `$ kill` menggunakan signal SIGTERM. Untuk menggunakan signal tertentu, gunakan `$ kill -{signal value} {pid}`. Contoh, `$ kill -9 3789` untuk menggunakan SIGKILL.

### 1.5 Membuat Proses
Proses dapat dibuat menggunakan dua cara (pada C), yaitu dengan `system()` atau `fork` & `exec`. `fork` dan `exec` adalah bagian dari [system call](http://man7.org/linux/man-pages/man2/syscalls.2.html), sedangkan `system` bukan.

#### 1.5.1 Menggunakan `fork` dan `exec`
   
TL;DR.  
`fork` digunakan untuk menduplikasi program yang sedang berjalan.  
`exec` digunakan untuk mengganti program yang sedang berjalan dengan program yang baru.

#### A. `fork`

`fork` digunakan untuk menduplikasi proses. Proses yang baru disebut dengan _child_ proses, sedangkan proses pemanggil disebut dengan _parent_ proses. Spesifikasi `fork` bisa dilihat dengan `$ man 2 fork` atau di [sini](http://man7.org/linux/man-pages/man2/fork.2.html).

Setelah `fork` dipanggil, kita tidaklah tahu proses manakah yang pertama selesai.

```
$ man 2 fork
....
RETURN VALUE
       On success, the PID of the child process is returned in the parent,
       and 0 is returned in the child.  On failure, -1 is returned in the
       parent, no child process is created, and errno is set appropriately.
....
```

```C
int main() { 
                            pid: 23, ppid: 10 
                             [Main process]
                                 |
  fork();              > Child process created <
                                 +
                               /   \
                             /       \
               pid: 23, ppid: 10    pid: 30, ppid: 23
                [Parent Process]    [Child Process]

  return 0;
}
```
Perhatikan, ppid child proses sama dengan pid parent process.

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

#### B. `exec`
Exec adalah function yang digunakan untuk menjalankan program baru dan mengganti program yang sedang berlangsung. `exec` adalah program family yang memiliki berbagai fungsi variasi, yaitu `execvp`, `execlp`, `execv`, dan lain lain.

Manual: `$ man 3 exec`

Example: [exec-sample.c](https://github.com/syukronrm/sisop-mod-2/blob/master/sample-exec.c)
```C
#include <stdio.h>
#include <unistd.h>

int main () {
  
  // argv[n] = { {your-program-name}, {argument[1]}, {argument[2]},.....,{argument[n-2]}, NULL }
  char *argv[4] = {"list", "-l", "/", NULL};
  
  execv("/bin/ls", argv);

  printf("This line will not be executed\n");

  return 0;
}

```

#### C. `fork` and `exec`
__Permasalahan:__
Bagaimana cara menjalankan dua proses dalam satu program?

__Contoh Permasalahan:__
Bagaimana cara membuat folder `~/sisop` dan membuat file kosong bernama `~/process.c`?

Maka, bagaimana cara menjalankan `mkdir` __dan__ `touch` dalam satu program?

__Solusi:__
Gunakan `fork` dan `exec`!

TL;DR.  
Buat sebuah program dengan:  
1. Buat proses baru dengan `fork`
2. Jalankan `exec` yang memanggil `mkdir` pada child process
3. Jalankan `exec` yang memanggil `touch` pada parent process

Visualisasi
```
+--------+
| pid=7  |
| ppid=4 |
| bash   |
+--------+
    |
    | calls fork
    V                         
+--------+                     +--------+
| pid=7  |    forks            | pid=22 |
| ppid=4 | ------------------> | ppid=7 |
| bash   |                     | bash   |
+--------+                     +--------+
    |                              |
    | calls exec to run touch      | calls exec to run mkdir
    |                              |
    V                              V
```

Example: [sample-fork-exec.c](https://github.com/syukronrm/sisop-mod-2/blob/master/sample-fork-exec.c)

#### D. `wait`

`wait` adalah function yang digunakan untuk mendapatkan informasi ketika child proses berganti _state_-nya. Pergantian state dapat berupa _termination_, _resume_, atau _stop_. Pada modul ini, kita hanya menggunakan `wait` untuk menangkap state _termination_.

Fungsi `wait` pada parent process juga berguna untuk menangkap exit status dari child.

__Permasalahan:__  
Bagaimana cara membuat program yang menjalankan suatu proses tanpa menghentikan program?

__Contoh permasalahan:__  
Bagaimana cara membuat folder `~/sisop` dan membuat file kosong bernama `~/process.c` __di dalamnya__?

Maka, bagaimana cara menjalankan `mkdir` __lalu__ menjalankan `touch` dalam satu program?

__Solusi:__  
Gunakan `fork`, `exec`, dan `wait`!

Buat sebuah program dengan:  
1. Buat proses baru dengan `fork`
2. Jalankan `exec` yang memanggil `mkdir` pada child process
3. Buat parent process menunggu (`wait`) hingga proses pada child selesai
4. Setelah child selesai, jalankan `exec` yang memanggil `touch` pada parent

Visualisasi
```
+--------+
| pid=7  |
| ppid=4 |
| bash   |
+--------+
    |
    | calls fork
    V
+--------+             +--------+
| pid=7  |    forks    | pid=22 |
| ppid=4 | ----------> | ppid=7 |
| bash   |             | bash   |
+--------+             +--------+
    |                      |
    | waits for pid 22     | calls exec to run mkdir
    |                      V
    |                  +--------+
    |                  | pid=22 |
    |                  | ppid=7 |
    |                  | ls     |
    V                  +--------+
+--------+                 |
| pid=7  |                 | exits
| ppid=4 | <---------------+
| bash   |
+--------+
    |
    | calls exec to run touch
    |
    V
```

Example: [sample-fork-exec-wait.c](https://github.com/syukronrm/sisop-mod-2/blob/master/sample-fork-exec-wait.c)

#### 1.5.2 Menggunakan `system`
   
Ketika [system](http://man7.org/linux/man-pages/man3/system.3.html) dijalankan, program hanya memanggil _external command_ (kalau di Ubuntu berupa program `/bin/bash`). Penggunaan `system` sangat tergantung pada environment. 

Contoh, ketika user menjalankan `system("ls -l")`, ini sama seperti menjalankan `$ ls -l` pada bash.

Meskipun mudah digunakan, tidak disarankan menggunakan fungsi `system` karena beberapa asalan:

```
$ man system

  ....
  NOTES
  system() provides simplicity and convenience: it handles  all
  of  the details of calling fork(2), execl(3), and waitpid(2),
  as well as the necessary manipulations of signals.

  Do not use system() from a program with set-user-ID  or  set-
  group-ID privileges, because strange values for some environ‐
  ment variables might be used  to  subvert  system  integrity.
  ...
```

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

## 1.6 Jenis-Jenis Proses
### 1.6.1 Zombie Process

Zombie proses adalah child proses yang telah selesai mengerjakan tugasnya, namun belum ditangkap oleh parentnya dengan fungsi `wait`.

```
PID     PPID    STAT  COMMAND
28621   31403   S+    ./sample-zombie-process
28622   28621   Z+    [sample-zombie-p] <defunct>
```

Status dari zombie process adalah `Z` atau terdapat `<defunct>` di akhir commandnya.

Example: [sample-zombie-process.c](https://github.com/syukronrm/sisop-mod-2/blob/master/sample-zombie-process.c)

```C
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main () {
  pid_t child_pid;
  
  /* Create a child process. */
  child_pid = fork ();
  
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  
  if (child_pid == 0) {
    /* This is the child process. Exit immediately. */
    exit (0);
  } else {
    /* This is the parent process. Sleep for a minute. */
    sleep (60);
  }
  
  return 0;
}

```

### 1.6.2 Orphan Process
Orphan process adalah child process yang yang parent processnya telah berhenti.

```
PID    PPID     STAT  COMMAND
    1     0     Ss    /sbin/init
28369     1     S     ./sample-orphan-process
```

Example: [sample-orphan-process.c](https://github.com/syukronrm/sisop-mod-2/blob/master/sample-orphan-process.c)
```C
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main () {
  pid_t child_pid;
  
  /* Create a child process. */
  child_pid = fork ();
  
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  
  if (child_pid == 0) {
    /* This is the child process. Sleep for a minute. */
    sleep (60);
  } else {
    /* This is the parent process. Exit immediately. */
    exit (0);
  }
  
  return 0;
}

```

### 1.6.3 Daemon Process
Daemon process adalah proses yang berjalan di balik layar (background) dan tidak dapat berinteraksi dengan user melalui standard input/output. Akan dijelaskan di bab selanjutnya.

## 2. Daemon
### 2.1 Daemon
Daemon process adalah proses yang berjalan di balik layar (background) dan tidak dapat berinteraksi dengan user melalui standard input/output.

### 2.2 Cara Membuat Daemon
Daemon dapat dibuat dalam beberapa langkah:
1. Melakukan fork pada parent process dan menghentikan parent process
2. Mengubah mode file dengan `umask`
3. Mengubah unique session ID
4. Mengubah direktori kerja
5. Menutup file descriptor
6. Membuat loop utama program

### 2.3 Proses pembuatan daemon
#### 2.3.1 Melakukan fork pada parent process dan menghentikan parent process
Langkah pertama adalah melakukan forking untuk membuat process baru
kemudian mematikan Parent Process. Process induk yang mati akan membuat system mengira proses telah selesai sehingga akan kembali ke terminal user. Proses anak yang melanjutkan program setelah proses induk dimatikan.
```C
pid_t pid;
pid = fork();

if (pid < 0) {
  exit(EXIT_FAILURE);
}

if (pid > 0) {

  // simpan PID dari child proces

  exit(EXIT_SUCCESS);
}
```

#### 2.3.2 Mengubah mode file dengan `umask`
Untuk menulis beberapa file (termasuk logs) yang dibuat oleh daemon, mode file harus diubah untuk memastikan bahwa file tersebut dapat ditulis dan dibaca secara benar. Pengubahan mode file menggunakan implementasi umask().
```C
umask(0);
```

#### 2.3.3 Mengubah unique session ID
Child Process harus memiliki unik SID dari kernel untuk dapat beroperasi. Sebaliknya, Child process menjadi Orphan Proses pada system. Tipe pid_t yang dideklarasikan pada bagian sebelumnya, juga digunakan untuk membuat SID baru untuk child process. Pembuatan SID baru menggunakan implementasi setsid(). Fungsi setsid() memiliki return tipe yang sama seperti fork().

```C
sid = setsid();

if (sid < 0) {
  exit(EXIT_FAILURE);
}
```

#### 2.3.4 Mengubah direktori kerja
Directori kerja yang aktif harus diubah ke suatu tempat yang telah pasti akan selalu ada. Pengubahan tempat direktori kerja dapat dilakukan dengan implementasi fungsi `chdir`. Fungsi `chdir` mengembalikan nilai -1 jika gagal.

```C

if ((chdir("/)) < 0) {
  exit(EXIT_FAILURE);
}
```

#### 2.3.5 Menutup file descriptor
Salah satu dari langkah terakhir dalam mengeset daemon adalah menutup file descriptor standar (STDIN, STDOUT, STDERR). Karena daemon tidak perlu menggunakan kendali terminal, file descriptor dapat berulang dan berpotensi memiliki bahaya dalam hal keamanan. Untuk mengatasi hal tersebut maka digunakan implemtasi fungsi close().

```C
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
```

#### 2.3.6 Membuat loop utama program
Daemon bekerja terus menerus, sehingga diperlukan sebuah looping.

```C
while(1) {
  // daemon program
  sleep(30);
}

exit(EXIT_SUCCESS);
```


### 2.4 Contoh Implementasi Daemon
```C
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
    sleep(30);
  }
  
  exit(EXIT_SUCCESS);
}
```

# Appendix
### Libraries documentation (and functions) :sparkles: :sparkles: :camel:
```
# apt-get install manpages-posix-dev

$ man {anything-you-want-to-know}
$ man fopen
$ man fclose
$ man unistd.h
```

### References 
https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet  
https://notes.shichao.io/apue/  
http://advancedlinuxprogramming.com/alp-folder/alp-ch03-processes.pdf  
http://www.linuxzasve.com/preuzimanje/TLCL-09.12.pdf  
https://stackoverflow.com/questions/1653340/differences-between-fork-and-exec
