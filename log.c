#include <stdarg.h>
#include <limits.h>
#include "common.h"

static int logfd = -1;

void open_log() {
    struct stat st;
/*    char buf[PATH_MAX];
    struct stat st;
    if(stat(LOGPATH, &st) == -1) {
        if(mkdir(LOGPATH, 0700) != -1) {
            memset(buf, 0, PATH_MAX);
            getcwd(buf, PATH_MAX);
            chdir(LOGPATH);
        }
        logfd = open(LOGNAME, O_CREAT|O_RDWR, S_IRUSR|S_IRGRP|S_IROTH);
        chdir(buf);
    }*/
    if(stat(LOGNAME, &st) != -1) {
        unlink(LOGNAME);
    }
    logfd = open(LOGNAME, O_CREAT|O_RDWR, S_IRUSR|S_IRGRP|S_IROTH);
    if(logfd < 0) {
        console_log(LOG_ERROR, "%s", strerror(errno));
    }
}

void close_log() {
    if(logfd >= 0) {
        close(logfd);
        logfd = -1;
    }
}

void msg_log(const int level, const char* format, ...) {
    char buffer[256];
    if(logfd >= 0) {
        switch(level) {
            case LOG_ERROR:
                snprintf(buffer, 256, "Error 0x%04X <%s,%d>: %s\n",
                        errno, __FILE__, __LINE__, format);
            break;
            case LOG_WARN:
                snprintf(buffer, 256, "Warn <%s,%d>: %s\n",
                        __FILE__, __LINE__, format);
            break;
            case LOG_INFO:
                snprintf(buffer, 256, "Info <%s,%d>: %s\n",
                        __FILE__, __LINE__, format);
            break;
            }
            va_list args;
            va_start(args, format);
            vdprintf(logfd, buffer, args);
            va_end(args);
    }
}

void console_log(const int level, const char* format, ...) {
    char buffer[256];
        switch(level) {
            case LOG_ERROR:
                snprintf(buffer, 256, "Error 0x%04X <%s,%d>: %s\n",
                        errno, __FILE__, __LINE__, format);
            break;
            case LOG_WARN:
                snprintf(buffer, 256, "Warn <%s,%d>: %s\n",
                        __FILE__, __LINE__, format);
            break;
            case LOG_INFO:
                snprintf(buffer, 256, "Info <%s,%d>: %s\n",
                        __FILE__, __LINE__, format);
            break;
            }
            va_list args;
            va_start(args, format);
            vdprintf(STDOUT_FILENO, buffer, args);
            va_end(args);
}
