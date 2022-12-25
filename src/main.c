#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include "../inc/graphics.h"
#include "../inc/prayer.h"
#include "../inc/hijri_cal.h"
#include "../inc/weather.h"



void parseFilename(char *rel_filename, date_t *date){

    // Fetch current Date
    time_t now;
    struct tm *current;

    /* Get the current time. */
    time(&now);

    /* Convert it to a structure. */
    current = localtime(&now);

    // Predefine filename struct with individual files of prayerstimes
    char months[12][7] = {
        "01.csv",
        "02.csv",
        "03.csv",
        "04.csv",
        "05.csv",
        "06.csv",
        "07.csv",
        "08.csv",
        "09.csv",
        "10.csv",
        "11.csv",
        "12.csv"};

    // Define source directory of CSV files 
    strcpy(rel_filename, "/home/strayker/.config/salahfetch/") ; strcat(rel_filename, months[current->tm_mon]);

    //YEAR  : current->tm_year + 1900
    //MONTH : current->tm_mon  + 1
    //DAY   : current->tm_mday
    date->current_day   = current->tm_mday;
    date->current_month = current->tm_mon + 1;
    date->current_year  = current->tm_year + 1900;
    date->current_hour  = current->tm_hour;
    date->current_min   = current->tm_min;
}

int main(int argc, char **argv){


    // Manual API Call Initiation 
    char api_call_make[] = "make -f /home/strayker/Coding/044_prayertimes/Makefile api_call\0";
    char hard_reset[]    = "make -f /home/strayker/Coding/044_prayertimes/Makefile reset\0";
    
    if((argc > 1)&&(strcmp(argv[1], "API_CALL") == 0)){

        system(api_call_make);
    }
    if((argc > 1)&&(strcmp(argv[1], "reset") == 0)){

        system(hard_reset);
    }

    // Instantiate prayer_times for storing 5 prayer times of the day
    prayers_t prayer_times;

    // Instantiate date struct
    date_t date;

    // Get the correct file depending on the current date
    char rel_filename[150]; parseFilename(rel_filename, &date);

    // Reassign date to struct
    prayer_times.current_date = date;

    // Create Pointer to File Stream
    FILE *input = fopen(rel_filename, "r");

    // Parse prayer times and save to struct
    parse_prayerstimes(&prayer_times, input, prayer_times.current_date);

    // free input stream
    fclose(input); input = NULL;

    // Calculate an int number from each prayer time
    determine_prayer_num(&prayer_times);

    retrieve_hijri_date(&prayer_times);

    retrieve_weather(&prayer_times);

    // Display formatted prayer times
    showTable(prayer_times, date);

    return 0;
}
