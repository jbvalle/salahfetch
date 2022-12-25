#include "../inc/prayer.h"
#include <stdio.h>
#include <string.h>




void determine_prayer_num(prayers_t *prayer_times){

    int hour, min;

    // Fajr Val
    sscanf(prayer_times->prayer.fajr, "%d:%d", &hour, &min);
    prayer_times->prayer.prayer_nums[0] = hour * 60 + min;

    // Shuruk Val
    sscanf(prayer_times->prayer.shuruk, "%d:%d", &hour, &min);
    prayer_times->prayer.prayer_nums[1] = hour * 60 + min;

    // Dhuhr Val
    sscanf(prayer_times->prayer.dhuhr, "%d:%d", &hour, &min);
    prayer_times->prayer.prayer_nums[2] = hour * 60 + min;

    // Asr Val
    sscanf(prayer_times->prayer.asr, "%d:%d", &hour, &min);
    prayer_times->prayer.prayer_nums[3] = hour * 60 + min;

    // Maghrib Val
    sscanf(prayer_times->prayer.maghrib, "%d:%d", &hour, &min);
    prayer_times->prayer.prayer_nums[4] = hour * 60 + min;

    // isha Val
    sscanf(prayer_times->prayer.isha, "%d:%d", &hour, &min);
    prayer_times->prayer.prayer_nums[5] = hour * 60 + min;
}

int parse_prayerstimes(prayers_t *prayer_times, FILE *input, date_t date){

    // Error handling checking valid file pointer
    if(input == NULL){

        fprintf(stderr, "ERROR Reading CSV File");
        return -1;
    }

    // Token for strok
    // tmp for buffer for strtok
    // seperator for delimiter for strtok
    char *token, tmp[100], seperator[3] = ",\n";

    // Read first line of input stream to dump header
    fgets(tmp, sizeof(tmp), input);

    // Feed input stream to buffer until desired date is reached
    for(int i = 0; ((fgets(tmp, sizeof(tmp), input)) != NULL) && (i < date.current_day - 1); i++);


    // Read First token of string
    token = strtok(tmp, seperator); 

    // Parse the tokens and save prayer times
    for(int i = 0; token != NULL; i++){

        switch(i){

            // Fetch 3. Token -> Shuruk
            case 1 :
                strcpy(prayer_times->prayer.fajr, token);
                break;

                // Fetch 3. Token -> Shuruk
            case 2 :
                strcpy(prayer_times->prayer.shuruk, token);
                break;

                // Fetch 4. Token -> Dhuhr
            case 3 :
                strcpy(prayer_times->prayer.dhuhr, token);
                break;

                // Fetch 5. Token -> Asr}
        case 4 :
                strcpy(prayer_times->prayer.asr, token);
                break;

                // Fetch 6. Token -> Maghrib
        case 5 :
                strcpy(prayer_times->prayer.maghrib, token);
                break;

                // Fetch 6. Token -> Isha
        case 6 :
                strcpy(prayer_times->prayer.isha, token);
                break;
    }
    token = strtok(NULL, seperator);
}
return 0;
}
