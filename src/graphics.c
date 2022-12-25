#include <stdlib.h>
#include <stdio.h>

#include "../inc/graphics.h"
#include "../inc/hijri_cal.h"
#include "../inc/graphics.h"
#include "../inc/prayer.h"


int showTable(prayers_t prayer_times, date_t date){


    // Calculate a integer number from the current time
    int current_time_num = date.current_hour * 60 + date.current_min;
    /*****************************/
    /** Output And Format Table **/
    /*****************************/

    system("ascii-image-converter /home/strayker/Pictures/salafetch/logo.png --color --complex -b -d 65,20");

    YEL; 
    printf("\n"); 
    printf(" +---------+---------+---------+---------+---------+---------+\n"); 
    printf(" | %-7s | %-7s | %-7s | %-7s | %-7s | %-7s |\n", "FAJR", "SHURUK", "DHUHR", "ASR", "MAGHRIB", "ISHA"); 
    printf(" +---------+---------+---------+---------+---------+---------+\n"); 

    RESET_COLOUR;

    YEL; printf(" | ");
    ((current_time_num > prayer_times.prayer.prayer_nums[0])&&
     (current_time_num < prayer_times.prayer.prayer_nums[1])) ? YEL : WHT;
    printf("%-6s ", prayer_times.prayer.fajr);

    YEL; printf(" | ");
    ((current_time_num > prayer_times.prayer.prayer_nums[1])&&
     (current_time_num < prayer_times.prayer.prayer_nums[2])) ? RED : WHT;
    printf("%-6s ", prayer_times.prayer.shuruk);

    YEL; printf(" | ");
    ((current_time_num > prayer_times.prayer.prayer_nums[2])&&
     (current_time_num < prayer_times.prayer.prayer_nums[3])) ? RED : WHT;
    printf("%-6s ", prayer_times.prayer.dhuhr);

    YEL; printf(" | ");
    ((current_time_num > prayer_times.prayer.prayer_nums[3])&&
     (current_time_num < prayer_times.prayer.prayer_nums[4])) ? RED : WHT;
    printf("%-6s ", prayer_times.prayer.asr);

    YEL; printf(" | ");
    ((current_time_num > prayer_times.prayer.prayer_nums[4])&&
     (current_time_num < prayer_times.prayer.prayer_nums[5])) ? RED : WHT;
    printf("%-6s ", prayer_times.prayer.maghrib);

    YEL; printf(" | ");
    ((current_time_num > prayer_times.prayer.prayer_nums[5])) ? RED : WHT;
    printf("%-6s ", prayer_times.prayer.isha); CRESET;

    YEL; printf(" |\n");
    printf(" +---------+---------+---------+---------+---------+---------+\n"); 
    RESET_COLOUR;

    YEL; 
    printf(" | %-7s |", "Hijri"); RESET_COLOUR;
    WHT; printf("    %02d-%02d-%4d     ", 
            prayer_times.current_date.hijri_day,
            prayer_times.current_date.hijri_month,
            prayer_times.current_date.hijri_year); 

    YEL; 
    printf("| %-7s |", "Greg"); RESET_COLOUR;
    WHT; printf("    %02d-%02d-%4d     ", 
            prayer_times.current_date.current_day,
            prayer_times.current_date.current_month,
            prayer_times.current_date.current_year); 
    YEL; 
    printf("|"); RESET_COLOUR;

    YEL; 
    printf("\n"); 

    printf(" +---------+---------+---------+---------+---------+---------+\n"); 

    printf(" | %-7s |", "Temp"); RESET_COLOUR;
    WHT; printf(" %04.1f°C  ", prayer_times.weather.tmp);

    YEL; printf("| %-7s |", "Weather"); RESET_COLOUR;
    WHT; printf(" %-28s", prayer_times.weather.info);
    YEL; printf("|");

    printf("\n +---------+---------+---------+---------+---------+---------+\n"); 
    RESET_COLOUR;

    return 0;
}
