// copy from dogcom https://github.com/mchome/dogcom

#include "daemond.h"
#include "logging.h"

int daemon_flag = 0;
int pid_file_handle;

void kill_daemon() {
    close(pid_file_handle);
    remove("/tmp/webdogcom.pid");
}

void signal_handler(int signal) {
    switch (signal) {
        case SIGHUP:
            break;
        case SIGINT:
            break;
        case SIGTERM:
            kill_daemon();
            exit(0);
            break;
        default:
            break;
    }
}

void daemonise() {
    pid_t pid;
    struct sigaction sig_action;
    sigset_t sigset;
    char msg[128];

    pid = fork();
    if (pid < 0) {
        printf("Fork failed!\n");
        logging("Fork failed!");
        exit(1);
    } else if (pid > 0) {
        sprintf(msg, "PID is %d.", pid);
        puts(msg);
        logging(msg);
        exit(0);
    }
    if (setsid() < 0) {
        printf("Setsid failed!\n");
        logging("Setsid failed!");
        exit(1);
    }

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGCHLD);
    sigaddset(&sigset, SIGTSTP);
    sigaddset(&sigset, SIGTTOU);
    sigaddset(&sigset, SIGTTIN);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
    sig_action.sa_handler = signal_handler;
    sigemptyset(&sig_action.sa_mask);
    sig_action.sa_flags = 0;
    sigaction(SIGHUP, &sig_action, NULL);
    sigaction(SIGTERM, &sig_action, NULL);
    sigaction(SIGINT, &sig_action, NULL);

    pid = fork();
    if (pid < 0) {
        printf("Fork failed!\n");
        logging("Fork failed!");
        exit(1);
    } else if (pid > 0) {
        sprintf(msg, "PID is %d.", pid);
        puts(msg);
        logging(msg);
        exit(0);
    }

    chdir("/tmp/");
    umask(027);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_WRONLY);
    open("/dev/null", O_RDWR);

    pid_file_handle = open("/tmp/webdogcom.pid", O_RDWR | O_CREAT, 0600);
    if (pid_file_handle < 0) {
        exit(1);
    }
    if (lockf(pid_file_handle, F_TLOCK, 0) < 0) {
        exit(1);
    }

    char spid[10];
    sprintf(spid, "%d\n", getpid());
    write(pid_file_handle, spid, strlen(spid));
    sprintf(msg, "final PID is %d.", getpid());
    logging(msg);
}