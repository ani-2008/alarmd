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
    
    openlog("alarmd",LOG_PID, LOG_DAEMON);
    
    syslog(LOG_INFO, "Alarm daemon started for %s", target_time);

    if(daemon(0,0) == -1){
        perror("ERROR: DAEMON FAILED\n");
        exit(EXIT_FAILURE);
    } 
    
    char *sound = "/usr/bin/mpv --no-video --quiet /home/bitabyte/Desktop/alarm.wav &"; // insert your own music file and music player   
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
