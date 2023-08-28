#ifndef WEBDOGCOM_DAEMOND_H
#define WEBDOGCOM_DAEMOND_H

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void kill_daemon();
void signal_handler(int signal);
void daemonise();

extern int daemon_flag;
extern int pid_file_handle;

#endif //WEBDOGCOM_DAEMOND_H
