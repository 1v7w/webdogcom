#include "school_ip.h"
#include "login.h"
#include "config.h"

int main(int argc, char *argv[]) {
    struct Config *config = get_config();
    char *inner_ip = get_school_ip(config->network_segment, config->subnet_mask);
    if(inner_ip == NULL) {
        fprintf(stderr, "network_segment:%s\n"
                        "subnet_mask:%s\n"
                        "cannot find target ip!\n",
                        config->network_segment, config->subnet_mask);
        exit(1);
    }
    printf("ip: %s\n", inner_ip);

    int ret = login(config->server, config->port, config->user_account, config->user_password,
                    inner_ip, config->wlan_ac_name);
    printf("ret: %s\n", convert_ret_code(ret));
    return 0;
}