# Modul 2
Proses dan Daemon

## Process

### Process
TL; DR.

Program yang sedang berjalan disebut dengan proses. 

Jika kamu mempunyai dua program yang tampil pada monitor, berarti terdapat dua proses yang sedang berjalan. Proses yang dijalankan pada terminal (shell) akan menghentikan proses shell, kemudian setelah proses selesai, shell akan dijalankan kembali.

Jalankan: `$ xlogo`

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
```

