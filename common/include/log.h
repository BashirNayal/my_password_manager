#pragma once

#include <stdio.h>
#include <string.h>

#define CLR_DEF "\x1B[0m"
#define CLR_RED "\x1B[31m"
#define CLR_GRN "\x1B[32m"
#define CLR_YEL "\x1B[33m"
#define CLR_BLU "\x1B[34m"
#define CLR_MAG "\x1B[35m"
#define CLR_CYN "\x1B[36m"
#define CLR_WHT "\x1B[37m"

// spcify a color to print provided text in
#define CLR_TXT(clr, txt) clr txt CLR_DEF 
// add cyan color to provided text
#define CLR_VAR(txt) CLR_CYN txt CLR_DEF


// Example:     
// printf("\x1B[0m""|%-8s|%-5s|%20s:%4d|""%s %s","SOURCE","TYPE","LOCATION",1,"test777","test999\n");

#ifdef DEBUG
#define LOG_FMT(color, level_str, tag, fmt, ...)                               \
    do                                                                         \
    {                                                                          \
        printf(color "|%-8s|%-5s|%20s:%-4d|" CLR_DEF fmt, tag, level_str,      \
               __FILE__, __LINE__, ##__VA_ARGS__);                             \
    } while (0)
#else
#define LOG_FMT(color, level_str, tag, fmt, ...)                               \
    do                                                                         \
    {                                                                          \
    } while (0);
#endif

#define log_info(tag, fmt, ...)                                                \
    LOG_FMT(CLR_BLU, "INFO", tag, fmt, ##__VA_ARGS__)
#define log_warn(tag, fmt, ...)                                                \
    LOG_FMT(CLR_YEL, "WARN", tag, fmt, ##__VA_ARGS__)
#define log_err(tag, fmt, ...)                                                 \
    LOG_FMT(CLR_RED, "ERROR", tag, fmt, ##__VA_ARGS__)
#define log_dbg(tag, fmt, ...)                                                 \
    LOG_FMT(CLR_GRN, "DEBUG", tag, fmt, ##__VA_ARGS__)
