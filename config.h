#ifndef WEBDOGCOM_CONFIG_H
#define WEBDOGCOM_CONFIG_H

#include <stdio.h>
#include <stdlib.h>

struct Config {
    // 定义变量来存储IP地址和用户名
    char server[16];            // 最多15字符的IP地址 + 终止符
    char port[6];               // 最多5位端口号 + 终止符
    char network_segment[16];   // 最多15字符的网段 + 终止符
    char subnet_mask[16];       // 最多15字符的子网掩码 + 终止符
    char user_account[21];      // 最多20个字符的用户名 + 终止符
    char user_password[21];     // 最多20位密码 + 终止符
    char wlan_ac_name[32];      // 最多31位ac名字 + 终止符
};

struct Config *get_config();

#endif //WEBDOGCOM_CONFIG_H
