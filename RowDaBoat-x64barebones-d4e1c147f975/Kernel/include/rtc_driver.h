#ifndef RTC_DRIVER_H_
#define RTC_DRIVER_H_
#include <stdint.h>

typedef struct{
    uint32_t secs;
    uint32_t mins;
    uint32_t hours;
}TimeFormat;

void sys_getTime(TimeFormat *time);

#endif
