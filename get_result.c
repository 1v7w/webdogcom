#include "get_result.h"

char *get_result(char *str) {
    // 搜索 "result":" 字符串
    char *start = strstr(str, "\"result\":\"");
    if (start != NULL) {
        // 移动指针到值的开始位置
        start += strlen("\"result\":\"");
        char *result = (char *) malloc(32); // 假设result最多包含32个字符
        // 使用 sscanf 提取值，直到遇到双引号为止
        if (sscanf(start, "%31[^\"]", result) == 1) {
            printf("result: %s\n", result);
            return result;
        }
    }

    return NULL;
}

char *get_ret_code(char *str) {
    // 搜索 "ret_code":" 字符串
    char *start = strstr(str, "\"ret_code\":\"");
    if (start != NULL) {
        // 移动指针到值的开始位置
        start += strlen("\"ret_code\":\"");

        char *ret_code = (char *) malloc(32); // 假设result最多包含32个字符
        // 使用 sscanf 提取值，直到遇到双引号为止
        if (sscanf(start, "%31[^\"]", ret_code) == 1) {
            printf("ret_code: %s\n", ret_code);
            return ret_code;
        }
    }
    return NULL;
}
