#ifndef WEBDOGCOM_LOGIN_H
#define WEBDOGCOM_LOGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

int login(const char *server, const char *port, const char *user_account, const char *user_password, const char *ip, const char *wlan_ac_name);



#endif //WEBDOGCOM_LOGIN_H
