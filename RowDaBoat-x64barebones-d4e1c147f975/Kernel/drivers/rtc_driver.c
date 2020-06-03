#include <rtc_driver.h>
#include <libasm64.h>

void sys_getTime(TimeFormat *time){

    TimeFormat timeAux;
    int ok = 0;

    while( !ok ){
        getTimeRTC( time );
        getTimeRTC( &timeAux );
        if( time->secs == timeAux.secs && time->mins == timeAux.mins && time->hours == timeAux.hours ){
            ok = 1;
        }
    }
  
}