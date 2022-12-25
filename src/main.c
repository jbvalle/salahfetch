#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

#define RESET_COLOUR    printf("\033[0m")
#define CRESET printf("\e[0m")
#define BG printf("\e[0;103m")
#define YEL printf("\e[1;93m")
#define WHT printf("\e[1;90m")
//#define RED printf("\e[1;97m")
#define RED printf("\e[1;97m")

// Intervall Api Access
#define API_INTERVALL 40

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

typedef struct weather_t{
    double tmp;
    char info[28];
}weather_t;

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

    weather_t weather;
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

// If the return value is 0 this indicates that API access is allowed
// If the return value is other than 0 this mean that no API call should be made
int check_last_api_access(prayers_t *prayer_times){

    char filepath[] = "/home/strayker/.config/weather/last_access\0";

    FILE *fp = fopen(filepath, "a+");

    // Error handling checking valid file pointer
    if(fp == NULL){

        fprintf(stderr, "ERROR Reading Weather File");
        return -1;
    }

    
    //Check if file is empty
    //If it is empty overwrite it with the current time and exit 
    struct stat st;
    if((stat(filepath, &st) == 0) && (st.st_size == 0)){

        WHT;
        printf("Info: last_access File was not found\n");
        printf("Info: new last_access File is being generated, API_CALL will be initiated\n");
        RESET_COLOUR;

        // Overwrite the content
        fprintf(fp, "%d:%d", 
                prayer_times->current_date.current_hour,
                prayer_times->current_date.current_min);


        fclose(fp); fp = NULL;
        // If the file has not been created before the current time is written to it and 0 is returned meaning api access is permitted
        return 0;
    }

    // Read File
    char buff[100]; fgets(buff, sizeof(buff), fp);
    int hour, min, diff;

    // Parse hour and min from file
    sscanf(buff, "%d:%d", &hour, &min);

    // Determine Difference is greater than XX min
    diff = abs((prayer_times->current_date.current_hour * 60 + prayer_times->current_date.current_min) - (hour * 60 + min));

    // If the difference is smaller than the API intervall do nothing and exist
    if(diff <= API_INTERVALL){

        fclose(fp); fp = NULL;
        return -1;
    }

    // If the diff is bigger than the API Intervall than overwrite the file by repositioning the pointer and overwrite current time
    // reposition the file pointer /to the beginning of the file
    //(void)fseek(fp, 0, SEEK_SET) ;
    fp = fopen(filepath, "w+");

    // Overwrite the content
    fprintf(fp, "%d:%d", 
            prayer_times->current_date.current_hour,
            prayer_times->current_date.current_min);
                
    fclose(fp); fp = NULL;
    return 0;
}

int retrieve_weather(prayers_t *prayer_times){


    FILE *fp = NULL;
    char proc_buff[50];
    char weather_info_filepath[] = "/home/strayker/.config/weather/info\0";

    // if check() == 0 -> make an API CALL otherwise just read the info file
    if(check_last_api_access(prayer_times) == 0){
        
        // Retrieve the return value of the weather process
        FILE *proc_ptr = popen("get_weather", "r");
        fgets(proc_buff, sizeof(proc_buff), proc_ptr);
        pclose(proc_ptr); proc_ptr = NULL;
    
        WHT;
        printf("Info: API Call initiated, minimum API period exceeded\n");
        RESET_COLOUR;
        
        // If API Call is valid create the file with rw permissions and 
        fp = fopen(weather_info_filepath, "w+");
        // Write the get_weather process output to info file 
        fprintf(fp, "%s", proc_buff); fclose(fp); fp = NULL;

        // reassign file pointer to same file but with read permission
        fp = fopen(weather_info_filepath, "r+");

    }else{

        // Create file pointer to info file in order to read it
        fp = fopen(weather_info_filepath, "a+");

        // Error handling checking valid file pointer
        if(fp == NULL){

            fprintf(stderr, "ERROR Reading Weather File");
            return -1;
        }

        struct stat st;
        if((stat(weather_info_filepath, &st) == 0) && (st.st_size == 0)){

            WHT;
            printf("\nInfo: info file was not found\n");
            printf("\nInfo: API call needs to be manually initiated - \nExecute: prayer_times API_CALL\n");
            RESET_COLOUR;

            // Overwrite the content
            fprintf(fp, "0, API Call necessary");

            fclose(fp); fp = NULL;
            fp = fopen(weather_info_filepath, "r+");
        }
    }

    //Initialize tmp and info values
    prayer_times->weather.tmp = 9999;
    for(int i = 0;i < (int)sizeof(prayer_times->weather.info); i++){

        prayer_times->weather.info[i] = '\0';
    }

    // Token for strok
    // tmp for buffer for strtok
    // seperator for delimiter for strtok
    char *token, buff[100], seperator[3] = ",\n";
    char letter;

    // Read first line of input stream to dump header
    fgets(buff, sizeof(buff), fp);

    // Read First token of string
    token = strtok(buff, seperator); 

    // Parse the tokens and save prayer times
    for(int i = 0; (token != NULL ) && (i < 30); i++){

        // First argument -> min TMP
        if(i == 0)sscanf(token, "%lf", &prayer_times->weather.tmp);
        // Second argument -> max TMP
        if(i == 1){

            for(int j = 0; ((letter = token[j]) != '\0') && (j < 30 ); j++){

                prayer_times->weather.info[j] = letter;
            }
        }
        token = strtok(NULL, seperator);
    }

    fclose(fp); fp = NULL;

    return 0;

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

    printf(" | %-7s |", "Temp"); RESET_COLOUR;
    WHT; printf(" %04.1f°C  ", prayer_times.weather.tmp);

    YEL; printf("| %-7s |", "Weather"); RESET_COLOUR;
    WHT; printf(" %-28s", prayer_times.weather.info);
    YEL; printf("|");

    printf("\n +---------+---------+---------+---------+---------+---------+\n"); 
    RESET_COLOUR;

    return 0;
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
