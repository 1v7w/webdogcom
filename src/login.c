#include "include/login.h"
#include "include/base64.h"

extern char base64_res[64];

const char *ret_msg[] = {"All ok.", "Password error or ip error or other error.", "Auth has been successful.", "Need to configure wlan_ac_name.", "Other error."};
const int MAX_HTTP_SIZE = 1024;
char result[32], ret_code[32], msg[128];

/*
 * 获取str中的key:"值"
 * @param str 需要查询的str
 * @param key 需要查询的key
 * @param str 结果保存的地方
 * @return 成功返回1，否则返回0
 */
int get_value(char *str, char *key, char *buf) {
    // 搜索 "result":" 字符串
    char *start = strstr(str, key);
    if (start != NULL) {
        // 移动指针到值的开始位置
        start += strlen(key);
        // 使用 sscanf 提取值，直到遇到双引号为止
        if (sscanf(start, "%[^\"]", buf) == 1) {
            printf("%s%s\"\n", key, buf);
            return 1;
        }
    }
    return 0;
}

/*
 * 登录认证
 * @param server 认证服务器地址
 * @param port 认证服务器端口号
 * @param user_account 校园网用户名
 * @param user_password 校园网密码
 * @param ip 主机校园网ip
 * @param wlan_ac_name 连接的AC名字,可NULL
 * @return 成功认证返回OK, 密码错误返回PASSWORD_ERROR, 已经认证过返回INUSE, 其他错误返回ERROR
 * */
enum RET_CODE login(const char *server, const char *port, const char *user_account, const char *user_password, const char *ip, const char *wlan_ac_name) {
    // 构建URL
    // 将time_t统一转为%lld
    long long int time_0 = time(NULL);
    long long int time_1 = time(NULL);
    char request[MAX_HTTP_SIZE];
    snprintf(request, sizeof(request),
             "GET /eportal/?c=Portal&a=login&callback=dr%lld&login_method=1&user_account=%s&user_password=%s&wlan_user_ip=%s&wlan_user_mac=000000000000&wlan_ac_ip=&wlan_ac_name=%s&jsVersion=3.0&_=%lld HTTP/1.1\r\n"
                       "Host: %s:%s\r\n"
                       "Connection: keep-alive\r\n"
                       "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36 Edg/89.0.774.57\r\n"
                       "Accept: */*\r\n"
                       "Referer: http://%s/\r\n"
                       "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n\r\n",
             time_1, user_account, user_password, ip, wlan_ac_name, time_0, server, port, server);

    // 创建套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return ERROR;
    }

    // 解析主机名
    struct hostent *server_info = gethostbyname(server);
    if (server_info == NULL) {
        perror("gethostbyname");
        close(sockfd);
        return ERROR;
    }

    // 设置服务器地址结构
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server_info->h_addr, server_info->h_length);
    server_addr.sin_port = htons(atoi(port));

    // 连接到服务器
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return ERROR;
    }

    // 发送HTTP请求
    if (write(sockfd, request, strlen(request)) == -1) {
        perror("write");
        close(sockfd);
        return ERROR;
    }

    // 接收并打印服务器响应
    char response[MAX_HTTP_SIZE];
    ssize_t bytes_received;
    if ((bytes_received = read(sockfd, response, sizeof(response) - 1)) > 0) {
        response[bytes_received] = '\0';
        printf("%s", response);
    }

    // 关闭套接字
    close(sockfd);

    get_value(response, "\"result\":\"", result);
    if(!strcasecmp(result, "1") || !strcasecmp(result, "ok")) {
        return OK;
    }

    get_value(response, "\"ret_code\":\"", ret_code);
    get_value(response, "\"msg\":\"", msg);
    base64_decode(msg);
    printf("msg: %s\n", base64_res);
    if(!strcasecmp(ret_code, "1")) {
        return PASSWORD_ERROR;
    }
    if(!strcasecmp(ret_code, "2")) {
        return INUSE;
    }
    if(!strcasecmp(ret_code, "8")) {
        return NO_AC_NAME;
    }

    return ERROR;
}

/*
 * 返回RET_CODE对应的说明
 * @param ret_code
 * @return
 */
const char *convert_ret_code(enum RET_CODE retcode) {
    if(retcode == 0) {
        return ret_msg[retcode];
    }
    snprintf(msg, sizeof(msg), "%s(ret=%s, msg=%s)", ret_msg[retcode], ret_code, base64_res);
    return msg;
}