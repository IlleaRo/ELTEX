#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void main()
{
    printf("Information about my proccess:\n");
    pid_t curPID = getpid();
    printf("PID: %d\n", curPID);
    printf("PPID: %d\n", getppid());
    printf("UID: %d\n", getuid());
    printf("EUID: %d\n", geteuid());
    printf("GID: %d\n", getgid());
    printf("SID: %d\n", getsid(curPID));
}