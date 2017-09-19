# Modul 2
Proses dan Daemon

## Process

### Process
TL; DR.

Program yang sedang berjalan disebut dengan proses. 

Jika kamu mempunyai dua program yang tampil pada monitor, berarti terdapat dua proses yang sedang berjalan. Proses yang dijalankan pada terminal (shell) akan menghentikan proses shell, kemudian setelah proses selesai, shell akan dijalankan kembali.

Jalankan:
```
$ xlogo
```

### Process ID
Setiap proses memiliki dikenali dengan _Process ID_/_pid_. Setiap proses memiliki _Parent Process ID_/_ppid_, kecuali proses `init` atau `systemd`. 

Jalankan: 
```
$ pstree
```
```
systemd-+-ModemManager-+-{gdbus}
        |              `-{gmain}
        |-NetworkManager-+-dhclient
        |                |-dnsmasq
        |                |-{gdbus}
        |                `-{gmain}
        |-accounts-daemon-+-{gdbus}
        |                 `-{gmain}
        |-acpid
        |-agetty
```

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

### Melihat Proses
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
 long list
 .......
 3760  2684 /usr/lib/x86_64-linux-gnu/notify-osd
 3789  2684 /opt/google/chrome/chrome
```

### Membunuh Proses
Membunuh proses menggunakan `$ kill {pid}`

Contoh: 
```
$ kill 3789
```

### Membuat Proses
Proses dapat dibuat menggunakan dua cara (pada C), yaitu dengan `system()` atau `fork()` & `exec()`

1. Menggunakan `system()`
   Ketika `system()` dijakankan, ia akan memanggil standard shell (`/bin/bash`) dan menjalankan perintah yang diminta.

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

2. Menggunakan `fork()` dan `exec()`


### Useful things
#### Get all libraries documentation (and functions)
`apt-get install manpages-posix-dev`

`man popen`
