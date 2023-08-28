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

#include "get_result.h"

/*
 * OK认证成功
 * PASSWORD_ERROR密码错误or其他错误
 * INUSE已经认证过
 * ERROR其他错误
 */
enum RET_CODE{OK, PASSWORD_ERROR, INUSE, ERROR};

enum RET_CODE login(const char *server, const char *port, const char *user_account, const char *user_password, const char *ip, const char *wlan_ac_name);
const char *convert_ret_code(enum RET_CODE ret_code);

#endif //WEBDOGCOM_LOGIN_H
