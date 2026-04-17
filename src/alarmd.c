#define MINIAUDIO_IMPLEMENTATION
#include "../includes/miniaudio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <syslog.h>
#include <ctype.h>
#include <math.h> 

#define ALARM_ASSET_PATH "/usr/local/share/alarmd/alarm.wav"

int is_numeric(char *str)
{
    if(*str == '\0') return 0;
    while(*str){
        if(isdigit(*str)) return 1;
        str++;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // alarmd YYYY MM DD HH MM SS
     
    if(argc != 7){
        printf("USAGE: alaramd YYYY MM DD HH MM SS\n");
        exit(EXIT_FAILURE);
    }
    for(int i = 1; i < 7; i++){
        if(!is_numeric(argv[i])){
            fprintf(stderr, "ERROR: INVALID CHARACTER\n");
            exit(EXIT_FAILURE);
        }
    }
    ma_engine engine;
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS){
        fprintf(stderr, "Miniaudio failed to initialize\n");
        exit(EXIT_FAILURE);
    }
    char target_time[80];

    snprintf(target_time, sizeof(target_time), "%s %s %s %s %s %s",
            argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]);
    
    struct tm alarm_tm = {0};
    alarm_tm.tm_year = atoi(argv[1]) - 1900;
    alarm_tm.tm_mon = atoi(argv[2]) - 1;
    alarm_tm.tm_mday = atoi(argv[3]);
    alarm_tm.tm_hour = atoi(argv[4]);
    alarm_tm.tm_min = atoi(argv[5]);
    alarm_tm.tm_sec = atoi(argv[6]);
    
    time_t alarm_time = mktime(&alarm_tm);
    
    time_t now = time(NULL);

    
    if(alarm_time < now) { 
        
        fprintf(stderr, "ERROR: Target time must be in future !!\n");
        exit(EXIT_FAILURE);
    }

    openlog("alarmd",LOG_PID, LOG_DAEMON);
    
    syslog(LOG_INFO, "Alarm daemon started for %s", target_time);

    while(1){
        time_t rawtime = time(NULL);

        if(rawtime >= alarm_time){
            syslog(LOG_WARNING, "AlARM TRIGGERED!!");
            ma_engine_play_sound(&engine, ALARM_ASSET_PATH, NULL);
            sleep(10);
            break;
        }
    
        sleep(1);
    }

    syslog(LOG_WARNING, "ALARM DAEMON EXITING...");
    ma_engine_uninit(&engine);
    
    closelog();
    
}
