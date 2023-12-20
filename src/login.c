#include "include/login.h"
#include "include/base64.h"
#include "include/logging.h"

extern char base64_res[64];

const char *ret_msg[] = {"All ok.", "Password error or ip error or other error.", "Auth has been successful.", "Need to configure wlan_ac_name.", "Other error."};

char result[32], ret_code[32], msg[128];
char request[MAX_HTTP_SIZE];
char response[MAX_HTTP_SIZE];

/*
 * 获取str中的key:"值"
 * @param str 需要查询的str
 * @param key 需要查询的key
 * @param str 结果保存的地方
 * @return 成功返回1，否则返回0
 */
int get_value(char *str, char *key, char *buf) {
    // 搜索 key 后面的字符串
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
enum RET_CODE login(const char *server, const char *port, const char *user_account, const char *user_password, const char *ip, const char *wlan_ac_name, int mode) {
    // 构建URL
    // 将time_t统一转为%lld
    long long int time_0 = time(NULL);
    long long int time_1 = time(NULL);
    char uri[64], data[256];

    // 查看是否已经登录
    puts("start get page");
    if(!curl(server, "80", "/", NULL, mode)) {
        sprintf(ret_code, "-1");
        sprintf(base64_res, "curl error");
        return ERROR;
    }
    puts("end get page");

    // 检测一登录标记
    /*
     * Dr.COMWebLoginID_0.htm 登陆页（未登陆）
     * Dr.COMWebLoginID_1.htm 注销页（已登录）
     * Dr.COMWebLoginID_2.htm 登陆失败页
     * Dr.COMWebLoginID_3.htm 登陆成功页
     */
    if(strstr(response, "Dr.COMWebLoginID_1.htm")) {
        sprintf(ret_code, "0");
        sprintf(base64_res, "in ues");
        return INUSE;
    }

    // 登录
    sprintf(uri, "/eportal/");
    sprintf(data, "c=Portal&a=login&callback=dr%lld&login_method=1&user_account=%s&user_password=%s&wlan_user_ip=%s&wlan_user_mac=000000000000&wlan_ac_ip=&wlan_ac_name=%s&jsVersion=3.0&_=%lld",
            time_1, user_account, user_password, ip, wlan_ac_name, time_0);

    if(!curl(server, port, uri, data, 0)) {
        sprintf(ret_code, "-1");
        sprintf(base64_res, "curl error");
        return ERROR;
    }
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

/*
 * 向目标主机请求http信息
 * @param server 目标主机
 * @param port 目标主机端口
 * @param uri 以/开头
 * @param data GET和POST的param
 * @param method 请求方式 0为GET 1为POST
 * @return 请求成功返回1 数据存储在response中 失败返回0
 */
int curl(const char *server, const char *port, const char *uri, const char *data, int method) {
    if(uri == NULL) {
        uri = "";
    }
    if(data == NULL) {
        data = "";
    }
    if (method == 0) {
        snprintf(request, sizeof(request),
                 "GET %s?%s HTTP/1.1\r\n"
                 "Host: %s:%s\r\n"
                 "Connection: keep-alive\r\n"
                 "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36 Edg/89.0.774.57\r\n"
                 "Accept: */*\r\n"
                 "Referer: http://%s/\r\n"
                 "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n\r\n",
                 uri, data, server, port, server);
    }
    else {
        long long int content_size = strlen(data);
        snprintf(request, sizeof(request),
                 "POST %s HTTP/1.1\r\n"
                 "Host: %s:%s\r\n"
                 "Connection: keep-alive\r\n"
                 "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.90 Safari/537.36 Edg/89.0.774.57\r\n"
                 "Accept: */*\r\n"
                 "Referer: http://%s/\r\n"
                 "Content-Length: %lld\r\n"
                 "Content-Type: application/x-www-form-urlencoded\r\n"
                 "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6\r\n\r\n"
                 "%s\r\n",
                 uri, server, port, server, content_size, data);
    }
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

    ssize_t bytes_received;
    size_t content_length = 0; // 声明响应消息体长度变量

    while ((bytes_received = read(sockfd, response, sizeof(response) - 1)) > 0) {
        response[bytes_received] = '\0';
        printf("%s", response);
        char *content_length_start = response;
        // 在响应头中查找Content-Length字段
        if (content_length == 0 && content_length_start == response ) {
            content_length_start = strstr(response, "Content-length:");
            printf("\n\ncontent_length_start: %ld\n\n", (long int)content_length_start);
            if (content_length_start != NULL) {
                content_length_start += strlen("Content-Length:");
                printf("开始转换 %c\n", *content_length_start);
                content_length = strtoul(content_length_start, NULL, 10);
                printf("经过转换：%ld\n", content_length);
            }
        }

        puts("判断content length");
        printf("cl = %ld, cls=%ld\n", content_length, (long int)content_length_start);
        // 判断是否已经读取了整个响应体
        if (content_length > 0) {
            // 已读取的字节数
            size_t bytes_read = strlen( content_length_start);
            printf("已经读取到了content_length = %ld, bttes_read = %ld\n", content_length, bytes_read);
            if (bytes_read >= content_length) {
                printf("Received complete response body.\n");
                break; // 已读取整个响应体，退出循环
            }
            content_length -= bytes_read;
        }
        if(content_length == 0 && content_length_start == NULL) {
            break;
        }
    }

    // 关闭套接字
    close(sockfd);
    return 1;
}