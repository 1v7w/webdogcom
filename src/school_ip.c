/*
 * 检查是否为目标网段的ip
 * @param ip_str 要检查的IP地址
 * @param subnet_str 网段
 * @param netmask_str 子网掩码
 * @return 返回1表示ip属于该网段
 * */

#include "include/school_ip.h"

int check_network(char ip_str[], char subnet_str[], char netmask_str[]) {
    // 将IP地址和子网掩码转换为网络字节序
    struct in_addr ip, subnet, netmask;
    inet_pton(AF_INET, ip_str, &ip);
    inet_pton(AF_INET, subnet_str, &subnet);
    inet_pton(AF_INET, netmask_str, &netmask);

    // 计算IP地址的网络地址
    struct in_addr network;
    network.s_addr = ip.s_addr & netmask.s_addr;

    // 检查IP地址是否属于特定的网段
    if (network.s_addr == (subnet.s_addr & netmask.s_addr)) {
        return 1;
    }
    return 0;
}

/*
 * 获取校园网分配的内网ip
 * @param subnet_str 校园网的网段
 * @param netmask_str 校园网的子网掩码
 * @return 成功返回16位char数组表示ip地址，失败返回NULL
 * */
char *get_school_ip(char subnet_str[], char netmask_str[]) {
    int sockfd;
    struct ifreq *ifreq;
    struct ifconf ifconf;
    char buf[1024];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return NULL;
    }

    // 设置ifconf结构体以获取网络接口信息
    ifconf.ifc_len = sizeof(buf);
    ifconf.ifc_buf = buf;

    // 使用ioctl系统调用获取网络接口信息并存储在ifconf中
    if (ioctl(sockfd, SIOCGIFCONF, &ifconf) == -1) {
        perror("ioctl");
        close(sockfd);
        return NULL;
    }

    // 将ifreq指针指向ifconf中的数据
    ifreq = (struct ifreq *)ifconf.ifc_req;

    // 遍历网络接口信息
    for (int i = 0; i < ifconf.ifc_len / sizeof(struct ifreq); i++) {
        // 从ifreq结构中提取IPv4地址
        struct sockaddr_in *addr = (struct sockaddr_in *)&ifreq[i].ifr_addr;
        char ip_str[INET_ADDRSTRLEN];

        // 检查地址族是否为IPv4
        if (addr->sin_family == AF_INET) {
            // 将IPv4地址转换为可读字符串形式
            inet_ntop(AF_INET, &addr->sin_addr, ip_str, sizeof(ip_str));
//            printf("%s: %s: ", ifreq[i].ifr_name, ip_str);
            if(check_network(ip_str, subnet_str, netmask_str)) {
                char *res = (char *)malloc(16);
                memcpy(res, ip_str, 16);
                close(sockfd);
                return res;
            }
        }
    }
    close(sockfd);
    return NULL;
}