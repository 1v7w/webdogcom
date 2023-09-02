#include "include/base64.h"

char base64_res[64];

// Base64解码表
static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// 查找字符在Base64解码表中的位置
static int base64_index(char c) {
    const char *ptr = strchr(base64_chars, c);
    if (ptr != NULL) {
        return ptr - base64_chars;
    } else {
        return -1; // 未找到字符
    }
}

// Base64解码函数
int base64_decode(const char *input) {
    int input_len = strlen(input);
    int output_len = input_len / 4 * 3;
    if (input_len % 4 != 0) {
        return -1; // 长度不是4的倍数，不是有效的Base64编码
    }

    int i, j;
    uint8_t a, b, c, d;

    for (i = 0, j = 0; i < input_len; i += 4, j += 3) {
        a = (input[i] != '=') ? base64_index(input[i]) : 0;
        b = (input[i + 1] != '=') ? base64_index(input[i + 1]) : 0;
        c = (input[i + 2] != '=') ? base64_index(input[i + 2]) : 0;
        d = (input[i + 3] != '=') ? base64_index(input[i + 3]) : 0;

        base64_res[j] = (a << 2) | (b >> 4);
        base64_res[j + 1] = ((b & 0x0F) << 4) | (c >> 2);
        base64_res[j + 2] = ((c & 0x03) << 6) | d;
    }
    base64_res[output_len] = 0;

    return output_len;
}