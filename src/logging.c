#include "include/logging.h"

#define LOG_MAX_SIZE 1024 * 1024 // 1 MB

const char *log_path = "/tmp/webdogcom.log";
const char *log_path_bak = "/tmp/webdogcom_bak.log";

/*
 * 将msg写入log中
 * @param msg
 */
void logging(char *msg) {
    FILE *ptr_file;
    ptr_file = fopen(log_path, "a");
    if (ptr_file == NULL) {
        perror("Error reopening log file");
        return;
    }

    char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    time_t timep;
    struct tm *p;
    time(&timep);

    p = localtime(&timep);
    fprintf(ptr_file, "[%04d/%02d/%02d %s %02d:%02d:%02d] ",
            (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);

    fprintf(ptr_file, "%s\n", msg);

    // 检查日志文件大小
    fseek(ptr_file, 0, SEEK_END);
    long log_size = ftell(ptr_file);

    if (log_size >= LOG_MAX_SIZE) {
        fclose(ptr_file);

        // 滚动日志文件
        rename(log_path, log_path_bak);

        // 重新打开日志文件
        ptr_file = fopen(log_path, "a");

        if (ptr_file == NULL) {
            perror("Error reopening log file");
            return;
        }
    }

    fclose(ptr_file);
}
