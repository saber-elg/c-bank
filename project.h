#ifndef PROJECT_H
#define PROJECT_H
#include<time.h>

Date catch_Date(){
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    Date date;
    date.day = time.tm_mday;
    date.month = time.tm_mon+1;
    date.year = time.tm_year+1900;
    return date;
}


Time catch_Time(){
    time_t t = time(NULL);
    struct tm time = *localtime(&t);
    Time cur_time;
    cur_time.hour = time.tm_hour;
    cur_time.minute = time.tm_min;
    cur_time.second = time.tm_sec;
    return cur_time;
}



#endif