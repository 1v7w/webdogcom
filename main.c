#include "school_ip.h"

int main(int argc, char *argv[]) {
    char msg[128];
    if(argc != 3) {
        sprintf(msg, "usage: %s network_segment subnet_mask\nsample: %s 192.168.1.0 255.255.255.0", argv[0], argv[0]);
        puts(msg);
        exit(1);
    }
    char *inner_ip = get_school_ip(argv[1], argv[2]);
    if(inner_ip == NULL) {
        sprintf(msg, "network_segment:%s\nsubnet_mask:%s\ncannot find target ip!", argv[1], argv[2]);
        puts(msg);
        exit(1);
    }
    printf("ip: %s\n", inner_ip);
    return 0;
}