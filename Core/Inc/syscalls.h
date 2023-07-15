#ifndef __syscalls_h
#define __syscalls_h
#ifdef __cplusplus
extern "C" {
#endif

#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>

int _chown (const char *path, uid_t owner, gid_t group);
int _close (int fildes);
int _execve (char *name, char **argv, char **env);
int _fork (void);
int _fstat (int fildes, struct stat *st);
int _getpid (void);
int _gettimeofday (struct timeval *ptimeval, void *ptimezone);
int _isatty (int file);
int _kill (int pid, int sig);
int _link (char *existing, char *_new);
int _lseek (int file, int ptr, int dir);
int _open (char *file, int flags, int mode);
int _read (int file, char *ptr, int len);
int _readlink (const char *path, char *buf, size_t bufsize);
int _stat (const char *file, struct stat *st);
int _symlink (const char *path1, const char *path2);
clock_t _times (struct tms *buf);
int _unlink (char *name);
int _wait (int *status);
int _write (int file, char *ptr, int len);

#ifdef __cplusplus
}
#endif
#endif