#ifndef WEBDOGCOM_SCHOOL_IP_H
#define WEBDOGCOM_SCHOOL_IP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int check_network(char ip_str[], char subnet_str[], char netmask_str[]);
char *get_school_ip(char subnet_str[], char netmask_str[]);

#endif //WEBDOGCOM_SCHOOL_IP_H
