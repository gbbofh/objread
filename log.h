#ifndef LOG_H
#define LOG_H

#define LOGPATH "/var/log/objread/"
#define LOGNAME "objread.log"

#define LOG_ERROR   0
#define LOG_WARN    1
#define LOG_INFO    2

#define error()     msg_log(LOG_ERROR, "%s", strerror(errno));\
                    close_log();\
                    exit(EXIT_FAILURE);

void open_log();
void close_log();
void msg_log(const int level, const char* format, ...);
void console_log(const char* format, ...);

#endif
