#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "include/school_ip.h"
#include "include/login.h"
#include "include/config.h"
#include "include/daemond.h"
#include "include/logging.h"

#define VERSION "0.0.4"

int interval_time = 60;

int is_positive_integer(const char *str);
void print_help(int exval);

int main(int argc, char *argv[]) {
    char msg[128];
    // 获取参数
    struct option long_options[] = {
            {"interval", required_argument, NULL, 'i'},
            {"daemon", no_argument, NULL, 'd'},
            {"help", no_argument, 0, 'h'},
            {NULL, 0, NULL, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "i:dh", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                if(!is_positive_integer(optarg)) {
                    printf("Interval must be a positive integer, using the default value of 1.\n");
                    logging("Interval must be a positive integer, using the default value of 1.");
                    break;
                }
                interval_time = atoi(optarg);
                break;
            case 'd':
                daemon_flag = 1;
                break;
            case 'h':
                print_help(0);
                break;
            case '?':
                print_help(1);
                break;
            default:
                break;
        }
    }
    // 设置daemon
    if(daemon_flag) {
        daemonise();
    }
    // 获取配置
    struct Config *config = get_config();
    if(config == NULL) {
        fprintf(stderr, "read config file failed.");
        logging("read config file failed.");
        exit(1);
    }
    char *inner_ip = get_school_ip(config->network_segment, config->subnet_mask);
    if(inner_ip == NULL) {
        fprintf(stderr, "network_segment:%s\n"
                        "subnet_mask:%s\n"
                        "cannot find target ip!\n",
                config->network_segment, config->subnet_mask);
        exit(1);
    }

    sprintf(msg, "ip: %s", inner_ip);
    puts(msg);
    logging(msg);

    while(1) {
        int ret = login(config->server, config->port, config->user_account, config->user_password,
                        inner_ip, config->wlan_ac_name);
        sprintf(msg, "logging ret: %s", convert_ret_code(ret));
        puts(msg);
        logging(msg);

        sleep(interval_time);
    }
    return 0;
}

/*
 * 判断str是否为正整数
 * @param str 要判断的字符串
 * @return 为正整数返回1，否则返回0
 */
int is_positive_integer(const char *str) {
    char *endptr;
    long num = strtol(str, &endptr, 10);

    // 检查转换是否成功，并且字符串中没有多余的字符
    if (*endptr != '\0' || endptr == str || num <= 0) {
        return 0;
    }

    return 1;
}

void print_help(int exval) {
    printf("\nImplementation of Dr.com in web authentication.\n");
    printf("Version: %s\n\n", VERSION);

    printf("Usage:\n");
    printf("\twebdogcom [options <argument>]...\n\n");

    printf("Options:\n");
    printf("\t--interval <m>, -i <m>                authentication per m(int) seconds\n");
    printf("\t--daemon, -d                          set daemon flag\n");
    printf("\t--help, -h                            display this help\n\n");
    exit(exval);
}