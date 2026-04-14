#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[])
{
    // alarmd YYYY MM DD HH MM SS
    
    if(argc != 7){
        printf("USAGE: alaramd YYYY MM DD HH MM SS\n");
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
        
        fprintf(stderr, "ERROR: DON'T GIVE INVALID INPUT YOU FILTHY LITTLE COCKROACH !!!\n");
        exit(EXIT_FAILURE);
    }

    openlog("alarmd",LOG_PID, LOG_DAEMON);
    
    syslog(LOG_INFO, "Alarm daemon started for %s", target_time);

    if(daemon(0,0) == -1){
        perror("ERROR: DAEMON FAILED\n");
        exit(EXIT_FAILURE);
    } 
    
    char *sound = "/usr/bin/mpv --no-video --quiet /home/bitabyte/Desktop/alarm.wav &";
    char buffer[80];
   
    while(1){
        time_t rawtime = time(NULL);

        struct tm *info = localtime(&rawtime);

        strftime(buffer, sizeof(buffer), "%Y %m %d %H %M %S",info);
 
        if(strcmp(target_time,buffer) == 0){
            syslog(LOG_WARNING, "AlARM TRIGGERED!!");
            system(sound);
            break;
        }
    
        sleep(1);
    }

    syslog(LOG_WARNING, "ALARM DAEMON EXITING...");
    
    closelog();
}
