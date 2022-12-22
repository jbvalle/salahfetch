#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RESET_COLOUR    printf("\033[0m")
#define CRESET printf("\e[0m")
#define BG printf("\e[0;103m")
#define YEL printf("\e[1;93m")
#define WHT printf("\e[1;90m")
//#define RED printf("\e[1;97m")
#define RED printf("\e[1;97m")

// Struct for storing current prayer times
typedef struct salah_t{

    char fajr[6];
    char shuruk[6];
    char dhuhr[6];
    char asr[6];
    char maghrib[6];
    char isha[6];
    int prayer_nums[6];
}salah_t;


// Struct for temporarily storing date information
typedef struct date_t{

    int current_day;
    int current_month;
    int current_year;
    int current_hour;
    int current_min;
    int hijri_day;
    int hijri_month;
    int hijri_year;
}date_t;

// Main struct for storing prayer times and current date
typedef struct prayers_t{

    date_t current_date;
    salah_t prayer;
}prayers_t;




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

void retrieve_hijri_date(prayers_t *prayer_times){
    
    char buff[20];
    FILE *fp = popen("hijra_cal", "r");
    fgets(buff, sizeof(buff), fp);

    sscanf(buff, "%d-%d-%d", 
            &prayer_times->current_date.hijri_day,
            &prayer_times->current_date.hijri_month,
            &prayer_times->current_date.hijri_year);

    pclose(fp); fp = NULL;
}

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


    return 0;
}

int main(void){


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

    // Display formatted prayer times
    showTable(prayer_times, date);


    return 0;
}
