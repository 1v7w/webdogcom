#ifndef WEBDOGCOM_LOGIN_H
#define WEBDOGCOM_LOGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <time.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>

#define MAX_HTTP_SIZE 4096
#define SOCKET_TIMEOUT_SECONDS 10

/*
 * OK认证成功
 * PASSWORD_ERROR密码错误or内网ip错误or其他错误
 * INUSE已经认证过
 * NO_AC_NAME未指定wlan_ac_name
 * ERROR其他错误
 */
enum RET_CODE{OK, PASSWORD_ERROR, INUSE, NO_AC_NAME, ERROR};

enum RET_CODE login(const char *server, const char *port, const char *user_account, const char *user_password, const char *ip, const char *wlan_ac_name, int mode);
const char *convert_ret_code(enum RET_CODE ret_code);
int curl(const char *server, const char *port, const char *uri, const char *data, int method);

#endif //WEBDOGCOM_LOGIN_H
