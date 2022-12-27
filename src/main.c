#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#include "main.h"


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
